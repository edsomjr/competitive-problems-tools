#include <iostream>

#include "fs.h"
#include "error.h"
#include "dirs.h"
#include "message.h"
#include "config.h"
#include "commands/genpdf.h"

namespace cptools::commands::pack4 {

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

    // A label deveria tá na config.json ou deveria vim do argv?
    std::string label{"B"};

    // Qual o nome do PDF deve ser dado? Nos exemplos do Ribas é a letra da questão (B.pdf)
    std::string pdf_name{label + ".pdf"};

    // Opção 1: Usar o genpdf::run
    char *argv[] = { (char*)"cp-tools",
                     (char*)"genpdf",
                     (char*)"--output=.cp-build/boca/description/B.pdf",
                     NULL
                    };
    auto res_pdf = genpdf::run(3, argv, out, err);

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

int genboca(std::ostream &out, std::ostream &err) {

    // Retornar int ou cptools::fs::Result?
    // Se falhar deveriámos abortar o programa?
    // Aqui não estou respeitando o padrão de sempre passar o out e o err, pois na função
    // não é usado o out, e isso gera um -Werror=unused-parameter no build
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

} // namespace cptools::commands::pack4