#ifndef CP_TOOLS_CONFIG_H
#define CP_TOOLS_CONFIG_H

#include <iostream>

#include "json.hpp"
#include "util.h"

using nlohmann::json;

namespace cptools::config {

    json read(const std::string& config_file_path);

    template<typename T>
    T get(const json& config, const std::string& fields, T default_value)
    {
        auto fs = split(fields, '|');
        auto js = config;

        for (const auto& f : fs)
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
        } catch (std::exception& e)
        {
        }

        return default_value;
    }
    
}

#endif
