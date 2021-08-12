#include <cstdlib>
#include <getopt.h>
#include <sstream>

#include "catch.hpp"
#include "commands/clean.h"
#include "commands/cptools.h"
#include "dirs.h"
#include "error.h"
#include "fs.h"
#include "sh.h"

SCENARIO("Command clean", "[clean]") {
    GIVEN("An execution of the command clean with options") {
        WHEN("There is no option") {
            int argc = 2;
            char *const argv[]{(char *)"cp-tools", (char *)"clean"};

            THEN("The the auto-generated files in current directory is deleted") {
                std::ostringstream out, err;
                REQUIRE(cptools::commands::run(argc, argv, out, err) == CP_TOOLS_OK);
            }
        }

        WHEN("The option -w is used") {
            int argc = 4;
            char *const argv[]{(char *)"cp-tools", (char *)"clean", (char *)"-w",
                               (char *)CP_TOOLS_TEMP_DIR};

            auto fs_res = cptools::fs::create_directory(CP_TOOLS_TEMP_DIR);

            REQUIRE(fs_res.ok);

            // getopt library must be reseted between tests
            optind = 1;

            THEN("The subdirectory with the auto-generated files is deleted") {
                std::ostringstream out, err;
                REQUIRE(cptools::commands::run(argc, argv, out, err) == CP_TOOLS_OK);
            }
        }

        WHEN("The option -h is used") {
            int argc = 3;
            char *const argv[]{(char *)"cp-tools", (char *)"clean", (char *)"-h"};

            // getopt library must be reseted between tests
            optind = 1;

            THEN("The output is the help message") {
                std::ostringstream out, err;

                auto rc = cptools::commands::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_OK);
                REQUIRE(err.str().empty());
                REQUIRE(out.str() == (cptools::commands::clean::help() + '\n'));
            }
        }

        WHEN("The an invalid option is passed") {
            int argc = 3;
            char *const argv[]{(char *)"cp-tools", (char *)"clean", (char *)"-x"};

            optind = 1;
            opterr = 0;

            THEN("The error output is the help message") {
                std::ostringstream out, err;

                auto rc = cptools::commands::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_ERROR_CLEAN_INVALID_OPTION);
                REQUIRE(out.str().empty());
                REQUIRE(err.str() == (cptools::commands::clean::help() + '\n'));
            }
        }
    }
}
