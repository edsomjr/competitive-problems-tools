#include <map>

#include <unistd.h>
#include <getopt.h>

#include "defs.h"
#include "error.h"
#include "cptools.h"
#include "problem.h"


// Raw strings
static const std::string help_message {
R"message(
A set of competitive programming tools for contests preparation.

    Action          Description

    contest         Creates programming contests or extract info from previous contests.
    problem         Format, test and pack competitive programming problems.
    users           Generates a LaTeX file from the problem description. 
)message"
};

static const std::string version_header { NAME " " VERSION "\n" };

static const std::string version_body { 
R"body(License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

Written by Edson Alves.)body"
};


namespace cptools {

    // Global variables
    std::map<std::string, int (*)(int, char *const [], std::ostream&, std::ostream&)> commands {
//        { "users", users::run },
        { "problem", problem::run },
 //       { "gentex", gentex::run },
    };

    static struct option longopts[] = {
       { "help", no_argument, NULL, 'h' },
       { "version", no_argument, NULL, 'v' },
       { 0, 0, 0, 0 }
    };

    // Auxiliary routines
    std::string usage()
    {
        return "Usage: " NAME " [-h] [-v] command";
    }

    std::string help()
    {
        return usage() + help_message;
    }

    std::string version()
    {
        return version_header + version_body;
    }

    // API functions
    int run(int argc, char* const argv[], std::ostream& out, std::ostream& err)
    {
        if (argc >= 2)
        {
            std::string command { argv[1] };

            for (const auto& [cmd, exec] : commands)
                if (cmd == command)
                    return exec(argc, argv, std::cout, std::cerr);

            if (command.front() != '-')
            {
                err << NAME << ": invalid command '" << command << "'\n";
                return CP_TOOLS_ERROR_INVALID_COMMAND;
            }
        }

        int option = -1;

        while ((option = getopt_long(argc, argv, "hv", longopts, NULL)) != -1)
        {
            switch (option) {
            case 'v':
                out << version() << '\n';
                return CP_TOOLS_OK;
            
            case 'h':
                out << help() << '\n';
                return CP_TOOLS_OK;

            default:
                err << help() << '\n';
                return CP_TOOLS_ERROR_INVALID_PARAMETER;
            }
        }

        out << usage() << '\n';
        return CP_TOOLS_OK;
    }
}
