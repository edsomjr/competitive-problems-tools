#ifndef CP_TOOLS_CONFIG_H
#define CP_TOOLS_CONFIG_H

#include <string>

#include "json.hpp"

namespace cptools::config {

nlohmann::json read_config_file();
std::string get_polygon_problem_id(nlohmann::json json_object);
std::string get_tool_file_name(nlohmann::json json_object,
                               const std::string &tool);

} // namespace cptools::config

#endif