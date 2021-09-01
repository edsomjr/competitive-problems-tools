#ifndef CP_TOOLS_CONFIG_H
#define CP_TOOLS_CONFIG_H

#include <string>

#include "fs.h"
#include "json.hpp"
#include "util.h"

namespace cptools::config {

const std::string config_path_name = "config.json";

enum test_type { sample, manual };
const std::unordered_map<test_type, std::string> test_type_tag{{sample, "samples"},
                                                               {manual, "manual"}};

nlohmann::json read_config_file();
std::string get_polygon_problem_id();
std::string get_tool_file_name(const std::string &tool);

std::vector<std::string> get_solutions_file_names(const std::string &tag);

std::vector<std::string> get_tests_file_names(const test_type type);

void insert_solution_file_name(const std::string &tag, const std::string &file_name);

std::string generate_old_config_filename();

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