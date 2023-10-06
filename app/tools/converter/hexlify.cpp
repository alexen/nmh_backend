/// @file hexlify.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include "hexlify.h"

#include <boost/algorithm/hex.hpp>


namespace alexen {
namespace tools {
namespace converter {


namespace {
namespace impl {


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


} // namespace impl
} // namespace {unnamed}


std::ostream& operator<<( std::ostream& os, const Hexlify& h )
{
     boost::apply_visitor( impl::HexlifyVisitor{ os }, h.input );
     return os;
}


} // namespace converter
} // namespace tools
} // namespace alexen
