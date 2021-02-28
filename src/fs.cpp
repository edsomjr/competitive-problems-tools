#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "fs.h"
#include "dirs.h"

namespace cptools::fs {
    string get_home_dir() {
        char *homedir = getenv("HOME");
        if(homedir == NULL) {
            auto pw_struct = getpwuid(getuid());
            homedir = pw_struct->pw_dir;
        }
        return string(homedir);
    }

    string get_default_config() {
        auto homedir = get_home_dir();
        return homedir + "/" + CP_TOOLS_CONFIG_FILE;
    }
}