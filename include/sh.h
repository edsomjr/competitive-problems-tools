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

    int copy_file(const string& dest, const string& src);
    int remove_file(const string& path, string& error);

    Result make_dir(const string& path);
    Result copy_dir(const string& dest, const string& src);
    Result remove_dir(const string& path);
    Result same_dirs(const string& dirA, const string& dirB);

    bool is_dir(const string& path, string& error);
    bool is_file(const string& path);

    int build(const string& output, const string& src);
    int process(const string& input, const string& program, const string& output, int timeout = 3);
    int exec(const string& program, const string& args, const string& output, int timeout = 3);

    Result execute(const string& program, const string& args, const string& infile = "", 
        const string& outfile = "/dev/null", int timeout = 3);
   
    Info profile(const string& program, const string& args, int timeout = 3, 
        const string& infile = "", const string& outfile = "/dev/null");
}

#endif
