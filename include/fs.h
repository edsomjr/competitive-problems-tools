#ifndef CP_TOOLS_FS_H
#define CP_TOOLS_FS_H

#include <string>

using std::string;
using std::ostream;

namespace cptools::fs {
    string get_home_dir();
    string get_default_config_path();
    bool file_exists(const string &path);
}

#endif