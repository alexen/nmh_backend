/// @file nmh.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <iosfwd>
#include <boost/utility/string_view.hpp>


namespace alexen {
namespace nmh {
namespace io {


bool read( std::istream& is, std::ostream& os );
void write( boost::string_view data, std::ostream& os );


} // namespace io
} // namespace nmh
} // namespace alexen
