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
#include "commands/genboca.h"

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

// API functions
int run(int argc, char *const argv[], std::ostream &out, std::ostream &err) {
    int option = -1;

    while ((option = getopt_long(argc, argv, "bh", longopts, NULL)) != -1) {
        switch (option) {
        case 'b':
            return genboca(out, err);

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
