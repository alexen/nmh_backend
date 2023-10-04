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

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/stream.hpp>

#include <boost/algorithm/hex.hpp>

#include <boost/utility/string_view.hpp>

#include <boost/variant.hpp>

#include <boost/lambda/lambda.hpp>


namespace nmh {


void read( std::istream& is, std::ostream& os )
{
     std::uint32_t len = 0;
     if( !is.read( reinterpret_cast< char* >( &len ), sizeof( len ) ) )
     {
          BOOST_THROW_EXCEPTION(
               boost::enable_error_info( std::runtime_error{ "nmh len read failed" } )
                    << boost::errinfo_errno{ errno } );
     }

     BOOST_LOG_TRIVIAL( debug ) << "nmh: incoming bytes: " << len;

     std::vector< char > buffer( len );
     if( !is.read( buffer.data(), buffer.size() ) )
     {
          BOOST_THROW_EXCEPTION(
               boost::enable_error_info( std::runtime_error{ "nmh data read failed" } )
                    << boost::errinfo_errno{ errno } );
     }
     os.write( buffer.data(), buffer.size() );

     BOOST_LOG_TRIVIAL( debug ) << "nmh: received bytes: " << is.gcount();
}


void write( std::ostream& os, const std::string& data )
{
     BOOST_LOG_TRIVIAL( info )
          << "Write data: " << data;

     const std::uint32_t len = data.size();
     os.write( reinterpret_cast< const char* >( &len ), sizeof( len ) )
          << data;
}


} // namespace nmh


void initLogSinks()
{
     boost::log::add_common_attributes();
     boost::log::add_console_log( std::cerr,
          boost::log::keywords::format = "%TimeStamp% [%Severity%] %Message%" );
     boost::log::add_file_log( "/tmp/nmh_backend.log",
          boost::log::keywords::format = "[%TimeStamp%] {%ProcessID%.%ThreadID%} <%Severity%>: %Message%",
          boost::log::keywords::auto_flush = true,
          boost::log::keywords::open_mode = std::ios_base::out | std::ios_base::app
     );
}


struct Hexlify {
     using InputVariant = boost::variant< boost::string_view, std::istream& >;

     explicit Hexlify( InputVariant&& v ) : input{ v } {}

     InputVariant input;
};


struct HexlifyVisitor : boost::static_visitor<> {
     explicit HexlifyVisitor( std::ostream& os ) : ostr{ os } {}

     void operator()( std::istream& is ) const
     {
          boost::algorithm::hex_lower(
               std::istreambuf_iterator< char >{ is },
               {},
               std::ostreambuf_iterator< char >{ ostr }
               );
     }

     void operator()( boost::string_view sv ) const
     {
          boost::algorithm::hex_lower(
               sv.cbegin(),
               sv.cend(),
               std::ostreambuf_iterator< char >{ ostr }
               );
     }

     std::ostream& ostr;
};


std::ostream& operator<<( std::ostream& os, const Hexlify& h )
{
     boost::apply_visitor( HexlifyVisitor{ os }, h.input );
     return os;
}


int main()
{
     using TeeDevice = boost::iostreams::tee_device< std::istream, std::stringstream >;
     using TeeStream = boost::iostreams::stream< TeeDevice >;

     try
     {
          initLogSinks();

          std::stringstream ioss{
               std::ios_base::in | std::ios_base::out | std::ios_base::binary
          };
          TeeDevice teeDevice{ std::cin, ioss };
          TeeStream teeStream{ teeDevice };

          BOOST_LOG_TRIVIAL( info ) << "Start listening STDIN...";

          std::ostringstream request;
          nmh::read( std::cin, request );

          BOOST_LOG_TRIVIAL( info )
               << "Read data: " << request.str();
          BOOST_LOG_TRIVIAL( debug )
               << "Tee stream: " << Hexlify{ ioss };

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

          nmh::write( std::cout, json.str() );
     }
     catch( ... )
     {
          BOOST_LOG_TRIVIAL( error ) << "exception: "
               << boost::current_exception_diagnostic_information();
     }
}
