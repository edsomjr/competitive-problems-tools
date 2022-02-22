#ifndef CP_TOOLS_DIRS_H
#define CP_TOOLS_DIRS_H

#include "defs.h"

#define CP_TOOLS_PLUGIN_DIR "./plugins"

#define CP_TOOLS_TEMPLATES_DIR          "/usr/local/lib/" NAME "/templates/"
#define CP_TOOLS_PROBLEM_TEMPLATE_DIR   CP_TOOLS_TEMPLATES_DIR "problem-template/"
#define CP_TOOLS_BOCA_TEMPLATES_DIR     CP_TOOLS_TEMPLATES_DIR "boca-package-structure/"
#define CP_TOOLS_CLASSES_DIR            "/usr/local/lib/" NAME "/classes/"

#define CP_TOOLS_BOCA_BUILD_DIR         CP_TOOLS_BUILD_DIR "/boca/"
#define CP_TOOLS_BUILD_DIR              ".cp-build"
#define CP_TOOLS_TEMP_DIR               ".cp-tmp"

#define CP_TOOLS_CONFIG_FILE            "." NAME "-config.json"

#endif
