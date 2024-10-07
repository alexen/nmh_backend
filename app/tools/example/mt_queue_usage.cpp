/// @file mt_queue_usage.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <deque>
#include <stdexcept>
#include <iostream>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/bind/bind.hpp>
#include <boost/log/trivial.hpp>
#include <boost/thread/thread.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <tools/mt/queue.h>


template< typename T >
void producer( alexen::tools::mt::BlockingQueue< T >& queue )
{
     static boost::uuids::random_generator gen;

     try
     {
          for( auto i=0; i<100; ++i )
          {
               const auto value = boost::uuids::to_string( gen() );
               queue.push( value );
               BOOST_LOG_TRIVIAL( info ) << __FUNCTION__ << ": add " << value;
               boost::this_thread::sleep( boost::posix_time::milliseconds{ 5 } );
          }
     }
     catch( const std::exception& )
     {
          BOOST_LOG_TRIVIAL( error )
               << "exception: " << boost::current_exception_diagnostic_information();
     }
}


template< typename T >
void consumer( alexen::tools::mt::BlockingQueue< T >& queue )
{
     try
     {
          while( true )
          {
               const auto value = queue.pop();
               BOOST_LOG_TRIVIAL( info ) << __FUNCTION__ << ": got " << value;
               boost::this_thread::sleep( boost::posix_time::milliseconds{ 30 } );
          }
     }
     catch( const std::exception& )
     {
          BOOST_LOG_TRIVIAL( error )
               << "exception: " << boost::current_exception_diagnostic_information();
     }
}


int main( int argc, char** argv )
{
     using namespace std::string_literals;

     boost::ignore_unused( argc, argv );
     try
     {
          alexen::tools::mt::BlockingQueue< std::string > q;

          boost::thread_group tg;
          tg.create_thread( boost::bind( producer< std::string >, boost::ref( q ) ) );
          tg.create_thread( boost::bind( producer< std::string >, boost::ref( q ) ) );

          tg.create_thread( boost::bind( consumer< std::string >, boost::ref( q ) ) );
          tg.create_thread( boost::bind( consumer< std::string >, boost::ref( q ) ) );
          tg.create_thread( boost::bind( consumer< std::string >, boost::ref( q ) ) );
          tg.create_thread( boost::bind( consumer< std::string >, boost::ref( q ) ) );
          tg.create_thread( boost::bind( consumer< std::string >, boost::ref( q ) ) );

          std::cout << "Press ENTER to stop this shit..." << std::flush;
          std::cin.get();

          tg.interrupt_all();
          tg.join_all();

          std::cout << "Queue: " << q << '\n';
     }
     catch( ... )
     {
          std::cerr << "exception: " << boost::current_exception_diagnostic_information() << '\n';
          return 1;
     }
     return 0;
}
