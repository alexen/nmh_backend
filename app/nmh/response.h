/// @file request.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>
#include <boost/date_time/posix_time/ptime.hpp>


namespace alexen {
namespace nmh {


using ResponsePtr = boost::shared_ptr< struct Response >;

struct Response {
     using Details = std::map< std::string, std::string >;

     void serialize( std::ostream& );

     std::string status;
     boost::optional< unsigned > replyTo;
     boost::optional< std::string > result;
     boost::optional< std::string > error;
     boost::optional< Details > details;
     boost::posix_time::ptime timestamp;
};


ResponsePtr makeResponse(
     unsigned sourceMessageId
     , const std::string& status
     , const std::string& result
);
ResponsePtr makeErrorResponse( unsigned sourceMessageId, const std::string& error );
ResponsePtr makeErrorResponse( const std::string& error, Response::Details&& details = {} );


} // namespace nmh
} // namespace alexen
