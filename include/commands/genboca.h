#ifndef CP_TOOLS_GENBOCA_H
#define CP_TOOLS_GENBOCA_H

#include <iostream>

namespace cptools::commands::genboca
{

    // Main routine
    int run(int argc, char *const argv[]);

    // Auxiliary routines
    std::string help();
    std::string usage();

} // namespace cptools::commands::genboca

#endif
