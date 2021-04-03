#ifndef CP_TOOLS_CONFIG_H
#define CP_TOOLS_CONFIG_H

#include <string>

#include "fs.h"
#include "json.hpp"
#include "util.h"

namespace cptools::config {

const std::string config_path_name = "config.json";

nlohmann::json read_config_file();
std::string get_polygon_problem_id(const nlohmann::json &json_object);
std::string get_tool_file_name(const nlohmann::json &json_object, const std::string &tool);

std::vector<std::string> get_solutions_file_names(const std::string &tag);

template <typename T>
void modify_config_file(const std::string &path, T new_value, const std::string &op = "replace") {
    auto pointer = util::to_json_pointer(path);
    auto json_patch = util::create_json_operation(pointer, op, new_value);

    auto config = read_config_file();
    auto patched = config.patch(json_patch);
    fs::overwrite_file(config_path_name, patched.dump(4));
}

} // namespace cptools::config

#endif