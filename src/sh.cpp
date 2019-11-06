#include <filesystem>
#include <iostream>

#include "sh.h"
#include "error.h"


namespace cptools::sh {

    int make_dir(const std::string& path)
    {
        if (std::filesystem::is_directory(path))
            return CP_TOOLS_OK;

        std::error_code ec;
        auto rc = std::filesystem::create_directory(path, ec);

        if (!rc or ec)
            return CP_TOOLS_ERROR_SH_CREATE_DIRECTORY;
    
        return CP_TOOLS_OK;
    }

    int copy_dir(const std::string& dest, const std::string& src)
    {
        for (const auto& p : std::filesystem::directory_iterator(src))
        {
            std::filesystem::path name = std::filesystem::relative(dest / p.path().filename());

            try {
                if (std::filesystem::exists(name))
                    continue;

                if (std::filesystem::is_directory(p.symlink_status()))
                {
                    std::filesystem::create_directory(name);
                    std::filesystem::copy(p.path(), name, std::filesystem::copy_options::recursive);
                } else
                    std::filesystem::copy(p.path(), name);
            } catch (const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                return CP_TOOLS_ERROR_SH_COPY_FILES;
            }
        }

        return CP_TOOLS_OK;
    }

    int remove_dir(const std::string& path)
    {
        try {
            std::filesystem::remove_all(path);
        } catch (const std::exception& e)
        {
            return CP_TOOLS_ERROR_SH_REMOVE_DIRECTORY;
        }

        return CP_TOOLS_OK;
    }

    bool compare_dirs(const std::string& dirA, const std::string& dirB)
    {
        auto command { "diff -r " + dirA + " " + dirB };

        return std::system(command.c_str()) == 0;
    }
}
