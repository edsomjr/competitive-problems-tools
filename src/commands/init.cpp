#include <filesystem>
#include <getopt.h>
#include <iostream>
#include <unistd.h>

#include "cli/cli.h"
#include "commands/init.h"
#include "defs.h"
#include "dirs.h"
#include "error.h"
#include "fs.h"
#include "sh.h"

// Raw strings
static const std::string help_message{
    R"message(
Initialize an empty directory with template files. If a file already exists, it is not overridden.

    Option          Description

    -h              Generates this help message.
    --help

    -o              Directory to be initialized
    --output

)message"};

namespace cptools::commands::init {

// Global variables
static struct option longopts[] = {
    {"help", no_argument, NULL, 'h'}, {"output", required_argument, NULL, 'o'}, {0, 0, 0, 0}};

// Auxiliary routines
std::string usage() { return "Usage: " NAME " init [-h] [-o output_dir]"; }

std::string help() { return usage() + help_message; }

int copy_template_files(const std::string &dest) {
    cli::write(cli::fmt::info, "Initializing directory '" + dest + "' ...");

    // Copy templates to the directory
    auto res = cptools::fs::copy(CP_TOOLS_PROBLEM_TEMPLATE_DIR, dest, true);
    if (not res.ok)
        cli::write(cli::fmt::error, res.error_message);
    else
        cli::write(cli::fmt::ok);

    return res.rc;
}

// API functions
int run(int argc, char *const argv[]) {
    int option = -1;
    std::string dest{"."};

    while ((option = getopt_long(argc, argv, "ho:", longopts, NULL)) != -1) {
        switch (option) {
        case 'h':
            cli::write(cli::fmt::none, help());
            return 0;

        case 'o':
            dest = std::string(optarg);
            break;

        default:
            cli::write(cli::fmt::none, help(), true);
            return CP_TOOLS_ERROR_INIT_INVALID_OPTION;
        }
    }

    return copy_template_files(dest);
}
} // namespace cptools::commands::init
