/// @file init.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include "init.h"

#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/keywords/severity.hpp>


BOOST_LOG_ATTRIBUTE_KEYWORD( Severity, "Severity", boost::log::trivial::severity_level )


namespace alexen {
namespace tools {
namespace logger {


void initFileLog(
     const boost::filesystem::path& path,
     const boost::log::trivial::severity_level minLevel,
     const std::ios_base::openmode mode, boost::string_view fmt
)
{
     boost::log::add_common_attributes();
     boost::log::add_file_log( path,
          boost::log::keywords::format = fmt,
          boost::log::keywords::auto_flush = true,
          boost::log::keywords::open_mode = mode,
          boost::log::keywords::filter = Severity >= minLevel
     );
}

void initOstreamLog(
     std::ostream& os,
     const boost::log::trivial::severity_level minLevel,
     boost::string_view fmt
)
{
     boost::log::add_common_attributes();
     boost::log::add_console_log(
          os,
          boost::log::keywords::format = fmt,
          boost::log::keywords::filter = Severity >= minLevel
     );
}


} // namespace logger
} // namespace tools
} // namespace alexen
