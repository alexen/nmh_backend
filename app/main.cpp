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

#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/stream.hpp>

#include <boost/algorithm/hex.hpp>

#include <boost/utility/string_view.hpp>

#include <boost/variant.hpp>

#include <boost/lambda/lambda.hpp>

#include "nmh/nmh.h"
#include "tools/logger/init.h"
#include "tools/converter/hexlify.h"


int main()
{
     using TeeDevice = boost::iostreams::tee_device< std::istream, std::stringstream >;
     using TeeStream = boost::iostreams::stream< TeeDevice >;

     try
     {
          alexen::tools::logger::initFileLog( "/tmp/nmh_backend.log" );
          alexen::tools::logger::initOstreamLog( std::cerr );

          std::stringstream ioss{
               std::ios_base::in | std::ios_base::out | std::ios_base::binary
          };
          TeeDevice teeDevice{ std::cin, ioss };
          TeeStream teeStream{ teeDevice };

          BOOST_LOG_TRIVIAL( info ) << "Start listening STDIN...";

          std::ostringstream request;
          alexen::nmh::protocol::read( std::cin, request );

          BOOST_LOG_TRIVIAL( info )
               << "Read data: " << request.str();
          BOOST_LOG_TRIVIAL( debug )
               << "Tee stream: " << alexen::tools::converter::Hexlify{ ioss };

          std::ostringstream json;
          json << '{'
               << std::quoted( "processId" ) << ':'
                    << getpid() << ','
               << std::quoted( "greeting" ) << ':'
                    << std::quoted( "Hello, my dear friend!" ) << ','
               << std::quoted( "timestamp" ) << ':'
                    << std::quoted( boost::posix_time::to_simple_string( boost::posix_time::second_clock::local_time() ) ) << ','
               << std::quoted( "sourceMessage" ) << ':'
                    << std::quoted( request.str() )
               << '}';

          alexen::nmh::protocol::write( json.str(), std::cout );
     }
     catch( ... )
     {
          BOOST_LOG_TRIVIAL( error ) << "exception: "
               << boost::current_exception_diagnostic_information();
     }
}
