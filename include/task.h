#ifndef CP_TOOLS_TASK_H
#define CP_TOOLS_TASK_H

#include <iostream>
#include <string>
#include <vector>

using std::ostream;
using std::pair;
using std::string;
using std::vector;

// Problem preparation related tasks
namespace cptools::task {

namespace tools {
constexpr int CHECKER = 1;
constexpr int VALIDATOR = 2;
constexpr int GENERATOR = 4;
constexpr int INTERACTOR = 8;
constexpr int ALL = CHECKER | VALIDATOR | GENERATOR | INTERACTOR;
} // namespace tools

vector<pair<string, string>> generate_io_files(const string &testset,
                                               ostream &out, ostream &err,
                                               bool gen_output = true);

int build_tools(string &error, int tools = tools::ALL,
                const string &where = ".");
int gen_exe(string &error, const string &source, const string &dest,
            const string &where = ".");

} // namespace cptools::task

#endif
