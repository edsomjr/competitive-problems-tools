#include <sstream>
#include <getopt.h>

#include "catch.hpp"

#include "cptools.h"
#include "error.h"

using std::ostringstream;

SCENARIO("Command line options", "[cptools]")
{
    GIVEN("An execution of cp-tools from command line")
    {
        WHEN("There is no option")
        {
            int argc = 1;
            char * const argv[] { (char *) "cp-tools" };
            optind = 1;     // getopt library must be reseted between tests

            THEN("The output must be the usage message")
            {
                ostringstream out, err;

                auto rc = cptools::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_OK);
                REQUIRE(out.str() == (cptools::usage() + '\n'));
                REQUIRE(err.str().empty());
            }
        }

        WHEN("The option -h is used")
        {
            int argc = 2;
            char * const argv[] { (char *) "cp-tools", (char *) "-h" };
            optind = 1;     // getopt library must be reseted between tests
            
            THEN("The output must be the help message")
            {
                ostringstream out, err;

                auto rc = cptools::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_OK);
                REQUIRE(err.str().empty());
                REQUIRE(out.str() == (cptools::help() + '\n'));
            }
        }

        WHEN("The option -v is used")
        {
            int argc = 2;
            char * const argv[] { (char *) "cp-tools", (char *) "-v" };
            optind = 1;

            THEN("The output must be the version message")
            {
                ostringstream out, err;

                auto rc = cptools::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_OK);
                REQUIRE(err.str().empty());
                REQUIRE(out.str() == (cptools::version() + '\n'));
            }
        }

        WHEN("The option is invalid")
        {
            int argc = 2;
            char * const argv[] { (char *) "cp-tools", (char *) "-p" };
            optind = 1;
            opterr = 0;     // Suppress getopt_long error message

            THEN("The error output must be the help message")
            {
                ostringstream out, err;

                auto rc = cptools::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_ERROR_INVALID_OPTION);
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

                    ostringstream out, err;

                    auto rc = cptools::run(argc, argv, out, err);

                    REQUIRE(rc == CP_TOOLS_OK);
                    REQUIRE(err.str().empty());
                    REQUIRE(out.str() == (cptools::help() + '\n'));
                }

                {
                    int argc = 3;
                    char * const argv[] { (char *) "cp-tools", (char *) "-v", (char *) "-h" };
                    optind = 1;

                    ostringstream out, err;

                    auto rc = cptools::run(argc, argv, out, err);

                    REQUIRE(rc == CP_TOOLS_OK);
                    REQUIRE(err.str().empty());
                    REQUIRE(out.str() == (cptools::version() + '\n'));
                }
            }
        }
    }
}
