#ifndef CP_TOOLS_UTILS_H
#define CP_TOOLS_UTILS_H

#include <string>
#include <vector>

#include "json.hpp"

namespace cptools::utils {

    size_t levenshtein_distance(const std::string& s, const std::string& t);

    std::vector<std::string> split(const std::string &s, const std::string &delimiter=" ");

    template <typename T> T get_json_value( const nlohmann::json &config, const std::string &fields, T default_value)
    {
        auto fs = utils::split(fields, "|");
        auto js = config;

        for (const auto &f : fs)
        {
            auto it = js.find(f);

            if (it == js.end())
                return default_value;

            else
                js = *it;
        }

        try {
            auto value = js.get<T>();
            return value;
        } catch (std::exception &e) { }

        return default_value;
    }
}

#endif
