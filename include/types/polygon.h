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

} // namespace cptools::types::polygon

#endif