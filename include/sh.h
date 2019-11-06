#ifndef CP_TOOLS_SH_H
#define CP_TOOLS_SH_H

#include <string>

// Functions that emulates shell commands
namespace cptools::sh {

    int make_dir(const std::string& path);
    int copy_dir(const std::string& dest, const std::string& src);
    int remove_dir(const std::string& path);
    bool compare_dirs(const std::string& dirA, const std::string& dirB);

}

#endif
