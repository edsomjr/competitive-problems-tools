#ifndef CP_TOOLS_TASK_H
#define CP_TOOLS_TASK_H

#include <iostream>
#include <string>
#include <vector>

// Problem preparation related tasks
namespace cptools::task {

namespace tools {
constexpr int CHECKER = 1;
constexpr int VALIDATOR = 2;
constexpr int GENERATOR = 4;
constexpr int INTERACTOR = 8;
constexpr int ALL = CHECKER | VALIDATOR | GENERATOR | INTERACTOR;
} // namespace tools

std::vector<std::pair<std::string, std::string>> generate_io_files(const std::string &testset,
                                                                   bool gen_output = true);

int build_tools(int tools = tools::ALL, const std::string &where = ".");
int gen_exe(const std::string &source, const std::string &dest, const std::string &where = ".");

} // namespace cptools::task

#endif
