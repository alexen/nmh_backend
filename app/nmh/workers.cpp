/// @file workers.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <nmh/workers.h>

#include <sstream>

#include <boost/log/trivial.hpp>
#include <boost/make_shared.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/thread/tss.hpp>

#include <nmh/io.h>


namespace alexen {
namespace nmh {


static std::ostream& operator<<( std::ostream& os, const Request& rq )
{
     return os
          << "id: " << rq.id
          << ", method: " << rq.method
          << ", data: " << rq.data
          << ", timestamp: " << rq.timestamp;
}


namespace mt {
namespace workers {


void istreamListener( std::istream& is, RequestQueue& requestQueue, ResponseQueue& responseQueue )
{
     BOOST_LOG_TRIVIAL( info ) << __FUNCTION__ << " started";

     try
     {
          std::stringstream request;
          while( alexen::nmh::io::read( is, request ) )
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


void ostreamWriter( std::ostream& os, ResponseQueue& responseQueue )
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
               auto&& item = responseQueue.pop();
               item->serialize( *oss );
               alexen::nmh::io::write( oss->str(), os );
          }
     }
     catch( const std::exception& )
     {
          BOOST_LOG_TRIVIAL( error )
                    << "exception: " << boost::current_exception_diagnostic_information();
     }

     BOOST_LOG_TRIVIAL( info ) << __FUNCTION__ << " finished";
}


void requestProcessor(
     RequestQueue& requestQueue
     , ResponseQueue& responseQueue
     , const handler::IHandlerMap& handlers
)
{
     BOOST_LOG_TRIVIAL( info ) << __FUNCTION__ << " started";

     try
     {
          while( true )
          {
               const auto request = requestQueue.pop();
               BOOST_LOG_TRIVIAL( info ) << "Processing: " << *request;

               try
               {
                    const auto handler = handlers.find( request->module );
                    if( handler == handlers.end() )
                    {
                         BOOST_THROW_EXCEPTION( std::runtime_error{ "module not found" } );
                    }

                    std::istringstream iss{ request->data };
                    std::ostringstream oss;

                    handler->second->process( request->method, iss, oss );

                    responseQueue.push(
                         makeResponse(
                              request->id
                              , "processed"
                              , oss.str()
                              )
                         );
               }
               catch( const std::exception& e )
               {
                    BOOST_LOG_TRIVIAL( error )
                         << "exception: " << boost::current_exception_diagnostic_information();
                    responseQueue.push( makeErrorResponse( request->id, e.what() ) );
               }
          }
     }
     catch( const std::exception& )
     {
          BOOST_LOG_TRIVIAL( error )
                    << "exception: " << boost::current_exception_diagnostic_information();
     }

     BOOST_LOG_TRIVIAL( info ) << __FUNCTION__ << " finished";
}


} // namespace workers
} // namespace mt
} // namespace nmh
} // namespace alexen
