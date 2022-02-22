#ifndef CP_TOOLS_MESSAGE_H
#define CP_TOOLS_MESSAGE_H

#include <string>

namespace cptools::message
{
    std::string ok(const std::string& text);

    std::string info(const std::string& text);
    std::string error(const std::string& text);
}

#endif
