/// @file nmh_test.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#define BOOST_TEST_MODULE nmh

#include <sstream>
#include <boost/test/unit_test.hpp>

#include <nmh/nmh.h>


BOOST_AUTO_TEST_SUITE( NmhIo )
BOOST_AUTO_TEST_SUITE( Read )

using alexen::nmh::protocol::read;

BOOST_AUTO_TEST_CASE( ReadEmptyStream )
{
     std::istringstream iss;
     std::ostringstream oss;
     bool success = true;
     BOOST_REQUIRE_NO_THROW( success = read( iss, oss ) );
     BOOST_TEST( !success );
}
BOOST_AUTO_TEST_CASE( ReadValidData )
{
     static const std::string source = "Welcome to the Fantasy World of DiZZy!!!";
     const std::uint32_t len = source.size();

     std::stringstream ioss;
     ioss.write( reinterpret_cast< const char* >( &len ), sizeof( len ) );
     ioss.write( source.data(), source.size() );

     std::ostringstream oss;
     bool success = true;

     /// Первое чтение возвращает результат true и данные в полном соответствии с исходной строкой.
     BOOST_REQUIRE_NO_THROW( success = read( ioss, oss ) );
     BOOST_TEST( success );
     BOOST_TEST( oss.str() == source );

     /// Второе чтение этого же потока не должно выбрасывать исключение, но должно возвращать false
     BOOST_REQUIRE_NO_THROW( success = read( ioss, oss ) );
     BOOST_TEST( !success );
}
BOOST_AUTO_TEST_CASE( ReadInvalidData )
{
     std::stringstream ioss{ "Bad data: no leading integer" };
     std::ostringstream oss;

     BOOST_REQUIRE_THROW( read( ioss, oss ), std::runtime_error );
     BOOST_TEST( oss.str().empty() );
}
BOOST_AUTO_TEST_SUITE_END() // Read
BOOST_AUTO_TEST_SUITE( Write )

using alexen::nmh::protocol::write;

BOOST_AUTO_TEST_CASE( WriteEmptyData )
{
     std::stringstream oss;
     BOOST_REQUIRE_NO_THROW( write( {}, oss ) );

     std::uint32_t len = -1;
     BOOST_TEST( !!oss.read( reinterpret_cast< char* >( &len ), sizeof( len ) ) );
     BOOST_TEST( len == 0 );
}
BOOST_AUTO_TEST_CASE( WriteNonemptyData )
{
     static const std::string source = "Hello, world!";

     std::stringstream ioss;
     BOOST_REQUIRE_NO_THROW( write( source, ioss ) );

     std::uint32_t len = -1;
     BOOST_TEST( !!ioss.read( reinterpret_cast< char* >( &len ), sizeof( len ) ) );
     BOOST_TEST( len == source.size() );

     std::string data( len, 0 );
     BOOST_TEST( !!ioss.read( data.data(), len ) );
     BOOST_TEST( data == source );
}
BOOST_AUTO_TEST_SUITE_END() // Write
BOOST_AUTO_TEST_SUITE_END() // NmhIo
