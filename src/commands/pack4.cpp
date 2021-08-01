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
#include "commands/genpdf.h"

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

/**
 * @brief Function that initializes the Boca package build directory. This function will
 *        copy the boca package template files to a problem build directory
 *
 * @param out
 * @param err
 * @return int Returns CP_TOOLS_OK if everything goes as expected, or returns a specific
 *             error code if a problem occurs.
 */
int create_build_dirs(std::ostream &err) {
    std::string boca_build_dir{CP_TOOLS_BOCA_BUILD_DIR};
    auto res_create = fs::create_directory(boca_build_dir);

    if (not res_create.ok) {
        err << message::failure(res_create.error_message);
        return res_create.rc;
    }

    auto res_copy = cptools::fs::copy(CP_TOOLS_BOCA_TEMPLATES_DIR, boca_build_dir, true);

    if (not res_copy.ok)
        err << message::failure(res_copy.error_message) << "\n";

    return res_copy.rc;
}

int create_description_dir(std::ostream &out, std::ostream &err) {
    std::string boca_desc_dir{CP_TOOLS_BOCA_BUILD_DIR  + std::string("description/")};

    // Qual o nome do PDF deve ser dado? Nos exemplos do ribas é a letra da questão.
    std::string pdf_name{"B.pdf"};
    std::string opt_pdf_path{"--output=" + boca_desc_dir + pdf_name};

    // Here I am using a CLI interface of the genpdf command to generate the PDF of the Boca package
    char *argv[] = { (char*)"cp-tools",
                     (char*)"genpdf",
                     (char*) opt_pdf_path.c_str(),
                     NULL
                    };
    auto res_pdf = genpdf::run(3, argv, out, err);

    if(res_pdf != CP_TOOLS_OK) {
        return res_pdf;
    }

    std::string problem_description{boca_desc_dir + std::string("desc.txt")};

    // TODO: Como eu vou gerar um arquivo txt com os arquivos latex??

    std::string problem_info{boca_desc_dir + std::string("problem.info")};

    std::string problem_basename = "B";
    std::string problem_fullname = "Beautiful Mountains";

    std::string content{
        std::string("basename=") + problem_basename + '\n' +
        std::string("fullname=") + '\"' + problem_fullname + "\"\n" +
        std::string("descfile=") + pdf_name + '\n'
    };

    fs::overwrite_file(problem_info, content);

    return CP_TOOLS_OK;
}

int pack4_boca(std::ostream &out, std::ostream &err) {

    // Retornar int ou cptools::fs::Result?
    // Se falhar deveriámos abortar o programa?
    // Aqui não estou respeitando o padrão de sempre passar o out e o err
    // Na função não uso o out, e isso gera um -Werror=unused-parameter no build
    if(create_build_dirs(err) != CP_TOOLS_OK) {
        // Qual erro deveria ser retornado aqui?
        return -1;
    }

    if(create_description_dir(out, err) != CP_TOOLS_OK) {
        // Qual erro deveria ser retornado aqui?
        return -1;
    }

    out << "Done" << std::endl;
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
