/// @file hex.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <iosfwd>

#include <boost/variant/variant.hpp>
#include <boost/utility/string_view.hpp>


namespace alexen {
namespace tools {
namespace converter {


struct Hexlify {
     using InputVariant = boost::variant< boost::string_view, std::istream& >;

     explicit Hexlify( InputVariant&& v )
          : input{ v }
     {}

     InputVariant input;
};


std::ostream& operator<<( std::ostream& os, const Hexlify& h );


} // namespace converter
} // namespace tools
} // namespace alexen
