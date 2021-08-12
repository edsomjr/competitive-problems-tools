#include <filesystem>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "catch.hpp"
#include "util.h"

SCENARIO("Utilitary functions", "[util]") {
    GIVEN("A string") {
        WHEN("There is a prefix composed by blank characters") {
            THEN("The strip() method removes this prefix") {
                REQUIRE(cptools::util::strip("  abc") == "abc");
                REQUIRE(cptools::util::strip("\tabc") == "abc");
                REQUIRE(cptools::util::strip("\nabc") == "abc");
                REQUIRE(cptools::util::strip("\babc") == "abc");
                REQUIRE(cptools::util::strip("\rabc") == "abc");
                REQUIRE(cptools::util::strip("\r\t\n \b\t\n  abc") == "abc");
                REQUIRE(cptools::util::strip("  a b  c") == "a b  c");
            }
        }

        WHEN("There is a suffix composed by blank characters") {
            THEN("The strip() method removes this suffix") {
                REQUIRE(cptools::util::strip("abc   ") == "abc");
                REQUIRE(cptools::util::strip("abc\t") == "abc");
                REQUIRE(cptools::util::strip("abc\n") == "abc");
                REQUIRE(cptools::util::strip("abc\b") == "abc");
                REQUIRE(cptools::util::strip("abc\r") == "abc");
                REQUIRE(cptools::util::strip("abc\r\t\n \b\t\n  ") == "abc");
                REQUIRE(cptools::util::strip("a b  c\r\t\n \b\t\n  ") == "a b  c");
            }
        }

        WHEN("There is a prefix and a suffix composed by blank characters") {
            THEN("The strip() method removes these characters") {
                REQUIRE(cptools::util::strip("  abc   ") == "abc");
                REQUIRE(cptools::util::strip("\tabc\n") == "abc");
                REQUIRE(cptools::util::strip("\nabc\r\b") == "abc");
                REQUIRE(cptools::util::strip("\babc  \n\t") == "abc");
                REQUIRE(cptools::util::strip("\rabc \n\t \r \n  \t") == "abc");
                REQUIRE(cptools::util::strip("\r\t\n \b\t\n  abc   \n") == "abc");
                REQUIRE(cptools::util::strip("\r\t\n \b\t\n  a b  c   \n") == "a b  c");
            }
        }

        WHEN("There is a prefix composed by N copies of character c") {
            THEN("The strip() method removes these copies") {
                REQUIRE(cptools::util::strip("###abc", '#') == "abc");
                REQUIRE(cptools::util::strip("--abc", '-') == "abc");
                REQUIRE(cptools::util::strip("$$$abc", '$') == "abc");
                REQUIRE(cptools::util::strip("%abc", '%') == "abc");
                REQUIRE(cptools::util::strip("\"abc", '\"') == "abc");
                REQUIRE(cptools::util::strip("aaaaaaaabc", 'a') == "bc");
                REQUIRE(cptools::util::strip("bbbbbbabc", 'b') == "abc");
            }
        }

        WHEN("There is a suffix composed by N copies of character c") {
            THEN("The strip() method removes these copies") {
                REQUIRE(cptools::util::strip("abc   ", ' ') == "abc");
                REQUIRE(cptools::util::strip("abczzzz", 'z') == "abc");
                REQUIRE(cptools::util::strip("abc&&&&", '&') == "abc");
                REQUIRE(cptools::util::strip("abc********", '*') == "abc");
                REQUIRE(cptools::util::strip("abc99", '9') == "abc");
                REQUIRE(cptools::util::strip("abc+++++", '+') == "abc");
                REQUIRE(cptools::util::strip("a b  cccccc", 'c') == "a b  ");
            }
        }

        WHEN("There is a prefix and a suffix composed by N copies of character c") {
            THEN("The strip() method removes these copies") {
                REQUIRE(cptools::util::strip("  abc   ", ' ') == "abc");
                REQUIRE(cptools::util::strip("zabczz", 'z') == "abc");
                REQUIRE(cptools::util::strip("aaaaabcaa", 'a') == "bc");
                REQUIRE(cptools::util::strip("aaaaaa", 'a') == "");
                REQUIRE(cptools::util::strip("bbbbcbbb", 'b') == "c");
                REQUIRE(cptools::util::strip("--abc--", '-') == "abc");
                REQUIRE(cptools::util::strip("", 'x') == "");
            }
        }

        WHEN("The string s has no occurrence of delimiter") {
            THEN("The split() method return just one token: s") {
                REQUIRE(cptools::util::split("abc", '.') == std::vector<std::string>{"abc"});
                REQUIRE(cptools::util::split("a b c", '.') == std::vector<std::string>{"a b c"});
                REQUIRE(cptools::util::split("abd", 'c') == std::vector<std::string>{"abd"});
            }
        }

        WHEN("The string s is composed by n occurrences of delimiter") {
            THEN("The split() method return a empty token list") {
                REQUIRE(cptools::util::split(".....", '.') == std::vector<std::string>{});
                REQUIRE(cptools::util::split("a", 'a') == std::vector<std::string>{});
            }
        }

        WHEN("The string s has r occurrences of delimiter") {
            THEN("The split() method return a vector with (r + 1) tokens") {
                REQUIRE(cptools::util::split("a.b.c", '.') ==
                        (std::vector<std::string>{"a", "b", "c"}));
                REQUIRE(cptools::util::split("a b", 'a') == std::vector<std::string>{" b"});
                REQUIRE(cptools::util::split("a b", 'b') == std::vector<std::string>{"a "});
                REQUIRE(cptools::util::split("a    b", ' ') ==
                        (std::vector<std::string>{"a", "b"}));
                REQUIRE(cptools::util::split("  a    b  ", ' ') ==
                        (std::vector<std::string>{"a", "b"}));
            }
        }
    }

    GIVEN("An JSON config file") {
        auto config = cptools::util::read_json_file("templates/problem-template/config.json");

        WHEN("There is a string field") {
            std::map<std::string, std::string> fields{
                {"author|name", "Nome do Autor"},
                {"author|e-mail", "emaildoautor@email.com"},
                {"author|school", "Universidade/Escola do autor do problema"},
                {"author|country", "Brazil"},
                {"problem|title|pt_BR", "Título do problema, time limit em ms, memory limit em MB"},
                {"problem|title|en_US", "Title"},
                {"problem|contest", "Evento onde o problema foi aplicado pela primeira vez"},
                {"solutions|default", "solutions/solution.cpp"},
                {"tools|checker", "tools/checker.cpp"},
                {"tools|generator", "tools/generator.cpp"},
                {"tools|validator", "tools/validator.cpp"},
                {"tests|validator|tests/validator/1", "OK"},
                {"tests|validator|tests/validator/2", "INVALID"},
                {"PDF|first_column_size", "6cm"},
                {"PDF|second_column_size", "8cm"},
            };

            THEN("The get method returns its string value") {
                for (auto [k, v] : fields)
                    REQUIRE(cptools::util::get_json_value(config, k, std::string("error")) == v);
            }
        }

        WHEN("There is a missing string field") {
            std::string field{"missing"};

            THEN("The get method returns the indicated default value") {
                REQUIRE(cptools::util::get_json_value(config, field, std::string("error")) ==
                        "error");
            }
        }

        WHEN("There is a numeric field") {
            std::map<std::string, double> fields{
                {"problem|timelimit", 1000},
                {"problem|memory_limit", 256},
            };

            THEN("The get method returns its double value") {
                for (auto [k, v] : fields)
                    REQUIRE(cptools::util::get_json_value(config, k, -9.99) == v);
            }
        }

        WHEN("There is a boolean field") {
            std::map<std::string, bool> fields{
                {"PDF|include_email", true},
                {"PDF|include_author", true},
                {"PDF|include_contest", true},
                {"PDF|include_disclaimer", true},
            };

            THEN("The get method returns its bool value") {
                for (auto [k, v] : fields)
                    REQUIRE(cptools::util::get_json_value(config, k, !v) == v);
            }
        }

        WHEN("There is a list field") {
            std::map<std::string, std::vector<std::string>> fields{
                {"solutions|ac", {"solutions/AC.java", "solutions/ac.py"}},
                {"solutions|pe", {"solutions/pe.cpp"}},
                {"solutions|wa", {"solutions/wa.cpp"}},
                {"solutions|tle", {"solutions/tle.cpp"}},
                {"tests|random", {"10 10", "100 100", "50 1000"}},
            };

            THEN("The get method returns a vector of strings") {
                for (auto [k, v] : fields)
                    REQUIRE(cptools::util::get_json_value(config, k, std::vector<std::string>{}) ==
                            v);
            }
        }

        WHEN("There is a map field") {
            std::map<std::string, std::map<std::string, std::string>> fields{
                {"tests|samples",
                 {
                     {"tests/1", "Comentário do teste 1"},
                     {"tests/2", "Comentário do teste 2"},
                     {"tests/3", "Comentário do teste 3"},
                 }},

                {"tests|manual",
                 {
                     {"tests/4", "Comentário do teste 4"},
                     {"tests/5", "Comentário do teste 5"},
                     {"tests/6", "Comentário do teste 6"},
                 }},

                {"tests|validator",
                 {
                     {"tests/validator/1", "OK"},
                     {"tests/validator/2", "INVALID"},
                 }},
            };

            THEN("The get method returns a map of pairs of strings") {
                for (auto [k, v] : fields)
                    REQUIRE(cptools::util::get_json_value(
                                config, k, std::map<std::string, std::string>{}) == v);
            }
        }

        WHEN("There is a map of lists field") {
            std::map<std::string, std::map<std::string, std::vector<std::string>>> fields{
                {"tests|checker",
                 {{"tests/checker/in1", {"tests/checker/out1", "AC"}},
                  {"tests/checker/in2", {"tests/checker/out2", "PE"}},
                  {"tests/checker/in3", {"tests/checker/out3", "WA"}}}},
            };

            THEN("The get method returns a map of vectors of strings") {
                for (auto [k, v] : fields)
                    REQUIRE(cptools::util::get_json_value(
                                config, k, std::map<std::string, std::vector<std::string>>{}) == v);
            }
        }
    }
}
