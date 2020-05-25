#include <cmath>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <unistd.h>
#include <getopt.h>
#include <dirent.h>

#include "sh.h"
#include "task.h"
#include "defs.h"
#include "dirs.h"
#include "init.h"
#include "util.h"
#include "error.h"
#include "config.h"
#include "gentex.h"


// Raw strings
static const std::string help_message {
R"message(
Generate a PDF file from the problem description. The options are:

    -h              Display this help.
    --help

    -o              Output file. If omitted, the output will be the file 'problem.pdf'.
    --output        

    -b              Define the problem label. The default value is 'A'.
    --label         

    -c              Selected the document class that will be used. The default value 
    --class         is 'cp_modern'.

    -g              Define the problem language. The default value is 'en_US'.
    --lang

    -l              List all available document classes.
    --list

    --no_author     Don't show problem's author.

    --no_contest    Don't show problem's contest.

)message" };


namespace cptools::genpdf {

    typedef enum { NO_AUTHOR = 1000, NO_CONTEST } Value;

    // Global variables
    static struct option longopts[] = {
        { "help", no_argument, NULL, 'h' },
        { "label", required_argument, NULL, 'b' },
        { "lang", required_argument, NULL, 'g' },
        { "list", no_argument, NULL, 'l' },
        { "class", required_argument, NULL, 'c' },
        { "output", required_argument, NULL, 'o' },
        { "no_author", no_argument, NULL, NO_AUTHOR },
        { "no_contest", no_argument, NULL, NO_CONTEST },
        { 0, 0, 0, 0 }
    };

    // Auxiliary routines
    std::string usage()
    {
        return "Usage: " NAME " gentex [-h] [-o outfile] [-b label] [-c doc_class] [-g lang] [-l] [--no-author] [--no-contest]";
    }

    std::string help()
    {
        return usage() + help_message;
    }


    int generate_pdf(const std::string& doc_class, const std::string& language, 
        int flags, const std::string& label, const std::string& outfile, 
        std::ostream& out, std::ostream& err)
    {
        auto rc = sh::make_dir(CP_TOOLS_BUILD_DIR);

        if (rc != CP_TOOLS_OK)
        {
            err << "[genpdf] Error creating dir '" << CP_TOOLS_BUILD_DIR << "'";
            return rc;
        }

        // Generates the tex file that will be used to build the pdf file
        std::string texfile_path { std::string(CP_TOOLS_BUILD_DIR) + "/problem.tex" };
        std::ofstream tex_file(texfile_path);

        if (not tex_file)
        {
            err << "[genpdf] Error opening file '" << texfile_path << "'\n";
            return CP_TOOLS_ERROR_GENPDF_INVALID_OUTFILE;
        }

        rc = gentex::generate_latex(doc_class, language, flags, label, tex_file, err);

        if (rc != CP_TOOLS_OK)
        {
            err << "[genpdf] Error generating the LaTeX file '" << texfile_path << "'\n";
            return rc;
        }

        tex_file.close();

        // Generates the PDF file 'problem.pdf' on CP_TOOLS_BUILD_DIR
        std::string pdf_file { std::string(CP_TOOLS_BUILD_DIR) + "/problem.pdf" };

        rc = sh::build(CP_TOOLS_BUILD_DIR, texfile_path);

        if (rc != CP_TOOLS_OK)
        {
            err << "[genpdf] Error generating the PDF file '" << pdf_file << "'\n";
            return rc;
        }

        // Copy the generated PDF to the output file
        rc = sh::copy_file(outfile, pdf_file);

        if (rc != CP_TOOLS_OK)
        {
            err << "[genpdf] Error copying PDF file '" << pdf_file << "' to '" << outfile << "'\n";
            return CP_TOOLS_ERROR_GENPDF_INVALID_OUTFILE;
        }

        out << "[genpdf] File '" << outfile << "' generated.\n";

        return CP_TOOLS_OK;
    }

    // API functions
    int run(int argc, char * const argv[], std::ostream& out, std::ostream& err)
    {
        int option = -1;

        std::string document_class { "cp_modern" }, outfile { "problem.pdf" }, 
            language { "en_US" }, label { "A" };
        int flags = gentex::Flags::INCLUDE_AUTHOR | gentex::Flags::INCLUDE_CONTEST;

        while ((option = getopt_long(argc, argv, "ho:c:lg:b:", longopts, NULL)) != -1)
        {
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

            case 'g':
            {
                language = std::string(optarg);

                if (not gentex::validate_language(language))
                {
                    err << "Language " << language << " not find or supported\n";
                    return -1;
                }
        
                break;
            }

            case NO_AUTHOR:
                flags &= (~gentex::Flags::INCLUDE_AUTHOR);
                break;

            case NO_CONTEST:
                flags &= (~gentex::Flags::INCLUDE_CONTEST);
                break;

            default:
                err << help() << '\n';
                return CP_TOOLS_ERROR_GENPDF_INVALID_OPTION;
            }
        }

        return generate_pdf(document_class, language, flags, label, outfile, out, err);
    }

}
