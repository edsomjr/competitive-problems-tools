#include <sstream>
#include <getopt.h>

#include "catch.hpp"

#include "problem.h"
#include "error.h"


SCENARIO("Command line options for command 'problem'", "[cptools|problem]")
{
    GIVEN("An execution of cp-tools problem from command line")
    {
        WHEN("There is no option")
        {
            int argc = 2;
            char * const argv[] { (char *) "cp-tools", (char *) "problem" };
            optind = 1;     // getopt library must be reseted between tests

            THEN("The output must be the usage message")
            {
                std::ostringstream out, err;

                auto rc = cptools::problem::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_OK);
                REQUIRE(out.str() == (cptools::problem::usage() + '\n'));
                REQUIRE(err.str().empty());
            }
        }

        WHEN("The option -h is used")
        {
            int argc = 3;
            char * const argv[] { (char *) "cp-tools", (char *) "problem", (char *) "-h" };
            optind = 1;     // getopt library must be reseted between tests
            
            THEN("The output must be the help message")
            {
                std::ostringstream out, err;

                auto rc = cptools::problem::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_OK);
                REQUIRE(err.str().empty());
                REQUIRE(out.str() == (cptools::problem::help() + '\n'));
            }
        }

        WHEN("The option is invalid")
        {
            int argc = 3;
            char * const argv[] { (char *) "cp-tools", (char *) "problem", (char *) "-x" };
            optind = 1;
            opterr = 0;     // Suppres getopt_long error message

            THEN("The error output must be the help message")
            {
                std::ostringstream out, err;

                auto rc = cptools::problem::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_ERROR_INVALID_PARAMETER);
                REQUIRE(out.str().empty());
                REQUIRE(err.str() == (cptools::problem::help() + '\n'));
            }
        }
    }
}
