#include <getopt.h>
#include <iostream>
#include <sstream>
#include <unistd.h>

#include "commands/pack4.h"
#include "config.h"
#include "defs.h"
#include "dirs.h"
#include "error.h"
#include "fs.h"
#include "message.h"
#include "sh.h"
#include "task.h"
#include "util.h"

// Raw strings
static const std::string help_message{
    R"message(
Creates a package for a particular online judge.

    Option          Description

    -h              Generates this help message (default option).
    --help

    -b          Creates a package for BOCA online judge.
    --boca
)message"};

namespace cptools::commands::pack4 {

// Global variables
static struct option longopts[] = {{"boca", no_argument, NULL, 'b'},
                                   {"help", no_argument, NULL, 'h'},
                                   {0, 0, 0, 0}};

// Auxiliary routines
std::string usage() { return "Usage: " NAME " check [-h] [-b]"; }

std::string help() { return usage() + help_message; }

// Essa função deve receber os objetos ostream?
int create_build_dirs(std::ostream &err) {
    std::vector<std::string> dirs{{"compare/"}, {"compile/"}, {"run/"}, {"description/"},
                                  {"input/"}, {"limits/"}, {"output/"}, {"tests/"}};

    for(const auto& dir : dirs) {
        auto path{std::string(CP_TOOLS_BUILD_DIR) + "/boca/" + dir};
        auto fs_res = fs::create_directory(path);

        if (not fs_res.ok) {
            err << message::failure(fs_res.error_message) << '\n';
            return fs_res.rc;
        }
    }

    return CP_TOOLS_OK;
}

int pack4_boca(std::ostream &out, std::ostream &err) {

    // O que essa função deve retornar?
    // A função deve abortar o programa?
    auto res = create_build_dirs(err);

    if(res != CP_TOOLS_OK) {
        err << message::failure("Não foi possível criar ") << '\n';
        return CP_TOOLS_ERROR_JUDGE_MISSING_TOOL;
    }

    out << "Pacote Boca gerado com sucesso\n";

    return CP_TOOLS_OK;
}

// API functions
int run(int argc, char *const argv[], std::ostream &out, std::ostream &err) {
    int option = -1;

    while ((option = getopt_long(argc, argv, "bh", longopts, NULL)) != -1) {
        switch (option) {
        case 'b':
            return pack4_boca(out, err);

        case 'h':
            out << help() << '\n';
            return 0;

        default:
            err << help() << '\n';
            return CP_TOOLS_ERROR_CLEAN_INVALID_OPTION;
        }
    }

    out << usage() << '\n';
    return CP_TOOLS_OK;
}
} // namespace cptools::commands::pack4
