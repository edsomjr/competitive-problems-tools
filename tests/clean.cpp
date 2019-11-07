#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <getopt.h>

#include "catch.hpp"

#include "clean.h"
#include "error.h"

SCENARIO("Command problem, action clean", "[clean]")
{
    GIVEN("An execution of the command clean with options")
    {
/*        WHEN("There is no option")
        {
            int argc = 2;
            char * const argv[] { (char *) "cp-tools", (char *) "clean" };

            THEN("The directory with the auto-generated files is deleted")
            {
                std::string tmp_dir { ".cp-tmpdir" };
                std::string build_dir { tmp_dir + "/.cp-build" };

                auto command = "rm -rf " + build_dir;

                REQUIRE(std::system(command.c_str()) == 0);

                command = "mkdir -p " + build_dir;
                REQUIRE(std::system(command.c_str()) == 0);

                command = "cd " + tmp_dir + " && cp-tools clean";
                REQUIRE(std::system(command.c_str()) == 0);

                REQUIRE(not std::filesystem::exists(build_dir));
            }
        }
*/
        WHEN("The option -h is used")
        {
            int argc = 4;
            char * const argv[] { (char *) "cp-tools", (char *) "problem", (char *) "clean",
                (char *) "-h" };

            // getopt library must be reseted between tests
            optind = 1;

            THEN("The output is the help message")
            {
                std::ostringstream out, err;

                auto rc = cptools::clean::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_OK);
                REQUIRE(err.str().empty());
                REQUIRE(out.str() == (cptools::clean::help() + '\n'));
            }
        }

        WHEN("The an invalid option is passed")
        {
            int argc = 4;
            char * const argv[] { (char *) "cp-tools", (char *) "problem", (char *) "clean",
                (char *) "-i" };

            optind = 1;

            THEN("The error output is the help message")
            {
                std::ostringstream out, err;

                auto rc = cptools::clean::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_ERROR_CLEAN_INVALID_OPTION);
                REQUIRE(out.str().empty());
                REQUIRE(err.str() == (cptools::clean::help() + '\n'));
            }
        }
    }
}
