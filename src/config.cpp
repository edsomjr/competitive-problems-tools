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
} // namespace cptools::config