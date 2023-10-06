/// @file hexlify_test.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#define BOOST_TEST_MODULE tools

#include <sstream>

#include <boost/test/unit_test.hpp>

#include "../converter/hexlify.h"


BOOST_AUTO_TEST_SUITE( Tools )
BOOST_AUTO_TEST_SUITE( Hexlify )

using alexen::tools::converter::Hexlify;

BOOST_AUTO_TEST_CASE( HexlifyMemoryBlock )
{
     const std::string source = "Hello, world!";
     /// echo -n 'Hello, world!' | xxd -p
     const std::string expected = "48656c6c6f2c20776f726c6421";

     std::ostringstream oss;

     BOOST_REQUIRE_NO_THROW( oss << Hexlify{ source } );
     BOOST_TEST( oss.str() == expected );
}
BOOST_AUTO_TEST_CASE( HexlifyIstreamRewind )
{
     const std::string source = "First, second, third!";
     /// echo -n 'First, second, third!' | xxd -p
     const std::string expected = "46697273742c207365636f6e642c20746869726421";

     std::istringstream iss{ source };
     std::ostringstream oss;

     BOOST_REQUIRE_NO_THROW( oss << Hexlify{ iss } );
     BOOST_TEST( oss.str() == expected );

     std::string text;
     BOOST_TEST( !!std::getline( iss, text ) );
     BOOST_TEST( text == source );
}
BOOST_AUTO_TEST_CASE( HexlifyIstreamRewind2 )
{
     const std::string source = "First, second, third!";

     std::istringstream iss{ source };
     /// Откусываем первое слово (до пробела)
     {
          std::string _;
          iss >> _;
     }
     /// echo -n ' second, third!' | xxd -p
     const std::string expected = "207365636f6e642c20746869726421";

     std::ostringstream oss;

     BOOST_REQUIRE_NO_THROW( oss << Hexlify{ iss } );
     BOOST_TEST( oss.str() == expected );

     std::string text;
     BOOST_TEST( !!std::getline( iss, text ) );
     BOOST_TEST( text == " second, third!" );
}
BOOST_AUTO_TEST_SUITE_END() /// Hexlify
BOOST_AUTO_TEST_SUITE_END() /// Tools