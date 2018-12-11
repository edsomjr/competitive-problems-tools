#include <iostream>
#include <unistd.h>
#include <filesystem>

#include "defs.h"
#include "init.h"
#include "error.h"


static std::string usage()
{
    return "Usage: " NAME " init [-h]";
}

static std::string help()
{
    const std::string message {
R"message(
Initialize an empty directory with template files. If a file already exists, it is not overridden.
)message" };

    return usage() + message;
}

int copy_template_files()
{
    static const std::string templates_dir { "/usr/local/lib/cp-tools/templates/" };

    for (const auto& p : std::filesystem::directory_iterator(templates_dir))
    {
        auto name = p.path().filename();

        try {
            if (std::filesystem::exists(name))
                continue;

            if (std::filesystem::is_directory(p.symlink_status()))
            {
                std::filesystem::create_directory(name);
                std::filesystem::copy(p.path(), name, std::filesystem::copy_options::recursive);
            } else
                std::filesystem::copy(p.path(), ".");
        } catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return CP_TOOLS_ERROR_INIT_COPY_FILES;
        }
    }

    return CP_TOOLS_OK;
}

int init(int argc, char * const argv[])
{
    int option = -1;

    while ((option = getopt(argc, argv, "h")) != -1)
    {
        switch (option) {
        case 'h':
            std::cout << help() << '\n';
            return 0;

        default:
            std::cout << help() << '\n';
            return CP_TOOLS_ERROR_INIT_INVALID_OPTION;
        }
    }

    return copy_template_files();
}
