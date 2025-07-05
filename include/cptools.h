#ifndef CP_TOOLS_H
#define CP_TOOLS_H

#include <iostream>

using std::ostream;
using std::string;

namespace cptools {

    int run(int argc, char * const argv[], ostream& out, ostream& err);

    string help();
    string usage();
    string version();

}

#endif
