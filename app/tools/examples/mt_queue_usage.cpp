/// @file mt_queue_usage.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <stdexcept>
#include <iostream>

#include <boost/log/trivial.hpp>
#include <boost/thread/thread.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <tools/mt/queue.h>


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          boost::thread_group tg;
          tg.create_thread( []{} );
          tg.create_thread( []{} );
          tg.create_thread( []{} );
          tg.create_thread( []{} );
          tg.join_all();
     }
     catch( ... )
     {
          std::cerr << "exception: " << boost::current_exception_diagnostic_information() << '\n';
          return 1;
     }
     return 0;
}
