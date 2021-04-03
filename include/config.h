#ifndef CP_TOOLS_CONFIG_H
#define CP_TOOLS_CONFIG_H

#include <string>

#include "json.hpp"

namespace cptools::config {

nlohmann::json read_config_file();
std::string get_polygon_problem_id(const nlohmann::json &json_object);
std::string get_tool_file_name(const nlohmann::json &json_object, const std::string &tool);

std::vector<std::string> get_solutions_file_names(const nlohmann::json &json_object,
                                                  const std::string &tag);

} // namespace cptools::config

#endif