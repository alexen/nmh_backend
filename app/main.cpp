/// @file main.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <iostream>

#include <boost/bind/bind.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/thread.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/log/trivial.hpp>

#include <nmh/io.h>
#include <nmh/workers.h>

#include <tools/logger/init.h>
#include <tools/converter/hexlify.h>
#include <tools/mt/queue.h>

#include <handlers/echo.h>


template< typename Handler, typename ...Args >
alexen::handler::IHandlerMap::value_type makeHandlerEntry( Args&& ...args)
{
     auto handler = boost::make_shared< Handler >( std::forward< Args >( args )... );
     return { handler->name().to_string(), handler };
}


int main()
{
     try
     {
          alexen::tools::logger::initFileLog( "/tmp/nmh_backend.log", boost::log::trivial::trace );
          alexen::tools::logger::initOstreamLog( std::cerr, boost::log::trivial::info );

          BOOST_LOG_TRIVIAL( info ) << "Main thread start";

          alexen::nmh::mt::RequestQueue requestQueue;
          alexen::nmh::mt::ResponseQueue responseQueue;
          alexen::handler::IHandlerMap handlers{
               makeHandlerEntry< alexen::handler::EchoHandler >()
          };

          boost::thread_group tg;

          unsigned workers = 3;
          while( workers-- )
          {
               tg.create_thread(
                    boost::bind(
                         alexen::nmh::mt::workers::requestProcessor
                         , boost::ref( requestQueue )
                         , boost::ref( responseQueue )
                         , boost::cref( handlers )
                         )
                    );
          }

          tg.create_thread(
               boost::bind(
                    alexen::nmh::mt::workers::ostreamWriter
                    , boost::ref( std::cout )
                    , boost::ref( responseQueue )
                    )
               );

          alexen::nmh::mt::workers::istreamListener( std::cin, requestQueue, responseQueue );

          BOOST_LOG_TRIVIAL( info ) << "Input stream closed, finish working";

          tg.interrupt_all();
          tg.join_all();

          BOOST_LOG_TRIVIAL( info ) << "Main thread finished";
     }
     catch( ... )
     {
          BOOST_LOG_TRIVIAL( error ) << "exception: "
               << boost::current_exception_diagnostic_information();
     }
}
