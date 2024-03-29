#include <unordered_map>

#include <getopt.h>
#include <unistd.h>

#include "defs.h"
#include "error.h"

#include "cli/cli.h"
#include "commands/check.h"
#include "commands/clean.h"
#include "commands/cptools.h"
#include "commands/genboca.h"
#include "commands/genpdf.h"
#include "commands/gentex.h"
#include "commands/init.h"
#include "commands/judge.h"
#include "commands/polygon/polygon.h"

// Raw strings
static const std::string help_message{
    R"message(
Format, test and pack competitive programming problems.

    Action              Description

    init                Generates template files on current directory.
    check               Verifies problem files and tools.
    clean               Removes autogenerated files.
    genpdf              Generates a PDF file from the problem description.
    gentex              Generates a LaTeX file from the problem description.
    genboca             Pack the marathon problem for the online judge BOCA.
    judge               Runs a solution against all tests sets.
    polygon             Connects and synchronize with a Polygon account.
)message"};

static const std::string version_header{NAME " " VERSION "\n"};

static const std::string version_body{
    R"body(License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

Written by Edson Alves.)body"};

namespace cptools::commands {

// Global variables
std::unordered_map<std::string, int (*)(int, char *const[])> cmds{
    {"init", init::run},       {"check", check::run},    {"clean", clean::run},
    {"gentex", gentex::run},   {"genpdf", genpdf::run},  {"judge", judge::run},
    {"polygon", polygon::run}, {"genboca", genboca::run}};

static struct option longopts[] = {
    {"help", no_argument, NULL, 'h'}, {"version", no_argument, NULL, 'v'}, {0, 0, 0, 0}};

// Auxiliary routines
std::string usage() { return "Usage: " NAME " [-h] [-v] action"; }

std::string help() { return usage() + help_message; }

std::string version() { return version_header + version_body; }

// API functions
int run(int argc, char *const argv[], std::ostream &out, std::ostream &err) {
    cli::set_output_stream(out);
    cli::set_error_stream(err);

    if (argc >= 2) {
        std::string command{argv[1]};
        auto it = cmds.find(command);

        if (it != cmds.end()) {
            return cmds[command](argc, argv);
        }

        if (command.front() != '-') {
            cli::write(cli::fmt::error, "Invalid action '" + command + "'");
            return CP_TOOLS_ERROR_INVALID_COMMAND;
        }
    }

    int option = -1;

    while ((option = getopt_long(argc, argv, "hv", longopts, NULL)) != -1) {
        switch (option) {
        case 'h':
            cli::write(cli::fmt::none, help());
            return CP_TOOLS_OK;

        case 'v':
            cli::write(cli::fmt::none, version());
            return CP_TOOLS_OK;

        default:
            cli::write(cli::fmt::none, help(), true);
            return CP_TOOLS_ERROR_INVALID_OPTION;
        }
    }

    cli::write(cli::fmt::none, usage());
    return CP_TOOLS_OK;
}
} // namespace cptools::commands
