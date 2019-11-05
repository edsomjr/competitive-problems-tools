#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <getopt.h>

#include "catch.hpp"

#include "init.h"
#include "error.h"

/*
SCENARIO("Command problem, action init", "[init]")
{
    GIVEN("An execution of the command init with options")
    {
        WHEN("There is no option")
        {
            int argc = 3;
            char * const argv[] { (char *) "cp-tools", (char *) "problem", (char *) "init" };

            THEN("The current directory is initialized with the template files")
            {
                std::string tmp_dir { ".cp-tmpdir" };
                std::string dir { tmp_dir + "/cp_tools_test_init" };
                std::string templates_dir { "/usr/local/lib/cp-tools/templates" };

                auto command = "rm -rf " + dir;

                REQUIRE(std::system(command.c_str()) == 0);

                command = "mkdir -p " + dir;
                REQUIRE(std::system(command.c_str()) == 0);

                command = "cd " + dir + " && cp-tools init";
                REQUIRE(std::system(command.c_str()) == 0);

                command = "diff -r " + dir + " " + templates_dir;
                REQUIRE(std::system(command.c_str()) == 0);
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
*/
