#include <iostream>
#include <filesystem>

#include <unistd.h>
#include <getopt.h>

#include "defs.h"
#include "init.h"
#include "error.h"


// Raw strings
static const std::string help_message {
R"message(
Initialize an empty directory with template files. If a file already exists, it is not overridden.
)message"
};

namespace cptools::init {

    // Global variables
    static struct option longopts[] = {
       { "help", no_argument, NULL, 'h' },
       { 0, 0, 0, 0 }
    };

    // Auxiliary routines
    std::string usage()
    {
        return "Usage: " NAME " init [-h]";
    }

    std::string help()
    {
        return usage() + help_message;
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

    // API functions
    int run(int argc, char* const argv[], std::ostream& out, std::ostream& err)
    {
        int option = -1;

        while ((option = getopt_long(argc, argv, "h", longopts, NULL)) != -1)
        {
            switch (option) {
            case 'h':
                out << help() << '\n';
                return 0;

            default:
                err << help() << '\n';
                return CP_TOOLS_ERROR_INIT_INVALID_OPTION;
            }
        }

        return copy_template_files();
    }
}
