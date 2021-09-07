#ifndef CP_TOOLS_TYPES_COMMON_H
#define CP_TOOLS_TYPES_COMMON_H

#include <string>

namespace cptools::types::common {

struct Result {
    bool ok;
    int rc;
    std::string error_message;
};

} // namespace cptools::types::common

#endif