#ifndef CP_TOOLS_JUDGE_H
#define CP_TOOLS_JUDGE_H

#include <iostream>

using std::ostream;
using std::string;

namespace cptools::commands::judge {
namespace verdict {
extern const int AC;
extern const int WA;
extern const int PE;
extern const int CE;
extern const int TLE;
extern const int RTE;
extern const int MLE;
extern const int FAIL;
extern const int UNDEF;
} // namespace verdict

// Main routine
int run(int argc, char *const argv[], ostream &out, ostream &err);

// Auxiliary routines
string help();
string usage();

// Judge solution
int judge(const string &solution_path, ostream &out, ostream &err);
} // namespace cptools::commands::judge

#endif
