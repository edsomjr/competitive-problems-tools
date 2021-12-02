#ifndef CP_TOOLS_TYPES_COMMON_H
#define CP_TOOLS_TYPES_COMMON_H

#include <string>

#include "error.h"

namespace cptools::types::common
{

    struct Result
    {
        bool ok;
        int rc;
        std::string output;
        std::string error_message;
    };

    inline const Result
    make_result(bool res, int rc = CP_TOOLS_OK, const std::string &output = "",
                const std::string &err_msg = "")
    {
        return Result{ res, rc, output, err_msg };
    }

} // namespace cptools::types::common

#endif