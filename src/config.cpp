#include "config.h"
#include "util.h"

namespace cptools::config {

nlohmann::json read_config_file() {
  return util::read_json_file("config.json");
}

std::string get_polygon_problem_id(nlohmann::json json_object) {
  const std::string path = "polygon|problemId";
  const std::string problem_id =
      util::get_json_value<std::string>(json_object, path, "");
  return problem_id;
}

std::string get_checker_file_name(nlohmann::json json_object) {
  const std::string path = "tools|checker";
  const std::string checker_file_name =
      util::get_json_value<std::string>(json_object, path, "");
  return checker_file_name;
}

std::string get_validator_file_name(nlohmann::json json_object) {
  const std::string path = "tools|validator";
  const std::string validator_file_name =
      util::get_json_value<std::string>(json_object, path, "");
  return validator_file_name;
}

} // namespace cptools::config