#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <getopt.h>

#include "catch.hpp"

#include "sh.h"
#include "dirs.h"
#include "init.h"
#include "error.h"

SCENARIO("Command problem, action init", "[init]")
{
    GIVEN("An execution of the command init with options")
    {
        WHEN("The option -o is used")
        {
            int argc = 5;
            char * const argv[] { (char *) "cp-tools", (char *) "problem", (char *) "init",
                (char *) "-o", (char *) CP_TOOLS_TEMP_DIR };

            // TODO: checar porque este teste quebra no Travis com SIGSEGV
            THEN("The output directory is initialized with the template files")
            {
                REQUIRE(cptools::sh::remove_dir(CP_TOOLS_TEMP_DIR) >= 0);

                //std::ostringstream out, err;
                //REQUIRE(cptools::init::run(argc, argv, out, err) == CP_TOOLS_OK);

                // REQUIRE(cptools::sh::compare_dirs(CP_TOOLS_TEMP_DIR, CP_TOOLS_TEMPLATES_DIR));
            }
        }

        WHEN("The option -h is used")
        {
            int argc = 4;
            char * const argv[] { (char *) "cp-tools", (char *) "problem", 
                (char *) "init", (char *) "-h" };

            // getopt library must be reseted between tests
            optind = 1;

            THEN("The output is the help message")
            {
                std::ostringstream out, err;

                auto rc = cptools::init::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_OK);
                REQUIRE(err.str().empty());
                REQUIRE(out.str() == (cptools::init::help() + '\n'));
            }
        }

        WHEN("The an invalid option is passed")
        {
            int argc = 4;
            char * const argv[] { (char *) "cp-tools", (char *) "problem",
                (char *) "init", (char *) "-i" };

            // getopt library must be reseted between tests
            optind = 1;

            THEN("The error output is the help message")
            {
                std::ostringstream out, err;

                auto rc = cptools::init::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_ERROR_INIT_INVALID_OPTION);
                REQUIRE(out.str().empty());
                REQUIRE(err.str() == (cptools::init::help() + '\n'));
            }
        }
    }
}
