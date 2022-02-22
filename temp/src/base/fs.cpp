#include <iostream>
#include <filesystem>

#include "fs.h"
#include "error.h"
#include "message.h"

namespace cptools::fs {

    void copy(const std::string &src, const std::string &dst, bool overwrite)
    {
        std::filesystem::copy_options options = std::filesystem::copy_options::recursive;

        if (overwrite)
            options |= std::filesystem::copy_options::overwrite_existing;

        try {
            std::filesystem::copy(src, dst, options);
        } catch (const std::filesystem::filesystem_error &err)
        {
            std::cout << cptools::message::error(err.what()) << '\n';
            exit(CP_TOOLS_ERROR_FS_COPY);
        }
    }

}
