/// @file module.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/utility/string_view.hpp>


namespace alexen {
namespace handler {


class IHandler
{
public:
     virtual ~IHandler() {}

     virtual boost::string_view name() const noexcept = 0;
     virtual void process( boost::string_view method, std::istream&, std::ostream& ) = 0;
};


using IHandlerPtr = boost::shared_ptr< IHandler >;
using IHandlerMap = std::map< std::string, IHandlerPtr >;


} // namespace handler
} // namespace alexen
