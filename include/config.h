#ifndef CP_TOOLS_CONFIG_H
#define CP_TOOLS_CONFIG_H

#include <string>

#include "json.hpp"

namespace cptools::config {
nlohmann::json read_config_file();
} // namespace cptools::config

#endif