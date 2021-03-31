#include "config.h"
#include "exceptions.h"
#include "util.h"

namespace cptools::config {

nlohmann::json read_config_file() { return util::read_json_file("config.json"); }

std::string get_polygon_problem_id(const nlohmann::json &json_object) {
    const std::string path = "problem|polygonId";
    const std::string problem_id = util::get_json_value<std::string>(json_object, path, "");
    return problem_id;
}

std::string get_tool_file_name(const nlohmann::json &json_object, const std::string &tool) {
    const std::string path = "tools|" + tool;
    const auto file_name = util::get_json_value<std::string>(json_object, path, "");
    return file_name;
}

std::vector<std::string> get_solutions_file_names(const nlohmann::json &json_object,
                                                  const std::string &tag) {
    const std::string path = "solutions|" + tag;

    if (tag == "default") {
        auto file_name = util::get_json_value<std::string>(json_object, path, "");
        if (file_name == "")
            throw(exceptions::invalid_config_error("No valid default solution found"));
        return std::vector<std::string>{file_name};
    }

    auto file_names = util::get_json_value<std::vector<std::string>>(json_object, path, {});
    return file_names;
}

} // namespace cptools::config