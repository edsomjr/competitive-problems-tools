#include <iostream>

#include <unistd.h>
#include <getopt.h>

#include "sh.h"
#include "task.h"
#include "defs.h"
#include "dirs.h"
#include "check.h"
#include "error.h"


// Raw strings
static const std::string help_message {
R"message(
Verify problem files and tools. 

    Option          Description

    -a              Validate everything (default option).
    --all

    -h              Generate this help message.
    --help

    -s              Validate the solutions.
    --solutions

    -t              Validate the tests.
    --tests
)message"
};

namespace cptools::check {

    // Global variables
    static struct option longopts[] = {
       { "all", no_argument, NULL, 'a' },
       { "help", no_argument, NULL, 'h' },
       { "solutions", no_argument, NULL, 's' },
       { "tests", no_argument, NULL, 't' },
       { 0, 0, 0, 0 }
    };

    // Auxiliary routines
    std::string usage()
    {
        return "Usage: " NAME " check [-h] [-a] [-s] [-t]";
    }

    std::string help()
    {
        return usage() + help_message;
    }

    int validate_tests(std::ostream& out, std::ostream& err)
    {
        auto io_files = task::generate_io_files("all", out, err);

        if (io_files.empty())
        {
            err << "[validate_tests] There are no io files to validate!\n";
            return CP_TOOLS_ERROR_CHECK_MISSING_IO_FILES;
        }

        for (auto [infile, outfile] : io_files)
            out << "[validate_tests] " << infile << " <-> " << outfile << '\n';

        return CP_TOOLS_OK;
    }

    // API functions
    int run(int argc, char* const argv[], std::ostream& out, std::ostream& err)
    {
        int option = -1;

        while ((option = getopt_long(argc, argv, "ahst", longopts, NULL)) != -1)
        {
            switch (option) {
            case 'h':
                out << help() << '\n';
                return 0;

            case 't':
                return validate_tests(out, err);

            default:
                err << help() << '\n';
                return CP_TOOLS_ERROR_CLEAN_INVALID_OPTION;
            }
        }

        return CP_TOOLS_OK;
    }
}
