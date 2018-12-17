#include <sstream>

#include "catch.hpp"
#include "cptools.h"
#include "error.h"


SCENARIO("Command line options", "[cptools]")
{
    GIVEN("An execution of cp-tools from command line")
    {
        WHEN("there is no option")
        {
            int argc = 1;
            char * const argv[] { (char *) "cp-tools" };

            THEN("the error output is the usage message")
            {
                std::ostringstream out, err;

                auto rc = cptools::run(argc, argv, out, err);

                REQUIRE(rc == CP_TOOLS_ERROR_MISSING_ARGUMENT);
                REQUIRE(out.str().empty());
                REQUIRE(err.str() == (cptools::usage() + '\n'));
            }
        }
    }
}
