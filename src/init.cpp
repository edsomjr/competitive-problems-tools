#include <iostream>
#include <unistd.h>
#include <filesystem>

#include "defs.h"
#include "init.h"


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
        if (std::filesystem::is_directory(p.symlink_status()))
        {
            auto dir = p.path().filename();

            std::filesystem::create_directory(dir);
            std::filesystem::copy(p.path(), dir, std::filesystem::copy_options::recursive);
            std::cout << p.path() << "é diretório\n";
        } else
        {
            std::cout << p.path() << "é arquivo\n";
            std::filesystem::copy(p.path(), ".");
        }
    }

    return 0;
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
            return -1;
        }
    }

    return copy_template_files();
}
