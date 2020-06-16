#ifndef CP_TOOLS_CLEAN_H
#define CP_TOOLS_CLEAN_H

#include <iostream>

using std::string;
using std::ostream;

namespace cptools::clean
{
    // Main routine
    int run(int argc, char * const argv[], ostream& out, ostream& err);

    // Auxiliary routines
    string help();
    string usage();
}

#endif
