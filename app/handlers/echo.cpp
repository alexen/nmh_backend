/// @file echo.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <handlers/echo.h>

#include <iostream>

#include <boost/log/trivial.hpp>
#include <boost/iostreams/copy.hpp>


namespace alexen {
namespace handler {


boost::string_view EchoHandler::name() const noexcept
{
     static boost::string_view name = "echo";
     return name;
}


void EchoHandler::process( const boost::string_view method, std::istream& is, std::ostream& os )
{
     BOOST_LOG_TRIVIAL( info ) << name() << ": received call method " << method;
     if( method == "echo" )
     {
          echo( is, os );
     }
     else
     {
          BOOST_THROW_EXCEPTION( std::runtime_error{ "method not supported" } );
     }
}


void EchoHandler::echo( std::istream& is, std::ostream& os )
{
     BOOST_LOG_TRIVIAL( info ) << name() << ": " << __FUNCTION__ << " is working";
     boost::iostreams::copy( is, os );
}


} // namespace handler
} // namespace alexen
