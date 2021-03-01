#ifndef CP_TOOLS_CONFIG_H
#define CP_TOOLS_CONFIG_H

#include <iostream>

#include "json.hpp"
#include "util.h"

using nlohmann::json;

namespace cptools::config {

    json read(const std::string& config_file_path);

    template<typename T>
    T get(const json& config, const std::string& fields, T default_value);
    
}

#endif
