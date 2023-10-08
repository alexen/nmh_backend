/// @file workers.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <iosfwd>

#include <nmh/request.h>
#include <nmh/response.h>

#include <tools/mt/queue.h>

#include <handlers/handler.h>


namespace alexen {
namespace nmh {
namespace mt {


using RequestQueue = tools::mt::BlockingQueue< RequestPtr >;
using ResponseQueue = tools::mt::BlockingQueue< ResponsePtr >;


namespace workers {


void istreamListener( std::istream&, RequestQueue&, ResponseQueue& );
void ostreamWriter( std::ostream&, ResponseQueue& );
void requestProcessor( RequestQueue&, ResponseQueue&, const handler::IHandlerMap& );


} // namespace workers
} // namespace mt
} // namespace nmh
} // namespace alexen
