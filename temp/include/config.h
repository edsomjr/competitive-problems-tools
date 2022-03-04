#ifndef CP_TOOLS_CONFIG_H
#define CP_TOOLS_CONFIG_H

#include "json.hpp"

namespace cptools::config
{
    const std::string config_path_name = "config.json";

    nlohmann::json read_config_file();
}

#endif