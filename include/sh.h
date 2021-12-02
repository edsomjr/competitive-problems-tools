#ifndef CP_TOOLS_SH_H
#define CP_TOOLS_SH_H

#include <string>

#include "types/common.h"

// Functions that emulates shell commands
namespace cptools::sh
{

    using Result = types::common::Result;
    using types::common::make_result;

    struct Info
    {
        int rc;
        double elapsed;
        double memory;
    };

    const Result execute_command(const std::string &command,
                                 int on_error = CP_TOOLS_ERROR_SH_EXEC_ERROR);

    const Result diff_dirs(const std::string &dirA, const std::string &dirB);

    const Result build(const std::string &output, const std::string &src);

    const Result execute_program(const std::string &program, const std::string &args,
                                 const std::string &infile = "",
                                 const std::string &outfile = "/dev/null", int timeout = 3);

    Info profile(const std::string &program, const std::string &args, int timeout = 3,
                 const std::string &infile = "", const std::string &outfile = "/dev/null");
} // namespace cptools::sh

#endif
