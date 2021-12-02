#ifndef CP_TOOLS_CLEAN_H
#define CP_TOOLS_CLEAN_H

#include <iostream>

namespace cptools::commands::clean
{
    // Main routine
    int run(int argc, char *const argv[]);

    // Auxiliary routines
    std::string help();
    std::string usage();
} // namespace cptools::commands::clean

#endif
