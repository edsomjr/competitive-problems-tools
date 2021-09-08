#ifndef CP_TOOLS_TASK_H
#define CP_TOOLS_TASK_H

#include <iostream>
#include <string>
#include <vector>

#include "config.h"
#include "types/common.h"

// Problem preparation related tasks
namespace cptools::task {

using Result = types::common::Result;
using types::common::make_result;

const Result build_tool(config::tool_type tool_type);

std::vector<std::pair<std::string, std::string>> generate_io_files(const std::string &testset,
                                                                   bool gen_output = true);

const Result gen_exe(const std::string &source, const std::string &dest);

} // namespace cptools::task

#endif
