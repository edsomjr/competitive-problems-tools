#include <iostream>
#include <cstdlib>
#include <sstream>
#include <getopt.h>

#include "catch.hpp"

#include "sh.h"
#include "dirs.h"
#include "init.h"
#include "error.h"

using namespace std;

SCENARIO("Command init", "[init]")
{
    GIVEN("An execution of the command init with options")
    {
        WHEN("The option -o is used")
        {
            int argc = 4;
            char * const argv[] { (char *) "cp-tools", (char *) "init", (char *) "-o", 
                (char *) CP_TOOLS_TEMP_DIR };

            THEN("The output directory is initialized with the template files")
            {
                string error;
                REQUIRE(cptools::sh::remove_dir(CP_TOOLS_TEMP_DIR, error) >= 0);
                REQUIRE(error.empty());

                ostringstream out, err;
                REQUIRE(cptools::init::run(argc, argv, out, err) == CP_TOOLS_OK);
                REQUIRE(err.str().empty());

                REQUIRE(cptools::sh::compare_dirs(CP_TOOLS_TEMP_DIR, CP_TOOLS_TEMPLATES_DIR, error));
                REQUIRE(error.empty());
            }
        }

        WHEN("The option -h is used")
        {
            int argc = 3;
            char * const argv[] { (char *) "cp-tools", (char *) "init", (char *) "-h" };

            // getopt library must be reseted between tests
            optind = 1;

            THEN("The output is the help message")
            {
                ostringstream out, err;

                auto rc = cptools::init::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_OK);
                REQUIRE(err.str().empty());
                REQUIRE(out.str() == (cptools::init::help() + '\n'));
            }
        }

        WHEN("The an invalid option is passed")
        {
            int argc = 3;
            char * const argv[] { (char *) "cp-tools", (char *) "init", (char *) "-i" };

            // getopt library must be reseted between tests
            optind = 1;
            opterr = 0;

            THEN("The error output is the help message")
            {
                ostringstream out, err;

                auto rc = cptools::init::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_ERROR_INIT_INVALID_OPTION);
                REQUIRE(out.str().empty());
                REQUIRE(err.str() == (cptools::init::help() + '\n'));
            }
        }
    }
}
