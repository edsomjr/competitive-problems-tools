#ifndef CP_TOOLS_SH_H
#define CP_TOOLS_SH_H

#include <string>

// Functions that emulates shell commands
namespace cptools::sh {

    int copy_file(const std::string& dest, const std::string& src);

    int make_dir(const std::string& path);
    int copy_dir(const std::string& dest, const std::string& src);
    int remove_dir(const std::string& path);

    bool compare_dirs(const std::string& dirA, const std::string& dirB);
    bool is_dir(const std::string& path);

    int build(const std::string& output, const std::string& src);
    int process(const std::string& input, const std::string& program, const std::string& output,
        int timeout = 3);
}

#endif
