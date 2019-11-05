#ifndef CP_TOOLS_H
#define CP_TOOLS_H

#include <iostream>


namespace cptools {

    int run(int argc, char * const argv[], std::ostream& out, std::ostream& err);

    std::string help();
    std::string usage();
    std::string version();

}

#endif
