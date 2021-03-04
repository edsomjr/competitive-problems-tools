#ifndef CP_TOOLS_CLEAN_H
#define CP_TOOLS_CLEAN_H

#include <iostream>

using std::ostream;
using std::string;

namespace cptools::commands::clean {
// Main routine
int run(int argc, char *const argv[], ostream &out, ostream &err);

// Auxiliary routines
string help();
string usage();
} // namespace cptools::commands::clean

#endif
