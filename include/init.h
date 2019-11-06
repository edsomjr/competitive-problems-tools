#ifndef CP_TOOLS_INIT_H
#define CP_TOOLS_INIT_H

#include <iostream>

namespace cptools::init {

    int run(int argc, char * const argv[], std::ostream& out, std::ostream& err);

    std::string help();
    std::string usage();

}

#endif
