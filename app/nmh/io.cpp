/// @file nmh.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <vector>
#include <iostream>

#include <boost/log/trivial.hpp>
#include <boost/throw_exception.hpp>
#include <boost/exception/enable_error_info.hpp>
#include <boost/exception/errinfo_errno.hpp>
#include <nmh/io.h>


namespace alexen {
namespace nmh {
namespace io {


bool read( std::istream& is, std::ostream& os )
{
     static constexpr bool success = true;

     std::uint32_t len = 0;
     if( !is.read( reinterpret_cast< char* >( &len ), sizeof( len ) ) )
     {
          if( is.eof() )
          {
               return !success;
          }
          BOOST_THROW_EXCEPTION(
               boost::enable_error_info( std::runtime_error{ "nmh len read failed" } )
                    << boost::errinfo_errno{ errno } );
     }

     BOOST_LOG_TRIVIAL( trace ) << "nmh::read: bytes expected: " << len;

     std::vector< char > buffer( len );
     if( !is.read( buffer.data(), buffer.size() ) )
     {
          BOOST_THROW_EXCEPTION(
               boost::enable_error_info( std::runtime_error{ "nmh data read failed" } )
                    << boost::errinfo_errno{ errno } );
     }
     os.write( buffer.data(), buffer.size() );

     BOOST_LOG_TRIVIAL( trace ) << "nmh::read: bytes read: " << is.gcount();

     return success;
}


void write( boost::string_view data, std::ostream& os )
{
     BOOST_LOG_TRIVIAL( trace )
          << "nmh::write: outgoing message: len: " << data.size()
          << ", content: " << data;

     const std::uint32_t len = data.size();
     os.write( reinterpret_cast< const char* >( &len ), sizeof( len ) )
          << data
          << std::flush;
}


} // namespace protocol
} // namespace nmh
} // namespace alexen
