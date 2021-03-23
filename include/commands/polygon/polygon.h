#ifndef CP_TOOLS_POLYGON_POLYGON_H
#define CP_TOOLS_POLYGON_POLYGON_H

#include <iostream>

#include "api/polygon.h"

using std::ostream;
using std::string;

namespace cptools::commands::polygon {

int run(int argc, char *const argv[], ostream &out, ostream &err);

api::polygon::Credentials get_credentials_from_file(const string &filepath);

string help();
string usage();

} // namespace cptools::commands::polygon

#endif
