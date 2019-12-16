#ifndef CP_TOOLS_PROBLEM_H
#define CP_TOOLS_PROBLEM_H

#include <iostream>


namespace cptools::problem {

    int run(int argc, char * const argv[], std::ostream& out, std::ostream& err);

    std::string help();
    std::string usage();

}

#endif
