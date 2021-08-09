#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <sstream>

#include "catch.hpp"
#include "commands/init.h"
#include "dirs.h"
#include "error.h"
#include "fs.h"
#include "sh.h"

SCENARIO("Command init", "[init]") {
    GIVEN("An execution of the command init with options") {
        WHEN("The option -o is used") {
            int argc = 4;
            char *const argv[]{(char *)"cp-tools", (char *)"init", (char *)"-o",
                               (char *)CP_TOOLS_TEMP_DIR};

            THEN("The output directory is initialized with the template files") {
                auto res = cptools::fs::remove(CP_TOOLS_TEMP_DIR);
                REQUIRE(res.rc == CP_TOOLS_OK);

                std::ostringstream out, err;
                REQUIRE(cptools::commands::init::run(argc, argv, out, err) == CP_TOOLS_OK);
                REQUIRE(err.str().empty());

                auto res_same = cptools::sh::diff_dirs(CP_TOOLS_TEMP_DIR, CP_TOOLS_PROBLEM_TEMPLATE_DIR);
                REQUIRE(res_same.rc == CP_TOOLS_TRUE);
                REQUIRE(res_same.output.empty());
            }
        }

        WHEN("The option -h is used") {
            int argc = 3;
            char *const argv[]{(char *)"cp-tools", (char *)"init", (char *)"-h"};

            // getopt library must be reseted between tests
            optind = 1;

            THEN("The output is the help message") {
                std::ostringstream out, err;

                auto rc = cptools::commands::init::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_OK);
                REQUIRE(err.str().empty());
                REQUIRE(out.str() == (cptools::commands::init::help() + '\n'));
            }
        }

        WHEN("The an invalid option is passed") {
            int argc = 3;
            char *const argv[]{(char *)"cp-tools", (char *)"init", (char *)"-i"};

            // getopt library must be reseted between tests
            optind = 1;
            opterr = 0;

            THEN("The error output is the help message") {
                std::ostringstream out, err;

                auto rc = cptools::commands::init::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_ERROR_INIT_INVALID_OPTION);
                REQUIRE(out.str().empty());
                REQUIRE(err.str() == (cptools::commands::init::help() + '\n'));
            }
        }
    }
}
