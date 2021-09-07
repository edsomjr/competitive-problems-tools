#include "config.h"
#include "exceptions.h"
#include "util.h"

namespace cptools::config {

nlohmann::json read_config_file() { return util::read_json_file(config_path_name); }

std::string get_polygon_problem_id() {
    const auto json_object = read_config_file();
    const std::string path = "problem|polygonId";
    const std::string problem_id = util::get_json_value<std::string>(json_object, path, "");
    return problem_id;
}

std::string get_tool_file_name(const std::string &type) {
    for (const auto [key, value] : tool_type_tag) {
        if (value == type)
            return get_tool_file_name(key);
    }
    return "";
}

std::string get_tool_file_name(const tool_type type) {
    const auto json_object = read_config_file();
    const std::string path = "tools|" + tool_type_tag.at(type);
    const auto file_name = util::get_json_value<std::string>(json_object, path, "");
    return file_name;
}

/**
 * @brief Generates a file path to be used as the backup for the actual config file.
 *
 * @return std::string with the file path.
 */
std::string generate_old_config_filename() {
    auto config_path = std::filesystem::path(config_path_name);
    auto ext = config_path.extension().string();
    auto filename_no_ext = config_path.replace_extension("").string() + "_";
    auto current_timestamp = util::get_timestamp();
    auto new_name = filename_no_ext + current_timestamp + ext + ".old";
    auto old_config_path = config_path.replace_filename(new_name);
    return old_config_path.string();
}

/**
 * @brief Inserts a solution file name in the config file.
 *
 * @param tag solution's tag
 * @param file_name solution's file name
 */
void insert_solution_file_name(const std::string &tag, const std::string &file_name) {
    const auto json_object = read_config_file();
    const std::string path = "solutions|" + tag;

    auto files_with_tag = get_solutions_file_names(tag);
    auto found = std::find(files_with_tag.begin(), files_with_tag.end(), file_name);

    if (tag == "default") {
        modify_config_file(path, file_name);
        return;
    }

    if (found == files_with_tag.end()) {
        files_with_tag.push_back(file_name);
    }

    modify_config_file(path, files_with_tag);
}

/**
 * @brief Returns a vector with the obtained values found on config.json file using the
 *        tag string.
 *
 *
 * @param tag key used to search on config.json file.
 * @return std::vector<std::string> Returns a vector with the strings found on the
 *                                  config.json using the tag strings.
 */
std::vector<std::string> get_solutions_file_names(const std::string &tag) {
    const auto config_json = read_config_file();
    const std::string path = "solutions|" + tag;

    if (tag == "default") {
        auto file_name = util::get_json_value<std::string>(config_json, path, "");
        return std::vector<std::string>{file_name};
    }

    auto file_names = util::get_json_value<std::vector<std::string>>(config_json, path, {});
    return file_names;
}

std::vector<std::string> get_tests_file_names(const test_type type) {
    const auto config_json = read_config_file();
    auto path = "tests|" + test_type_tag.at(type);
    auto tests =
        util::get_json_value<std::unordered_map<std::string, std::string>>(config_json, path, {});

    std::vector<std::string> file_names;
    std::ranges::transform(tests, std::back_inserter(file_names),
                           [](const auto &pair) { return pair.first; });

    return file_names;
}

std::vector<std::string> get_random_tests_inputs() {
    const auto config_json = read_config_file();
    auto path = "tests|random";
    auto tests = util::get_json_value<std::vector<std::string>>(config_json, path, {});
    return tests;
}

} // namespace cptools::config