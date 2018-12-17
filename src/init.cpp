#include <iostream>
#include <filesystem>

#include <unistd.h>
#include <getopt.h>

#include "defs.h"
#include "init.h"
#include "error.h"


static struct option longopts[] = {
   { "help", no_argument, NULL, 'h' },
   { 0, 0, 0, 0 }
};


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
    static const std::string templates_dir { "/usr/local/lib/" NAME "/templates/" };

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

    while ((option = getopt_long(argc, argv, "h", longopts, NULL)) != -1)
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
