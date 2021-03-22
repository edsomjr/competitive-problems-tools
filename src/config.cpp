#include "config.h"
#include "util.h"

namespace cptools::config {
nlohmann::json read_config_file() {
  return util::read_json_file("config.json");
}
} // namespace cptools::config