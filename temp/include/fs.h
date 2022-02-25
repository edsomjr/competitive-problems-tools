#ifndef CP_TOOLS_FS_H
#define CP_TOOLS_FS_H

#include <string>

namespace cptools::fs {

    bool exists(const std::string &path);
    bool is_directory(const std::string &path);

    void create_directory(const std::string &path);

    void copy(const std::string &src, const std::string &dst, bool overwrite = false);
    void copy_recursive(const std::string &src, const std::string &dst, bool overwrite = false);

    std::string basename(const std::string& path);
}

#endif
