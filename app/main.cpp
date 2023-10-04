/// @file main.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <unistd.h>

#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include <boost/date_time.hpp>
#include <boost/throw_exception.hpp>
#include <boost/exception/errinfo_errno.hpp>
#include <boost/exception/enable_error_info.hpp>
#include <boost/exception/diagnostic_information.hpp>


namespace nmh {


void read( std::istream& is, std::ostream& os )
{
     std::uint32_t len = 0;
     if( !is.read( reinterpret_cast< char* >( &len ), sizeof( len ) ) )
     {
          BOOST_THROW_EXCEPTION( boost::enable_error_info( std::runtime_error{ "nmh len read failed" } )
               << boost::errinfo_errno{ errno } );
     }
     std::vector< char > buffer( len );
     if( !is.read( buffer.data(), buffer.size() ) )
     {
          BOOST_THROW_EXCEPTION( boost::enable_error_info( std::runtime_error{ "nmh data read failed" } )
               << boost::errinfo_errno{ errno } );
     }
     os.write( buffer.data(), buffer.size() );
}


void write( std::ostream& os, const std::string& data, std::ostream& log )
{
     log << "Write data: " << data << std::endl;
     const std::uint32_t len = data.size();
     os.write( reinterpret_cast< const char* >( &len ), sizeof( len ) ) << data;
}


}


int main()
{
     std::ofstream log{ "/tmp/nmh_backend.log", std::ios_base::app };

     try
     {
          std::ostringstream request;
          nmh::read( std::cin, request );

          log << "Read data: " << request.str() << std::endl;

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

          nmh::write( std::cout, json.str(), log );
     }
     catch( ... )
     {
          log << "exception: "
               << boost::current_exception_diagnostic_information()
               << std::endl;
     }
}
