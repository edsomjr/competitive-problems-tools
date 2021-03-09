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

Result copy_dir(const string &dest, const string &src);
Result remove_dir(const string &path);
Result same_dirs(const string &dirA, const string &dirB);

Result is_dir(const string &path);
Result is_file(const string &path);

Result build(const string &output, const string &src);

Result execute(const string &program, const string &args,
               const string &infile = "", const string &outfile = "/dev/null",
               int timeout = 3);

Info profile(const string &program, const string &args, int timeout = 3,
             const string &infile = "", const string &outfile = "/dev/null");
} // namespace cptools::sh

#endif
