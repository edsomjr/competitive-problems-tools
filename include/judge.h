#ifndef CP_TOOLS_JUDGE_H
#define CP_TOOLS_JUDGE_H

#include <iostream>

using std::string;
using std::ostream;

namespace cptools::judge
{
    namespace verdict {
        extern int AC;
        extern int WA;
        extern int PE;
        extern int CE;
        extern int TLE;
        extern int RTE;
        extern int MLE;
        extern int FAIL;
        extern int UNDEF;
    }

    // Main routine
    int run(int argc, char * const argv[], ostream& out, ostream& err);

    // Auxiliary routines
    string help();
    string usage();

    // Judge solution
    int judge(const string& solution_path, ostream& out, ostream& err);
}

#endif
