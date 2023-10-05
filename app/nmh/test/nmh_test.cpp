/// @file nmh_test.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#define BOOST_TEST_MODULE nmh

#include <sstream>
#include <boost/test/unit_test.hpp>

#include "../nmh.h"

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
BOOST_AUTO_TEST_SUITE_END() // Read
BOOST_AUTO_TEST_SUITE_END() // NmhIo
