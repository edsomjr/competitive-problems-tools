#include <iostream>
#include <vector>
#include <map>
#include "cptools.h"

#include "config.h"

int main(int argc, char* const argv[])
{
    auto config = cptools::config::read("templates/config.json");

    std::cout << cptools::config::get(config, "author:country", std::string("error")) << '\n';
    std::cout << cptools::config::get(config, "problem:timelimit", 0.0) << '\n';
    std::cout << cptools::config::get(config, "PDF:include_email", true) << '\n';

    auto xs = cptools::config::get(config, "tests:samples", std::vector<std::string> {"oi", "test"});

    for (auto x : xs)
        std::cout << x << '\n';

    auto ys = cptools::config::get(config, "tests:validator", std::map<std::string, std::string> { {"oi", "test"} } );

    for (auto [k, v] : ys)
        std::cout << k << ": " << v << '\n';

    auto zs = cptools::config::get(config, "tests:checker", std::map<std::string, std::vector<std::string>> { {"oi", {"test"}} } );

    for (auto [k, v] : zs)
    {
        std::cout << "k = " << k << '\n';

        for (auto x : v)
            std::cout << x << ' ';

        std::cout << '\n';
    }



    return cptools::run(argc, argv, std::cout, std::cerr);
}
