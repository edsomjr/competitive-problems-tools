#include <iostream>

#include "sh.h"
#include "error.h"


namespace cptools::sh {

    int make_dir(const std::string& path)
    {
        std::string command { "mkdir -p " + path };

        auto rc = std::system(command.c_str());

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_CREATE_DIRECTORY;
    }

    int copy_dir(const std::string& dest, const std::string& src)
    {
        std::string command { "cp -r " + src + "/* " + dest };

        auto rc = std::system(command.c_str());

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_COPY_DIRECTORY;
    }

    int remove_dir(const std::string& path)
    {
        std::string command { "rm -rf " + path };

        auto rc = std::system(command.c_str());

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_REMOVE_DIRECTORY;
    }

    bool compare_dirs(const std::string& dirA, const std::string& dirB)
    {
        std::string command { "diff -r " + dirA + " " + dirB };

        auto rc = std::system(command.c_str());

        return rc == 0;
    }

    bool is_dir(const std::string& path)
    {
        std::string command { "test -d " + path };

        auto rc = std::system(command.c_str());

        return rc == 0;
    }
}
