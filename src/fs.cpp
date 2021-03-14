#include <filesystem>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "dirs.h"
#include "error.h"
#include "fs.h"
#include "message.h"

namespace cptools::fs {

const Result make_result(bool res) { return Result{res, CP_TOOLS_OK, ""}; }

const Result make_result(bool res, const std::filesystem::filesystem_error &e) {
  Result result{res, CP_TOOLS_ERROR_CPP_FILESYSTEM_CREATE_DIRECTORY, ""};
  result.error_message = e.what();
  return result;
}

const Result make_result(bool res, const std::string err_msg) {
  return Result{res, CP_TOOLS_ERROR_CPP_FILESYSTEM_CREATE_DIRECTORY, err_msg};
}

const Result create_directory(const std::string &path) {
  if (exists(path).ok)
    return make_result(true);

  bool created = false;
  try {
    created = std::filesystem::create_directory(path);
  } catch (const std::filesystem::filesystem_error &err) {
    return make_result(false, err);
  }

  if (created)
    return make_result(created);
  else
    return make_result(created, "Failed to create directory " + path);
}

const Result exists(const std::string &path) {
  bool exists = false;
  try {
    exists = std::filesystem::exists(path);
  } catch (const std::filesystem::filesystem_error &err) {
    return make_result(false, err);
  }

  if (exists)
    return make_result(exists);
  else
    return make_result(exists, "Impossible to check if" + path + "exists");
}

std::string get_home_dir() {
  char *homedir = getenv("HOME");
  if (homedir == NULL) {
    auto pw_struct = getpwuid(getuid());
    homedir = pw_struct->pw_dir;
  }
  return std::string(homedir);
}

std::string get_default_config_path() {
  auto homedir = get_home_dir();
  auto config_path = homedir + "/" + CP_TOOLS_CONFIG_FILE;
  return config_path;
}

} // namespace cptools::fs