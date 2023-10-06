/// @file main.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <unistd.h>

#include <sstream>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/make_shared.hpp>
#include <boost/throw_exception.hpp>
#include <boost/optional/optional.hpp>
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
#include <tools/mt/queue.h>


using RequestPtr = boost::shared_ptr< struct Request >;

struct Request {
     static RequestPtr parse( std::istream& is )
     {
          boost::property_tree::ptree root;
          boost::property_tree::read_json( is, root );

          auto request = boost::make_shared< Request >();
          request->id = root.get< int >( "id" );
          request->message = root.get< std::string >( "message" );
          request->timestamp = root.get< std::string >( "timestamp" );
          return request;
     }

     std::uint32_t id;
     std::string message;
     std::string timestamp;
};


std::ostream& operator<<( std::ostream& os, const Request& rq )
{
     return os
          << "message: id: " << rq.id
          << ", message: " << rq.message
          << ", at " << rq.timestamp;
}


using ResponsePtr = boost::shared_ptr< struct Response >;

struct Response {
     void serialize( std::ostream& os )
     {
          boost::property_tree::ptree root;

          if( replyTo )
          {
               root.put( "replyTo", replyTo );
          }
          root.put( "status", status );
          root.put( "message", message );
          root.put( "timestamp", timestamp );

          boost::property_tree::write_json( os, root );
     }

     boost::optional< std::uint32_t > replyTo;
     std::string status;
     std::string message;
     std::string timestamp;
};


using RequestQueue = alexen::tools::mt::BlockingQueue< RequestPtr >;
using ResponseQueue = alexen::tools::mt::BlockingQueue< ResponsePtr >;


void requestListener( RequestQueue& requestQueue, ResponseQueue& responseQueue )
{
     static const auto makeErrorResponse = []( const std::string& message ){
          auto response = boost::make_shared< Response >();
          response->status = "error";
          response->message = message;
          response->timestamp = boost::posix_time::to_iso_extended_string( boost::posix_time::second_clock::local_time() );
          return response;
     };

     BOOST_LOG_TRIVIAL( info ) << __FUNCTION__ << " started";

     try
     {
          std::stringstream request;
          while( alexen::nmh::protocol::read( std::cin, request ) )
          {
               BOOST_LOG_TRIVIAL( debug )
                    << "Received: " << request.str();

               try
               {
                    requestQueue.push( Request::parse( request ) );
               }
               catch( const std::exception& e )
               {
                    BOOST_LOG_TRIVIAL( error )
                         << "exception: " << boost::current_exception_diagnostic_information();
                    responseQueue.push( makeErrorResponse( e.what() ) );
               }

               request.str( {} );
          }
     }
     catch( const std::exception& )
     {
          BOOST_LOG_TRIVIAL( error )
               << "exception: " << boost::current_exception_diagnostic_information();
     }

     BOOST_LOG_TRIVIAL( info ) << __FUNCTION__ << " finished";
}


void requestProcessor( RequestQueue& requestQueue, ResponseQueue& )
{
     BOOST_LOG_TRIVIAL( info ) << __FUNCTION__ << " started";

     try
     {
          while( true )
          {
               const auto request = requestQueue.pop();
               BOOST_LOG_TRIVIAL( info ) << "Processing: " << *request;
          }
     }
     catch( const std::exception& )
     {
          BOOST_LOG_TRIVIAL( error )
                    << "exception: " << boost::current_exception_diagnostic_information();
     }

     BOOST_LOG_TRIVIAL( info ) << __FUNCTION__ << " finished";
}


void responseWriter( ResponseQueue& responseQueue )
{
     static boost::thread_specific_ptr< std::ostringstream > oss;
     if( !oss.get() )
     {
          oss.reset( new std::ostringstream );
     }

     BOOST_LOG_TRIVIAL( info ) << __FUNCTION__ << " started";

     try
     {
          while( true )
          {
               oss->str( {} );
               responseQueue.pop()->serialize( *oss );
               alexen::nmh::protocol::write( oss->str(), std::cout );
          }
     }
     catch( const std::exception& )
     {
          BOOST_LOG_TRIVIAL( error )
                    << "exception: " << boost::current_exception_diagnostic_information();
     }

     BOOST_LOG_TRIVIAL( info ) << __FUNCTION__ << " finished";
}


int main()
{
     try
     {
          alexen::tools::logger::initFileLog( "/tmp/nmh_backend.log", boost::log::trivial::trace );
          alexen::tools::logger::initOstreamLog( std::cerr, boost::log::trivial::info );

          BOOST_LOG_TRIVIAL( info ) << "Main thread start";

          RequestQueue requestQueue;
          ResponseQueue responseQueue;

          boost::thread_group tg;
          tg.create_thread(
               boost::bind(
                    requestListener,
                    boost::ref( requestQueue ),
                    boost::ref( responseQueue )
                    )
               );
          tg.create_thread(
               boost::bind(
                    requestProcessor,
                    boost::ref( requestQueue ),
                    boost::ref( responseQueue )
                    )
               );
          tg.create_thread(
               boost::bind(
                    requestProcessor,
                    boost::ref( requestQueue ),
                    boost::ref( responseQueue )
                    )
               );
          tg.create_thread(
               boost::bind(
                    responseWriter,
                    boost::ref( responseQueue )
                    )
               );
          tg.join_all();

          BOOST_LOG_TRIVIAL( info ) << "Main thread finished";
     }
     catch( ... )
     {
          BOOST_LOG_TRIVIAL( error ) << "exception: "
               << boost::current_exception_diagnostic_information();
     }
}
