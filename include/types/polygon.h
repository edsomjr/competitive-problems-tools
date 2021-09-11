#ifndef CP_TOOLS_TYPES_POLYGON_H
#define CP_TOOLS_TYPES_POLYGON_H

#include <chrono>
#include <string>
#include <vector>

#include "json.hpp"

namespace cptools::types::polygon {

struct Credentials {
    std::string key;
    std::string secret;
};

struct Solution {
    std::string name;
    std::time_t modification_time;
    std::string tag;
};
using SolutionsVector = std::vector<Solution>;
void from_json(const nlohmann::json &j, Solution &s);

struct Statement {
    std::string language;
    std::string title;
};
using StatementsVector = std::vector<Statement>;
void from_json(const nlohmann::json &j, Statement &s);

struct Information {
    int time_limit;
    int memory_limit;
};
void from_json(const nlohmann::json &j, Information &i);

struct Test {
    int index;
    std::string file_name;
    std::string input;
    bool manual;
    bool use_in_statements;
};
using TestsVector = std::vector<Test>;
void from_json(const nlohmann::json &j, Test &t);

} // namespace cptools::types::polygon

#endif