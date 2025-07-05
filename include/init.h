#ifndef CP_TOOLS_INIT_H
#define CP_TOOLS_INIT_H

#include <iostream>

using std::ostream;
using std::string;

namespace cptools::init {

    int run(int argc, char * const argv[], ostream& out, ostream& err);

    string help();
    string usage();

}

#endif
