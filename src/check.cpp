#include <iostream>

#include <unistd.h>
#include <getopt.h>

#include "sh.h"
#include "task.h"
#include "defs.h"
#include "dirs.h"
#include "check.h"
#include "error.h"
#include "config.h"


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
        auto program { std::string(CP_TOOLS_BUILD_DIR) + "/validator" };

        auto config = cptools::config::read("config.json");
        auto source = cptools::config::get(config, "tools|validator", std::string("ERROR"));

        out << "[validate_tests] source = '" << source << "'\n";

        if (source == "solutions/ERROR")
        {
            err << "[validate_tests] Default solution file not found!\n";
            return CP_TOOLS_ERROR_CHECK_MISSING_VALIDATOR;
        }

        auto rc = cptools::sh::build(program, source);

        if (rc != CP_TOOLS_OK)
        {
            err << "[validate_tests] Can't compile validator '" << source << "'\n";
            return rc;
        }


        auto io_files = task::generate_io_files("all", out, err, false);

        if (io_files.empty())
        {
            err << "[validate_tests] There are no io files to validate!\n";
            return CP_TOOLS_ERROR_CHECK_MISSING_IO_FILES;
        }

        out << "Checking " << io_files.size() << " input files...\n";

        for (auto [input, _] : io_files)
        {
            err << "[check tests] Validating " << input << "...\n";

            auto rc = cptools::sh::process(input, program, "/dev/null");

            if (rc != CP_TOOLS_OK)
            {
                out << "Input file '" << input << "' is invalid!\n";
                return CP_TOOLS_ERROR_CHECK_INVALID_INPUT_FILE;
            }
        }

        out << "Ok!\n";

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
