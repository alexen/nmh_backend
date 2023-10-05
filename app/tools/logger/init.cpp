/// @file init.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include "init.h"

#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>


namespace alexen {
namespace tools {
namespace logger {


void initFileLog( const boost::filesystem::path& path, const std::ios_base::openmode mode, boost::string_view fmt )
{
     boost::log::add_common_attributes();
     boost::log::add_file_log( path,
          boost::log::keywords::format = fmt,
          boost::log::keywords::auto_flush = true,
          boost::log::keywords::open_mode = mode
     );
}

void initOstreamLog( std::ostream& os, boost::string_view fmt )
{
     boost::log::add_common_attributes();
     boost::log::add_console_log( os, boost::log::keywords::format = fmt );
}


} // namespace logger
} // namespace tools
} // namespace alexen
