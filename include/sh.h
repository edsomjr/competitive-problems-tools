#ifndef CP_TOOLS_SH_H
#define CP_TOOLS_SH_H

#include <string>

using std::string;

// Functions that emulates shell commands
namespace cptools::sh {

struct Result {
    int rc;
    string output;
};

struct Info {
    int rc;
    double elapsed;
    double memory;
};

Result diff_dirs(const string &dirA, const string &dirB);

Result build(const string &output, const string &src);

Result execute(const string &program, const string &args, const string &infile = "",
               const string &outfile = "/dev/null", int timeout = 3);

Info profile(const string &program, const string &args, int timeout = 3, const string &infile = "",
             const string &outfile = "/dev/null");
} // namespace cptools::sh

#endif
