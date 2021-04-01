#ifndef CP_TOOLS_UTIL_H
#define CP_TOOLS_UTIL_H

#include <string>
#include <vector>

#include "json.hpp"

namespace cptools::util {
std::vector<std::string> split(const std::string &s, char delim = ' ');

std::string strip(const std::string &s);
std::string strip(const std::string &s, char c);

std::string lower_string(const std::string &s);

nlohmann::json read_json_file(const std::string &config_file_path);

std::string sha_512(const std::string &s);

template <typename T>
T get_json_value(const nlohmann::json &config, const std::string &fields, T default_value) {
    auto fs = util::split(fields, '|');
    auto js = config;

    for (const auto &f : fs) {
        auto it = js.find(f);

        if (it == js.end())
            return default_value;
        else
            js = *it;
    }

    try {
        auto value = js.get<T>();
        return value;
    } catch (std::exception &e) {
    }

    return default_value;
}

} // namespace cptools::util

#endif
