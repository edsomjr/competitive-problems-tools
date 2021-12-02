#ifndef CP_TOOLS_CHECK_H
#define CP_TOOLS_CHECK_H

#include <iostream>

namespace cptools::commands::check
{
    // Main routine
    int run(int argc, char *const argv[]);

    // Auxiliary routines
    std::string help();
    std::string usage();

} // namespace cptools::commands::check

#endif
