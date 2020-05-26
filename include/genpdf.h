#ifndef CP_TOOLS_GENPDF_H
#define CP_TOOLS_GENPDF_H

#include <iostream>

namespace cptools::genpdf {

    // Main routine
    int run(int argc, char * const argv[], std::ostream& out, std::ostream& err);

}

#endif
