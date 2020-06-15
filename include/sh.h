#ifndef CP_TOOLS_SH_H
#define CP_TOOLS_SH_H

#include <string>

using std::string;

// Functions that emulates shell commands
namespace cptools::sh {

    int copy_file(const string& dest, const string& src);

    int make_dir(const string& path);
    int copy_dir(const string& dest, const string& src);
    int remove_dir(const string& path);
    int remove_file(const string& path);

    bool compare_dirs(const string& dirA, const string& dirB);
    bool is_dir(const string& path);
    bool is_file(const string& path);

    int build(const string& output, const string& src);
    int process(const string& input, const string& program, const string& output, int timeout = 3);
    int exec(const string& program, const string& args, const string& output, int timeout = 3);
}

#endif
