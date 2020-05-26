#ifndef CP_TOOLS_JUDGE_H
#define CP_TOOLS_JUDGE_H

#include <iostream>

#define AC      0
#define WA      1
#define TLE     2
#define PE      3
#define RTE     4
#define FL      5
#define MLE     6
#define CE      7
#define UNDEF   8

namespace cptools::judge
{
    // Main routine
    int run(int argc, char * const argv[], std::ostream& out, std::ostream& err);

    // Auxiliary routines
    std::string help();
    std::string usage();

    // Judge solution
    int judge(const std::string& solution_path, std::ostream& out, std::ostream& err);
}

#endif
