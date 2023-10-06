/// @file hex.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <iosfwd>

#include <boost/variant/variant.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/algorithm/hex.hpp>


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


struct HexlifyVisitor : boost::static_visitor<> {
     explicit HexlifyVisitor( std::ostream& os )
          : ostr{ os }
     {}

     struct Rewinder
     {
          explicit Rewinder( std::istream& is )
               : istr{ is }
               , saved{ is.tellg() }
          {}
          ~Rewinder() {
               istr.seekg( saved - istr.tellg(), std::ios_base::cur );
          }

          std::istream& istr;
          const std::istream::pos_type saved;
     };

     void operator()( std::istream& is ) const
     {
          Rewinder _{ is };

          boost::algorithm::hex_lower( std::istreambuf_iterator< char >{ is }, {},
               std::ostreambuf_iterator< char >{ ostr } );
     }

     void operator()( boost::string_view sv ) const
     {
          boost::algorithm::hex_lower( sv.cbegin(), sv.cend(),
               std::ostreambuf_iterator< char >{ ostr } );
     }

     std::ostream& ostr;
};


inline std::ostream& operator<<( std::ostream& os, const Hexlify& h )
{
     boost::apply_visitor( HexlifyVisitor{ os }, h.input );
     return os;
}


} // namespace converter
} // namespace tools
} // namespace alexen
