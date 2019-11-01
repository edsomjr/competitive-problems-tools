#include <filesystem>
#include <iostream>
#include <sstream>
#include <map>

#include "catch.hpp"
#include "config.h"


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
                { "PDF|row_size", 6.0 },
                { "PDF|column_size", 8.0 },
            };

            THEN("The get method returns its string value")
            {
                for (auto [k, v] : fields)
                    REQUIRE(cptools::config::get(config, k, -9.99) == v);
            }
        } 
    }
}
