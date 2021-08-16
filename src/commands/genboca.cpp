#include <getopt.h>
#include <iostream>

#include "cli/cli.h"
#include "commands/genpdf.h"
#include "commands/gentex.h"
#include "config.h"
#include "dirs.h"
#include "error.h"
#include "fs.h"
#include "task.h"
#include "util.h"

// Raw strings
static const std::string help_message{
    R"message(
Pack the marathon problem for the online judge BOCA (https://github.com/cassiopc/boca/).

    Option          Description

    -b              Defines the problem label. The default value is 'A'.
    --label

    -c              Document class that will be used. The default value
    --class         is 'cp_modern'.

    -g              Problem language. The default value is 'pt_BR'.
    --lang

    -h              Display this help message.
    --help

    -l              Lists all available document classes.
    --list

    -o              Output file. If omitted, the output will be the file 'problem.pdf'.
    --output

    -t              Generates the PDF for the problem's tutorial.
    --tutorial

    --no_author     Omits problem's author.

    --no_contest    problem's contest.


)message"};

namespace cptools::commands::genboca {

typedef enum { NO_AUTHOR = 1000, NO_CONTEST } Value;

static struct option longopts[] = {{"help", no_argument, NULL, 'h'},
                                   {"label", required_argument, NULL, 'b'},
                                   {"lang", required_argument, NULL, 'g'},
                                   {"list", no_argument, NULL, 'l'},
                                   {"class", required_argument, NULL, 'c'},
                                   {"output", required_argument, NULL, 'o'},
                                   {"tutorial", no_argument, NULL, 't'},
                                   {"no_author", no_argument, NULL, NO_AUTHOR},
                                   {"no_contest", no_argument, NULL, NO_CONTEST},
                                   {0, 0, 0, 0}};

// Auxiliary routines
std::string usage() {
    return "Usage: " NAME " genboca [-h] [-o outfile] [-b label] [-c doc_class] "
           "[-g lang] [-l] [-t] [--no-author] [--no-contest]";
}

std::string help() { return usage() + help_message; }

/**
 * @brief Function that initializes the Boca package build directory.
 *        This function will copy the boca template files to a build directory.
 *
 * @param out
 * @param err
 * @return int Returns CP_TOOLS_OK if everything goes as expected,
 *             or returns a specific error code if a problem occurs.
 */
int create_build_dirs(std::ostream &) {
    std::string boca_build_dir{CP_TOOLS_BOCA_BUILD_DIR};

    auto res_remove = fs::remove(boca_build_dir);

    if (not res_remove.ok) {
        cli::write(cli::message_type::error, res_remove.error_message);
        return res_remove.rc;
    }

    auto res_create = fs::create_directory(boca_build_dir);

    if (not res_create.ok) {
        cli::write(cli::message_type::error, res_create.error_message);
        return res_create.rc;
    }

    auto res_copy = cptools::fs::copy(CP_TOOLS_BOCA_TEMPLATES_DIR, boca_build_dir, true);

    if (not res_copy.ok)
        cli::write(cli::message_type::error, res_copy.error_message);

    return res_copy.rc;
}

int create_description_dir(int argc, char *const argv[], std::ostream &out, std::ostream &err) {
    // optind is a built-in global variable.
    // It is used to control the current position of argv.
    optind = 1;
    int res_pdf = genpdf::run(argc, argv, out, err);
    if (res_pdf != CP_TOOLS_OK) {
        return res_pdf;
    }

    // Generated pdf file name
    std::string pdf_file = util::get_from_argv(argc, argv, {"--output", "-o"}, "problem.pdf");

    std::string boca_desc_dir{CP_TOOLS_BOCA_BUILD_DIR + std::string("description/")};

    // Copy the pdf to boca's build directory
    auto res_cpy = fs::copy(pdf_file, boca_desc_dir, true);
    if (not res_cpy.ok) {
        cli::write(cli::message_type::error, res_cpy.error_message);
        return res_cpy.rc;
    }

    // Rename the copied file to <LABEL>.pdf
    std::string label = util::get_from_argv(argc, argv, {"--label", "-b"}, "A");
    auto res_rnm = fs::rename(boca_desc_dir + pdf_file, boca_desc_dir + label + ".pdf");
    if (not res_rnm.ok) {
        cli::write(cli::message_type::error, res_rnm.error_message);
        return res_rnm.rc;
    }

    std::string problem_info{boca_desc_dir + std::string("problem.info")};

    auto config = config::read_config_file();

    std::string problem_fullname{
        util::get_json_value(config, "problem|title|pt_BR", std::string("Título do problema"))};

    std::string content{std::string("basename=") + label + '\n' + std::string("fullname=") + '\"' +
                        problem_fullname + "\"\n" + std::string("descfile=") + label + ".pdf" +
                        '\n'};

    fs::overwrite_file(problem_info, content);

    return CP_TOOLS_OK;
}

int create_io_dir() {
    auto pairs = task::generate_io_files("all");

    for (const auto &dir : {std::string("input/"), std::string("output/")}) {

        auto res_cpy =
            fs::copy(CP_TOOLS_BUILD_DIR + std::string("/") + dir, CP_TOOLS_BOCA_BUILD_DIR + dir);

        if (not res_cpy.ok) {
            cli::write(cli::message_type::error, res_cpy.error_message);
            return res_cpy.rc;
        }
    }

    return CP_TOOLS_OK;
}

int genboca(int argc, char *const argv[], std::ostream &out, std::ostream &err) {
    if (create_build_dirs(out) != CP_TOOLS_OK) {
        return CP_TOOLS_ERROR_GENBOCA_FAILURE_TO_CREATE_BUILD_DIRECTORY;
    }

    if (create_description_dir(argc, argv, out, err) != CP_TOOLS_OK) {
        return CP_TOOLS_ERROR_GENBOCA_FAILURE_TO_CREATE_DESCRIPTION_DIRECTORY;
    }

    if (create_io_dir() != CP_TOOLS_OK) {
        return CP_TOOLS_ERROR_GENBOCA_FAILURE_TO_CREATE_IO_DIRECTORY;
    }

    return CP_TOOLS_OK;
}

// API functions
int run(int argc, char *const argv[], std::ostream &out, std::ostream &err) {
    int option = -1;

    // This command has many options, but only the `help` option is dealt with here.
    while ((option = getopt_long(argc, argv, "ho:c:lg:b:t", longopts, NULL)) != -1) {
        switch (option) {
        case 'h':
            cli::write(cli::message_type::none, help());
            return 0;
        }
    }

    return genboca(argc, argv, out, err);
}

} // namespace cptools::commands::genboca