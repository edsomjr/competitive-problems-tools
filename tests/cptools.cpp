#include <sstream>

#include "catch.hpp"
#include "cptools.h"
#include "error.h"


SCENARIO("Command line options", "[cptools]")
{
    GIVEN("An execution of cp-tools from command line")
    {
        WHEN("There is no option")
        {
            int argc = 1;
            char * const argv[] { (char *) "cp-tools" };

            THEN("The error output is the usage message")
            {
                std::ostringstream out, err;

                auto rc = cptools::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_ERROR_MISSING_ARGUMENT);
                REQUIRE(out.str().empty());
                REQUIRE(err.str() == (cptools::usage() + '\n'));
            }
        }

        WHEN("The option -h is used")
        {
            int argc = 2;
            char * const argv[] { (char *) "cp-tools", (char *) "-h" };

            THEN("The output is the help message")
            {
                std::ostringstream out, err;

                auto rc = cptools::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_OK);
                REQUIRE(err.str().empty());
                REQUIRE(out.str() == (cptools::help() + '\n'));
            }
        }

        WHEN("The option 2 -h is used")
        {
            int argc = 2;
            char * const argv[] { (char *) "cp-tools", (char *) "-h" };

            // getopt library must be reseted between tests
            optind = 1;

            THEN("The output is the help message")
            {
                std::ostringstream out, err;

                auto rc = cptools::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_OK);
                REQUIRE(err.str().empty());
                REQUIRE(out.str() == (cptools::help() + '\n'));
            }
        }


/*
        WHEN("The option -v is used")
        {
            int argc = 2;
            char * const argv[] { (char *) "cp-tools", (char *) "-h" };

            THEN("The output is the version message")
            {
                std::ostringstream out, err;

                auto rc = cptools::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_OK);
                REQUIRE(err.str().empty());
                REQUIRE(out.str() == (cptools::version() + '\n'));
            }
        }
*/

    }
}
