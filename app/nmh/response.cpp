/// @file response.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <nmh/response.h>

#include <boost/make_shared.hpp>
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


ResponsePtr makeResponse(
     unsigned sourceMessageId
     , const std::string& status
     , const std::string& result
)
{
     auto response = boost::make_shared< Response >();
     response->replyTo = sourceMessageId;
     response->status = status;
     response->result = result;
     response->timestamp = boost::posix_time::second_clock::local_time();
     return response;
}


ResponsePtr makeErrorResponse( unsigned sourceMessageId, const std::string& error )
{
     auto response = boost::make_shared< Response >();
     response->replyTo = sourceMessageId;
     response->status = "error";
     response->error = error;
     response->timestamp = boost::posix_time::second_clock::local_time();
     return response;
}


ResponsePtr makeErrorResponse( const std::string& error )
{
     auto response = boost::make_shared< Response >();
     response->status = "error";
     response->error = error;
     response->timestamp = boost::posix_time::second_clock::local_time();
     return response;
}


} // namespace nmh
} // namespace alexen
