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

    void create_directory(const std::string &path)
    {
        if (exists(path))
            return;

        try {
            std::filesystem::create_directories(path);
        } catch (const std::filesystem::filesystem_error &err)
        {
            std::cout << cptools::message::error(err.what()) << '\n';
            exit(CP_TOOLS_ERROR_FS_CREATE_DIRECTORY);
        }
    }

    bool exists(const std::string &path)
    {
        bool ok = false;

        try {
            ok = std::filesystem::exists(path);
        } catch (const std::filesystem::filesystem_error &err)
        {
            std::cout << cptools::message::error(err.what()) << '\n';
            exit(CP_TOOLS_ERROR_FS_EXISTS);
        }

        return ok;
    }

    void copy_recursive(const std::string &src, const std::string &dst, bool overwrite)
    {
        for (auto file : std::filesystem::recursive_directory_iterator(src))
        {
            auto relative_path = std::filesystem::relative(file.path(), src);
            auto path = dst + "/" + relative_path.string();

            if (is_directory(path))
                create_directory(path);
            else if (not exists(path))
                copy(file.path().string(), path, overwrite);
        }
    }

    std::string basename(const std::string& p)
    {
        return std::filesystem::path(p).filename();
    }

    bool is_directory(const std::string &path)
    {
        bool ok = false;

        try {
            ok = std::filesystem::is_directory(path);
        } catch (const std::filesystem::filesystem_error &err)
        {
            std::cout << cptools::message::error(err.what()) << '\n';
            exit(CP_TOOLS_ERROR_FS_IS_DIRECTORY);
        }

        return ok;
    }

    void remove(const std::string &path)
    {
        if (not fs::exists(path))
            return;

        try {
            std::filesystem::remove_all(path);
        } catch (const std::filesystem::filesystem_error &err)
        {
            std::cout << cptools::message::error(err.what()) << '\n';
            exit(CP_TOOLS_ERROR_FS_REMOVE);
        }
    }
}
