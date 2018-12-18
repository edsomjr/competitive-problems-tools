#include <sstream>
#include <getopt.h>

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

        WHEN("The option is invalid")
        {
            int argc = 2;
            char * const argv[] { (char *) "cp-tools", (char *) "-p" };

            // getopt library must be reseted between tests
            optind = 1;

            THEN("The error output is the help message")
            {
                std::ostringstream out, err;

                auto rc = cptools::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_ERROR_INVALID_PARAMETER);
                REQUIRE(out.str().empty());
                REQUIRE(err.str() == (cptools::help() + '\n'));
            }
        }

        WHEN("The both options are passed")
        {

            THEN("The the first one prevails")
            {
                {
                    int argc = 3;
                    char * const argv[] { (char *) "cp-tools", (char *) "-h", (char *) "-v" };
                    optind = 1;

                    std::ostringstream out, err;

                    auto rc = cptools::run(argc, argv, out, err);

                    REQUIRE(rc == CP_TOOLS_OK);
                    REQUIRE(err.str().empty());
                    REQUIRE(out.str() == (cptools::help() + '\n'));
                }

                {
                    int argc = 3;
                    char * const argv[] { (char *) "cp-tools", (char *) "-v", (char *) "-h" };
                    optind = 1;

                    std::ostringstream out, err;

                    auto rc = cptools::run(argc, argv, out, err);

                    REQUIRE(rc == CP_TOOLS_OK);
                    REQUIRE(err.str().empty());
                    REQUIRE(out.str() == (cptools::version() + '\n'));
                }
            }
        }
    }
}
