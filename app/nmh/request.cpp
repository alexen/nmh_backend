/// @file request.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <nmh/request.h>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/make_shared.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>


namespace alexen {
namespace nmh {


RequestPtr Request::parse( std::istream& is )
{
     boost::property_tree::ptree root;
     boost::property_tree::read_json( is, root );

     auto request = boost::make_shared< Request >();
     request->id = root.get< int >( "id" );
     request->module = root.get< std::string >( "module" );
     request->method = root.get< std::string >( "method" );
     request->data = root.get< std::string >( "data" );
     request->timestamp = root.get< std::string >( "timestamp" );
     return request;
}


RequestPtr Request::parse( boost::string_view sv )
{
     boost::iostreams::stream< boost::iostreams::array_source > is{
          boost::iostreams::array_source{ sv.data(), sv.size() }
     };
     return Request::parse( is );
}


} // namespace nmh
} // namespace alexen
