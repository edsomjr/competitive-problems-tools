#include "config.h"
#include "exceptions.h"
#include "fs.h"
#include "util.h"

#include <filesystem>
#include <fstream>

using std::filesystem::exists;
using std::filesystem::filesystem_error;

namespace cptools::config {

json read(const std::string &config_file_path) {
  bool res = false;
  try {
    res = exists(config_file_path);
  } catch (const filesystem_error &err) {
  }

  if (res) {
    std::ifstream config_file(config_file_path);
    json config;

    config_file >> config;

    return config;
  }

  throw(exceptions::inexistent_file());
}

} // namespace cptools::config
