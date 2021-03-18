#ifndef CP_TOOLS_FS_H
#define CP_TOOLS_FS_H

#include <string>

using std::ostream;
using std::string;

namespace cptools::fs {

string get_home_dir();
string get_default_config_path();

} // namespace cptools::fs

#endif