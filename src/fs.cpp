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

const Result make_result(bool res, int rc,
                         const std::filesystem::filesystem_error &e) {
  Result result{res, rc, ""};
  result.error_message = e.what();
  return result;
}

const Result make_result(bool res, int rc, const std::string err_msg) {
  return Result{res, rc, err_msg};
}

const Result create_directory(const std::string &path) {
  if (exists(path).ok)
    return make_result(true);

  bool created = false;
  try {
    created = std::filesystem::create_directory(path);
  } catch (const std::filesystem::filesystem_error &err) {
    return make_result(false, CP_TOOLS_ERROR_CPP_FILESYSTEM_CREATE_DIRECTORY,
                       err);
  }

  if (created)
    return make_result(created);
  else
    return make_result(created, CP_TOOLS_ERROR_CPP_FILESYSTEM_CREATE_DIRECTORY,
                       "Failed to create directory " + path);
}

const Result exists(const std::string &path) {
  bool exists = false;
  try {
    exists = std::filesystem::exists(path);
  } catch (const std::filesystem::filesystem_error &err) {
    return make_result(false, CP_TOOLS_ERROR_CPP_FILESYSTEM_EXISTS, err);
  }

  if (exists)
    return make_result(exists);
  else
    return make_result(exists, CP_TOOLS_ERROR_CPP_FILESYSTEM_EXISTS,
                       "Impossible to check if" + path + "exists");
}

const Result copy_file(const std::string &src, const std::string &dst) {
  bool copied = false;
  try {
    copied = std::filesystem::copy_file(src, dst);
  } catch (const std::filesystem::filesystem_error &err) {
    return make_result(false, CP_TOOLS_ERROR_CPP_FILESYSTEM_COPY_FILE, err);
  }

  if (copied)
    return make_result(copied);
  else
    return make_result(copied, CP_TOOLS_ERROR_CPP_FILESYSTEM_COPY_FILE,
                       "Impossible to copy " + src + " to " + dst);
}

const Result remove(const std::string &path) {
  bool removed = false;
  try {
    removed = std::filesystem::remove(path);
  } catch (const std::filesystem::filesystem_error &err) {
    return make_result(false, CP_TOOLS_ERROR_CPP_FILESYSTEM_REMOVE, err);
  }

  if (removed)
    return make_result(removed);
  else
    return make_result(removed, CP_TOOLS_ERROR_CPP_FILESYSTEM_REMOVE,
                       "Impossible to remove " + path);
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