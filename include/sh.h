#ifndef CP_TOOLS_SH_H
#define CP_TOOLS_SH_H

#include <string>

// Functions that emulates shell commands
namespace cptools::sh {

struct Result {
    int rc;
    std::string output;
};

struct Info {
    int rc;
    double elapsed;
    double memory;
};

Result diff_dirs(const std::string &dirA, const std::string &dirB);

Result build(const std::string &output, const std::string &src);

Result execute(const std::string &program, const std::string &args, const std::string &infile = "",
               const std::string &outfile = "/dev/null", int timeout = 3);

Info profile(const std::string &program, const std::string &args, int timeout = 3,
             const std::string &infile = "", const std::string &outfile = "/dev/null");
} // namespace cptools::sh

#endif
