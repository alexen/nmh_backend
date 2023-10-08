/// @file request.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <string>
#include <boost/shared_ptr.hpp>


namespace alexen {
namespace nmh {


using RequestPtr = boost::shared_ptr< struct Request >;

struct Request {
     static RequestPtr parse( std::istream& );

     unsigned id;
     std::string module;
     std::string method;
     std::string data;
     std::string timestamp;
};


} // namespace nmh
} // namespace alexen
