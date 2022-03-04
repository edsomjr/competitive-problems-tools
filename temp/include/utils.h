#ifndef CP_TOOLS_UTILS_H
#define CP_TOOLS_UTILS_H

#include <string>
#include <vector>

#include "json.hpp"


namespace cptools::utils {

    size_t levenshtein_distance(const std::string& s, const std::string& t);

    template <typename T>
    T get_json_value(
        const nlohmann::json &config,
        const std::string &fields,
        T default_value
    );

    std::vector<std::string>
    split(const std::string &s, const std::string &delimiter=" ");
}

#endif
