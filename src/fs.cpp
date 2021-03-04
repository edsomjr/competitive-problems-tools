#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "dirs.h"
#include "fs.h"
#include "message.h"

namespace cptools::fs {
string get_home_dir() {
  char *homedir = getenv("HOME");
  if (homedir == NULL) {
    auto pw_struct = getpwuid(getuid());
    homedir = pw_struct->pw_dir;
  }
  return string(homedir);
}

string get_default_config_path() {
  auto homedir = get_home_dir();
  auto config_path = homedir + "/" + CP_TOOLS_CONFIG_FILE;
  return config_path;
}

bool file_exists(const string &path) {
  struct stat buffer;
  auto success = stat(path.c_str(), &buffer);
  if (success == 0)
    return true;
  return false;
}
} // namespace cptools::fs