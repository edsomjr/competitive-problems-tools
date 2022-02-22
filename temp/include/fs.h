#ifndef CP_TOOLS_FS_H
#define CP_TOOLS_FS_H

#include <string>

namespace cptools::fs {

    void copy(const std::string &src, const std::string &dst, bool overwrite = false);

}

#endif
