#include <filesystem>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#include "catch.hpp"
#include "config.h"

/*
SCENARIO("Config read and get", "[config]")
{
    GIVEN("An JSON config file")
    {
        auto config = cptools::config::read("templates/config.json");

        WHEN("There is a string field")
        {
            std::map<std::string, std::string> fields {
                { "author|name", "Nome do Autor" },
                { "author|e-mail", "emaildoautor@email.com" },
                { "author|school", "Universidade/Escola do autor do problema" },
                { "author|country", "Brazil" },
                { "problem|title|pt_BR", "Título do problema, time limit em ms, memory limit em MB" },
                { "problem|title|en_US", "Title" },
                { "problem|contest", "Evento onde o problema foi aplicado pela primeira vez" },
                { "solutions|default", "solution.cpp" },
                { "tools|checker", "tools/checker.cpp" },
                { "tools|generator", "tools/generator.cpp" },
                { "tools|validator", "tools/validator.cpp" },
                { "tests|validator|tests/validator/1", "OK" },
                { "tests|validator|tests/validator/2", "INVALID" },
                { "PDF|first_column_size", "6cm" },
                { "PDF|second_column_size", "8cm" },
            };
 
            THEN("The get method returns its string value")
            {
                for (auto [k, v] : fields)
                    REQUIRE(cptools::config::get(config, k, std::string("error")) == v);
            }
        }


        WHEN("There is a missing string field")
        {
            std::string field { "missing" };

            THEN("The get method returns the indicated default value")
            {
                REQUIRE(cptools::config::get(config, field, std::string("error")) == "error");
            }
        }

        WHEN("There is a numeric field")
        {
            std::map<std::string, double> fields {
                { "problem|timelimit", 1000 },
                { "problem|memory_limit", 256 },
            };

            THEN("The get method returns its double value")
            {
                for (auto [k, v] : fields)
                    REQUIRE(cptools::config::get(config, k, -9.99) == v);
            }
        } 

        WHEN("There is a boolean field")
        {
            std::map<std::string, bool> fields {
                { "PDF|include_email", false },
                { "PDF|include_author", true },
                { "PDF|include_contest", true },
                { "PDF|include_disclaimer", true },
            };

            THEN("The get method returns its bool value")
            {
                for (auto [k, v] : fields)
                    REQUIRE(cptools::config::get(config, k, !v) == v);
            }
        } 

        WHEN("There is a list field")
        {
            std::map<std::string, std::vector<std::string>> fields {
                { "solutions|ac", { "AC.java", "ac.py" } },
                { "solutions|pe", { "pe.cpp" } },
                { "solutions|wa", { "wa.cpp" } },
                { "solutions|tle", { "tle.cpp" } }
            };

            THEN("The get method returns a vector of strings")
            {
                for (auto [k, v] : fields)
                    REQUIRE(cptools::config::get(config, k, std::vector<std::string> {} ) == v);
            }
        } 

        WHEN("There is a map field")
        {
            std::map<std::string, std::map<std::string, std::string>> fields {
                { "tests|validator", { 
                    { "tests/validator/1", "OK" },
                    { "tests/validator/2", "INVALID" },
                }},
            };

            THEN("The get method returns a map of pairs of strings")
            {
                for (auto [k, v] : fields)
                    REQUIRE(cptools::config::get(config, k, std::map<std::string, std::string> {} ) == v);
            }
        } 

        WHEN("There is a map of lists field")
        {
            std::map<std::string, std::map<std::string, std::vector<std::string>>> fields {
                { "tests|checker", { 
                    { "tests/checker/in1", { "tests/checker/out1", "AC" } },
                    { "tests/checker/in2", { "tests/checker/out2", "PE" } },
                    { "tests/checker/in3", { "tests/checker/out3", "WA" } }
                }},
            };

            THEN("The get method returns a map of vectors of strings")
            {
                for (auto [k, v] : fields)
                    REQUIRE(cptools::config::get(config, k, std::map<std::string, std::vector<std::string>> {} ) == v);
            }
        } 



    }
}
*/
