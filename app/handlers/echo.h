/// @file echo.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <handlers/handler.h>


namespace alexen {
namespace handler {


class EchoHandler : public IHandler
{
public:
     boost::string_view name() const noexcept override;
     void process( boost::string_view method, std::istream&, std::ostream& ) override;

private:
     void echo( std::istream&, std::ostream& );
};


} // namespace handler
} // namespace alexen
