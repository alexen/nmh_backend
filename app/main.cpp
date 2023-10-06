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
     static RequestPtr fromJson( std::istream& is )
     {
          boost::property_tree::ptree root;
          boost::property_tree::read_json( is, root );

          return boost::make_shared< Request >(
               root.get< int >( "messageId" ),
               root.get< std::string >( "content" ),
               root.get< std::string >( "timestamp" )
               );
     }

     Request( std::uint32_t msgId, std::string&& content, std::string&& tstamp )
          : messageId{ msgId }
          , content{ content }
          , timestamp{ tstamp }
     {}

     std::uint32_t messageId;
     std::string content;
     std::string timestamp;
};


std::ostream& operator<<( std::ostream& os, const Request& rq )
{
     return os
          << "message: id: " << rq.messageId
          << ", content: " << rq.content
          << ", at " << rq.timestamp;
}


using ResponsePtr = boost::shared_ptr< struct Response >;

struct Response {
     std::string toJson( const int messageId, boost::string_view message )
     {
          /// Делаем статический поток, но отдельный для каждого потока,
          /// для чего используем boost::thread_specific_ptr<>
          static boost::thread_specific_ptr< std::ostringstream > oss;
          if( !oss.get() )
          {
               oss.reset( new std::ostringstream );
          }

          /// Поскольку буфер статический, очистку выполняем при каждом обращении.
          /// Такой метод работы самый быстрый.
          oss->str( {} );

          boost::property_tree::ptree root;

          root.put( "processId", getpid() );
          root.put( "replyTo", messageId );
          root.put( "status", "accepted" );
          root.put( "sourceMessage", message );
          root.put( "timestamp", boost::posix_time::to_iso_extended_string( boost::posix_time::second_clock::local_time() ) );

          boost::property_tree::write_json( *oss, root );

          return oss->str();
     }

     std::uint32_t messageId;
     std::uint32_t inReplyTo;
     std::string status;
     std::string content;
     std::string timestamp;
};


using RequestQueue = alexen::tools::mt::BlockingQueue< RequestPtr >;
using ResponseQueue = alexen::tools::mt::BlockingQueue< ResponsePtr >;


void requestListener( RequestQueue& requestQueue )
{
     try
     {
          BOOST_LOG_TRIVIAL( info ) << "Start request listener";

          std::stringstream request;
          while( alexen::nmh::protocol::read( std::cin, request ) )
          {
               BOOST_LOG_TRIVIAL( debug )
                    << "Received: " << request.str();

               try
               {
                    requestQueue.push( Request::fromJson( request ) );
               }
               catch( const std::exception& )
               {
                    BOOST_LOG_TRIVIAL( error )
                         << "exception: " << boost::current_exception_diagnostic_information();
               }

               request.str( {} );
          }
     }
     catch( const std::exception& )
     {
          BOOST_LOG_TRIVIAL( error )
               << "exception: " << boost::current_exception_diagnostic_information();
     }
}


void requestProcessor( RequestQueue& requestQueue, ResponseQueue& )
{
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
}


int main()
{
     try
     {
          alexen::tools::logger::initFileLog( "/tmp/nmh_backend.log", boost::log::trivial::trace );
          alexen::tools::logger::initOstreamLog( std::cerr, boost::log::trivial::info );

          RequestQueue requestQueue;
          ResponseQueue responseQueue;

          boost::thread_group tg;
          tg.create_thread( boost::bind( requestListener, boost::ref( requestQueue ) ) );
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
                    requestProcessor,
                    boost::ref( requestQueue ),
                    boost::ref( responseQueue )
                    )
               );
          tg.join_all();
     }
     catch( ... )
     {
          BOOST_LOG_TRIVIAL( error ) << "exception: "
               << boost::current_exception_diagnostic_information();
     }
}
