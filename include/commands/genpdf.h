#ifndef CP_TOOLS_GENPDF_H
#define CP_TOOLS_GENPDF_H

#include <iostream>

namespace cptools::commands::genpdf {

// Main routine
int run(int argc, char *const argv[], std::ostream &out, std::ostream &err);

} // namespace cptools::commands::genpdf

#endif
