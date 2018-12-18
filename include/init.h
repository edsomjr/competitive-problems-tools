#ifndef CP_TOOLS_INIT_H
#define CP_TOOLS_INIT_H

extern int init(int argc, char * const argv[]);

#include <iostream>


namespace cptools {

    namespace init {
        // Main routine
        int run(int argc, char * const argv[], std::ostream& out, std::ostream& err);

        // Auxiliary routines
        std::string help();
        std::string usage();
    }

}


#endif
