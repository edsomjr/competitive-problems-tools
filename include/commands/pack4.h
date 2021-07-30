#ifndef CP_TOOLS_PACK4_H
#define CP_TOOLS_PACK4_H

#include <iostream>

namespace cptools::commands::pack4 {
// Main routine
int run(int argc, char *const argv[], std::ostream &out, std::ostream &err);

// Auxiliary routines
std::string help();
std::string usage();

} // namespace cptools::commands::pack4

#endif