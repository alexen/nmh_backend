/// @file main.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <unistd.h>

#include <sstream>
#include <iostream>

#include <boost/date_time.hpp>
#include <boost/throw_exception.hpp>
#include <boost/exception/errinfo_errno.hpp>
#include <boost/exception/enable_error_info.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/log/trivial.hpp>
#include <boost/variant.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <nmh/nmh.h>
#include <tools/logger/init.h>
#include <tools/converter/hexlify.h>


std::tuple< int, std::string > parseMessage( std::istream& is )
{
     boost::property_tree::ptree root;
     boost::property_tree::read_json( is, root );

     return {
          root.get< int >( "messageId" ),
          root.get< std::string >( "content" )
     };
}


std::string makeResponseTo( const int messageId, boost::string_view message )
{
     static std::ostringstream ostr;

     ostr.str( {} );

     boost::property_tree::ptree root;

     root.put( "processId", getpid() );
     root.put( "replyTo", messageId );
     root.put( "status", "accepted" );
     root.put( "sourceMessage", message );
     root.put( "timestamp", boost::posix_time::to_iso_extended_string( boost::posix_time::second_clock::local_time() ) );

     boost::property_tree::write_json( ostr, root );

     return ostr.str();
}


int main()
{
     try
     {
          alexen::tools::logger::initFileLog( "/tmp/nmh_backend.log", boost::log::trivial::trace );
          alexen::tools::logger::initOstreamLog( std::cerr, boost::log::trivial::info );

          std::stringstream request;
          while( alexen::nmh::protocol::read( std::cin, request ) )
          {
               BOOST_LOG_TRIVIAL( info )
                    << "Raw data incoming: " << request.str();

               try
               {
                    const auto& [ messageId, content ] = parseMessage( request );

                    BOOST_LOG_TRIVIAL( info )
                         << "Parsed message: id: " << messageId
                         << ", content: " << std::quoted( content );

                    alexen::nmh::protocol::write( makeResponseTo( messageId, content ), std::cout );
               }
               catch( const boost::property_tree::ptree_error& )
               {
                    static const std::string errorResponse =
                         R"json({
                              "state": "error",
                              "reason": "Bad incoming message format"
                         })json";

                    BOOST_LOG_TRIVIAL( error )
                         << "exception: " << boost::current_exception_diagnostic_information();
                    alexen::nmh::protocol::write( errorResponse, std::cout );
               }

               request.str( {} );
          }
          BOOST_LOG_TRIVIAL( info ) << "Seems to finished working!";
     }
     catch( ... )
     {
          BOOST_LOG_TRIVIAL( error ) << "exception: "
               << boost::current_exception_diagnostic_information();
     }
}
