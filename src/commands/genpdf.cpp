#include <cmath>
#include <dirent.h>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <unistd.h>

#include "commands/gentex.h"
#include "commands/init.h"
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
Generate a PDF file from the problem description. The options are:

    -b              Defines the problem label. The default value is 'A'.
    --label

    -c              Document class that will be used. The default value
    --class         is 'cp_modern'.

    -g              Problem language. The default value is 'pt_BR'.
    --lang

    -h              Displays this help.
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

namespace cptools::commands::genpdf {

typedef enum { NO_AUTHOR = 1000, NO_CONTEST } Value;

// Global variables
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
    return "Usage: " NAME " gentex [-h] [-o outfile] [-b label] [-c doc_class] "
           "[-g lang] [-l] [-t] [--no-author] [--no-contest]";
}

std::string help() { return usage() + help_message; }

int generate_pdf(const std::string &doc_class, const std::string &language, int flags,
                 const std::string &label, const std::string &outfile, bool tutorial,
                 std::ostream &out, std::ostream &err) {
    auto fs_res = fs::create_directory(CP_TOOLS_BUILD_DIR);
    if (not fs_res.ok) {
        err << message::failure(fs_res.error_message);
        return fs_res.rc;
    }

    // Generates the tex file that will be used to build the pdf file
    std::string texfile_path{std::string(CP_TOOLS_BUILD_DIR) +
                             (tutorial ? "/tutorial.tex" : "/problem.tex")};
    auto removed_result = fs::remove(texfile_path);
    if (not removed_result.ok) {
        err << message::failure(removed_result.error_message);
        return removed_result.rc;
    }

    std::ofstream tex_file(texfile_path);

    if (not tex_file) {
        err << message::failure("Error opening file '" + texfile_path) << "'\n";
        return CP_TOOLS_ERROR_GENPDF_INVALID_OUTFILE;
    }

    auto task = tutorial ? gentex::generate_tutorial_latex : gentex::generate_latex;
    auto rc = task(doc_class, language, flags, label, tex_file, err);

    if (rc != CP_TOOLS_OK) {
        err << message::failure("Error generating the LaTeX file '" + texfile_path) << "'\n";
        return rc;
    }

    tex_file.close();

    // Generates the PDF file 'problem.pdf' on CP_TOOLS_BUILD_DIR
    std::string pdf_file{std::string(CP_TOOLS_BUILD_DIR) +
                         (tutorial ? "/tutorial.pdf" : "/problem.pdf")};

    auto res = sh::build(pdf_file, texfile_path);

    if (res.rc != CP_TOOLS_OK) {
        err << message::failure("Error generating the PDF file '" + pdf_file + "'!") << "\n";
        err << message::trace(res.output) << '\n';
        return res.rc;
    }

    // Copy the generated PDF to the output file
    auto copy_res = fs::copy(pdf_file, outfile, true);
    if (not copy_res.ok) {
        err << message::failure(copy_res.error_message);
        return copy_res.rc;
    }

    out << message::success("File '" + outfile + "' generated.\n");

    return CP_TOOLS_OK;
}

// API functions
int run(int argc, char *const argv[], std::ostream &out, std::ostream &err) {
    int option = -1;
    bool tutorial = false;

    int flags = gentex::flag::INCLUDE_AUTHOR | gentex::flag::INCLUDE_CONTEST;

    std::string document_class{"cp_modern"}, outfile{"problem.pdf"}, language{"pt_BR"}, label{"A"};

    while ((option = getopt_long(argc, argv, "ho:c:lg:b:t", longopts, NULL)) != -1) {
        switch (option) {
        case 'h':
            out << help() << '\n';
            return 0;

        case 'b':
            label = std::string(optarg);
            break;

        case 'o':
            outfile = std::string(optarg);
            break;

        case 'c':
            document_class = std::string(optarg);
            break;

        case 'l':
            return gentex::list_document_classes(out, err);

        case 'g': {
            language = std::string(optarg);

            if (not gentex::validate_language(language)) {
                err << message::failure("Language '" + language + "' not find or supported\n");
                return CP_TOOLS_ERROR_GENPDF_INVALID_LANGUAGE;
            }

            break;
        }

        case 't':
            tutorial = true;
            outfile = "tutorial.pdf";
            break;

        case NO_AUTHOR:
            flags &= (~gentex::flag::INCLUDE_AUTHOR);
            break;

        case NO_CONTEST:
            flags &= (~gentex::flag::INCLUDE_CONTEST);
            break;

        default:
            err << help() << '\n';
            return CP_TOOLS_ERROR_GENPDF_INVALID_OPTION;
        }
    }

    return generate_pdf(document_class, language, flags, label, outfile, tutorial, out, err);
}

} // namespace cptools::commands::genpdf
