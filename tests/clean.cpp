#include <iostream>
#include <cstdlib>
#include <sstream>
#include <getopt.h>

#include "catch.hpp"

#include "sh.h"
#include "dirs.h"
#include "clean.h"
#include "error.h"

using std::string;
using std::ostringstream;

SCENARIO("Command clean", "[clean]")
{
    GIVEN("An execution of the command clean with options")
    {
        WHEN("There is no option")
        {
            int argc = 2;
            char * const argv[] { (char *) "cp-tools", (char *) "clean" };

            THEN("The the auto-generated files in current directory is deleted")
            {
                ostringstream out, err;
                REQUIRE(cptools::clean::run(argc, argv, out, err) == CP_TOOLS_OK);
            }
        }


        WHEN("The option -w is used")
        {
            int argc = 4;
            char * const argv[] { (char *) "cp-tools", (char *) "clean", (char *) "-w", 
                (char *) CP_TOOLS_TEMP_DIR };

            string error;
            REQUIRE(cptools::sh::make_dir(CP_TOOLS_TEMP_DIR, error) == CP_TOOLS_OK);
            REQUIRE(error.empty());

            // getopt library must be reseted between tests
            optind = 1;

            THEN("The subdirectory with the auto-generated files is deleted")
            {
                ostringstream out, err;
                REQUIRE(cptools::clean::run(argc, argv, out, err) == CP_TOOLS_OK);
            }
        }

        WHEN("The option -h is used")
        {
            int argc = 3;
            char * const argv[] { (char *) "cp-tools", (char *) "clean", (char *) "-h" };

            // getopt library must be reseted between tests
            optind = 1;

            THEN("The output is the help message")
            {
                ostringstream out, err;

                auto rc = cptools::clean::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_OK);
                REQUIRE(err.str().empty());
                REQUIRE(out.str() == (cptools::clean::help() + '\n'));
            }
        }

        WHEN("The an invalid option is passed")
        {
            int argc = 3;
            char * const argv[] { (char *) "cp-tools", (char *) "clean", (char *) "-x" };

            optind = 1;
            opterr = 0;

            THEN("The error output is the help message")
            {
                ostringstream out, err;

                auto rc = cptools::clean::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_ERROR_CLEAN_INVALID_OPTION);
                REQUIRE(out.str().empty());
                REQUIRE(err.str() == (cptools::clean::help() + '\n'));
            }
        }
    }
}
