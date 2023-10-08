/// @file response.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <nmh/response.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/date_time/posix_time/time_formatters.hpp>


namespace alexen {
namespace nmh {


namespace {
namespace impl {
namespace json {


template< typename T >
void put( boost::property_tree::ptree& ptree, const std::string& key, const T& value )
{
     ptree.put( key, value );
}


template< typename T >
void put( boost::property_tree::ptree& ptree, const std::string& key, const boost::optional< T >& value )
{
     if( value )
     {
          ptree.put( key, value );
     }
}


void put( boost::property_tree::ptree& ptree, const std::string& key, const boost::posix_time::ptime& value )
{
     ptree.put( key, boost::posix_time::to_iso_extended_string( value ) );
}


} // namespace json
} // namespace impl
} // namespace {unnamed}



void Response::serialize( std::ostream& os )
{
     boost::property_tree::ptree root;

     impl::json::put( root, "status", status );
     impl::json::put( root, "replyTo", replyTo );
     impl::json::put( root, "result", result );
     impl::json::put( root, "error", error );
     impl::json::put( root, "timestamp", timestamp );

     boost::property_tree::write_json( os, root );
}


} // namespace nmh
} // namespace alexen
