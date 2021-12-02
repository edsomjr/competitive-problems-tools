#ifndef CP_TOOLS_POLYGON_POLYGON_H
#define CP_TOOLS_POLYGON_POLYGON_H

#include <iostream>

#include "api/polygon.h"
#include "types/polygon.h"

namespace cptools::commands::polygon
{

    int run(int argc, char *const argv[]);

    types::polygon::Credentials get_credentials_from_file(const std::string &filepath);

    std::string help();
    std::string usage();

} // namespace cptools::commands::polygon

#endif
