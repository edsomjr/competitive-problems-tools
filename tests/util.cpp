#include <string>

#include "catch.hpp"
#include "util.h"

using std::string;

SCENARIO("Utilitary functions", "[util]")
{
    GIVEN("A string")
    {
        WHEN("There is a prefix composed by blank characters")
        {
            THEN("The strip() method removes this prefix")
            {
                REQUIRE(cptools::util::strip("  abc") == "abc");
                REQUIRE(cptools::util::strip("\tabc") == "abc");
                REQUIRE(cptools::util::strip("\nabc") == "abc");
                REQUIRE(cptools::util::strip("\babc") == "abc");
                REQUIRE(cptools::util::strip("\rabc") == "abc");
                REQUIRE(cptools::util::strip("\r\t\n \b\t\n  abc") == "abc");
                REQUIRE(cptools::util::strip("  a b  c") == "a b  c");
            }
        }

        WHEN("There is a suffix composed by blank characters")
        {
            THEN("The strip() method removes this suffix")
            {
                REQUIRE(cptools::util::strip("abc   ") == "abc");
                REQUIRE(cptools::util::strip("abc\t") == "abc");
                REQUIRE(cptools::util::strip("abc\n") == "abc");
                REQUIRE(cptools::util::strip("abc\b") == "abc");
                REQUIRE(cptools::util::strip("abc\r") == "abc");
                REQUIRE(cptools::util::strip("abc\r\t\n \b\t\n  ") == "abc");
                REQUIRE(cptools::util::strip("a b  c\r\t\n \b\t\n  ") == "a b  c");
            }
        }

        WHEN("There is a prefix and a suffix composed by blank characters")
        {
            THEN("The strip() method removes these characters")
            {
                REQUIRE(cptools::util::strip("  abc   ") == "abc");
                REQUIRE(cptools::util::strip("\tabc\n") == "abc");
                REQUIRE(cptools::util::strip("\nabc\r\b") == "abc");
                REQUIRE(cptools::util::strip("\babc  \n\t") == "abc");
                REQUIRE(cptools::util::strip("\rabc \n\t \r \n  \t") == "abc");
                REQUIRE(cptools::util::strip("\r\t\n \b\t\n  abc   \n") == "abc");
                REQUIRE(cptools::util::strip("\r\t\n \b\t\n  a b  c   \n") == "a b  c");
            }
        }

        WHEN("There is a prefix composed by N copies of character c")
        {
            THEN("The strip() method removes these copies")
            {
                REQUIRE(cptools::util::strip("###abc", '#') == "abc");
                REQUIRE(cptools::util::strip("--abc", '-') == "abc");
                REQUIRE(cptools::util::strip("$$$abc", '$') == "abc");
                REQUIRE(cptools::util::strip("%abc", '%') == "abc");
                REQUIRE(cptools::util::strip("\"abc", '\"') == "abc");
                REQUIRE(cptools::util::strip("aaaaaaaabc", 'a') == "bc");
                REQUIRE(cptools::util::strip("bbbbbbabc", 'b') == "abc");
            }
        }

        WHEN("There is a suffix composed by N copies of character c")
        {
            THEN("The strip() method removes these copies")
            {
                REQUIRE(cptools::util::strip("abc   ", ' ') == "abc");
                REQUIRE(cptools::util::strip("abczzzz", 'z') == "abc");
                REQUIRE(cptools::util::strip("abc&&&&", '&') == "abc");
                REQUIRE(cptools::util::strip("abc********", '*') == "abc");
                REQUIRE(cptools::util::strip("abc99", '9') == "abc");
                REQUIRE(cptools::util::strip("abc+++++", '+') == "abc");
                REQUIRE(cptools::util::strip("a b  cccccc", 'c') == "a b  ");
            }
        }

        WHEN("There is a prefix and a suffix composed by N copies of character c")
        {
            THEN("The strip() method removes these copies")
            {
                REQUIRE(cptools::util::strip("  abc   ", ' ') == "abc");
                REQUIRE(cptools::util::strip("zabczz", 'z') == "abc");
                REQUIRE(cptools::util::strip("aaaaabcaa", 'a') == "bc");
                REQUIRE(cptools::util::strip("aaaaaa", 'a') == "");
                REQUIRE(cptools::util::strip("bbbbcbbb", 'b') == "c");
                REQUIRE(cptools::util::strip("--abc--", '-') == "abc");
                REQUIRE(cptools::util::strip("", 'x') == "");
            }
        }

    }
}
