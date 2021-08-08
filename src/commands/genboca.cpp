#include <getopt.h>
#include <iostream>

#include "fs.h"
#include "error.h"
#include "dirs.h"
#include "message.h"
#include "config.h"
#include "commands/gentex.h"
#include "commands/genpdf.h"

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
int create_build_dirs(std::ostream &, std::ostream &err) {
    std::string boca_build_dir{CP_TOOLS_BOCA_BUILD_DIR};

    auto res_remove = fs::remove(boca_build_dir);

    if (not res_remove.ok) {
        err << message::failure(res_remove.error_message);
        return res_remove.rc;
    }

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

    // A label deveria tá na config.json ou deveria vim do argv?
    std::string label{"B"};

    // Qual o nome do PDF deve ser dado? Nos exemplos do Ribas é a letra da questão (B.pdf)
    std::string pdf_name{label + ".pdf"};

    // Opção 1: Usar o genpdf::run
    char *const argv[]{
                        (char *)"cp-tools",
                        (char *)"genpdf",
                        (char *)"-o",
                        (char *)".cp-build/boca/description/B.pdf"
                    };
    auto res_pdf = genpdf::run(4, argv, out, err);

    // Opção 2: Usar diretamente o genpdf::generate_pdf
    // std::string pdf_path{boca_desc_dir + pdf_name};
    // int flags = gentex::flag::INCLUDE_AUTHOR | gentex::flag::INCLUDE_CONTEST;
    // bool tutorial = false;
    // auto res_pdf = cptools::commands::genpdf::generate_pdf("cp_modern", "pt_BR", flags, label, pdf_path, tutorial, out, err);

    if(res_pdf != CP_TOOLS_OK) {
        return res_pdf;
    }

    std::string problem_info{boca_desc_dir + std::string("problem.info")};

    auto config = config::read_config_file();
    std::string default_title{"Título do problema"};
    // Sempre em português?
    std::string problem_fullname{util::get_json_value(config,
                                                      "problem|title|pt_BR",
                                                      default_title)};
    std::string content{
        std::string("basename=") + label + '\n' +
        std::string("fullname=") + '\"' + problem_fullname + "\"\n" +
        std::string("descfile=") + pdf_name + '\n'
    };

    fs::overwrite_file(problem_info, content);

    return CP_TOOLS_OK;
}

int genboca(const std::string &doc_class, const std::string &language, int flags,
            const std::string &label, const std::string &outfile, bool tutorial,
            std::ostream &out, std::ostream &err)
{
    if(create_build_dirs(out, err) != CP_TOOLS_OK) {
        return -1;
    }

    std::string a{doc_class}; a+="a";
    std::string b{language}; b+="b";
    std::string c{label}; c+="c";
    std::string d{outfile}; d+="d";
    std::string e{doc_class}; e+="e";
    flags++;
    tutorial &= false;

    // if(create_description_dir(out, err) != CP_TOOLS_OK) {
    //     // Qual erro deveria ser retornado aqui?
    //     return -1;
    // }

    // out << "Done" << std::endl;
    return CP_TOOLS_OK;
}



// API functions
int run(int argc, char *const argv[], std::ostream &out, std::ostream &err) {
    int option = -1;

    // tutorial faz sentido para o contexto do genboca?
    bool tutorial = false;

    int flags = gentex::flag::INCLUDE_AUTHOR | gentex::flag::INCLUDE_CONTEST;

    std::string document_class{"cp_modern"},
                outfile{"problem.pdf"},
                language{"pt_BR"},
                label{"A"};

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
            return CP_TOOLS_ERROR_CLEAN_INVALID_OPTION;
        }
    }

    return genboca(document_class, language, flags, label,
                        outfile, tutorial, out, err);
}

} // namespace cptools::commands::genboca