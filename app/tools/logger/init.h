/// @file init.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <iosfwd>

#include <boost/filesystem/path.hpp>
#include <boost/utility/string_view.hpp>


namespace alexen {
namespace tools {
namespace logger {


void initFileLog(
     const boost::filesystem::path& path,
     std::ios_base::openmode mode = std::ios_base::out | std::ios_base::app,
     boost::string_view fmt = "[%TimeStamp%] {%ProcessID%.%ThreadID%} <%Severity%>: %Message%"
);

void initOstreamLog( std::ostream& os, boost::string_view fmt = "%TimeStamp% [%Severity%] %Message%" );


} // namespace logger
} // namespace tools
} // namespace alexen
