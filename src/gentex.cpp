#include <cmath>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <unistd.h>
#include <getopt.h>
#include <dirent.h>

#include "task.h"
#include "defs.h"
#include "dirs.h"
#include "init.h"
#include "util.h"
#include "error.h"
#include "config.h"


// Raw strings
static const std::string help_message {
R"message(
Generate a LaTeX file from the problem description. The options are:

    -h              Display this help.
    --help

    -o              Output file. If omitted, the output will be written on stdout.
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


namespace cptools::gentex {

    typedef enum { NO_AUTHOR = 1000, NO_CONTEST } Value;
    
    // Global variables
    static struct option longopts[] = {
        { "help", no_argument, NULL, 'h' },
        { "label", no_argument, NULL, 'b' },
        { "lang", no_argument, NULL, 'g' },
        { "list", no_argument, NULL, 'l' },
        { "class", required_argument, NULL, 'c' },
        { "output", required_argument, NULL, 'o' },
        { "no_author", no_argument, NULL, NO_AUTHOR },
        { "no_contest", no_argument, NULL, NO_CONTEST },
        { 0, 0, 0, 0 }
    };

    static const std::map<std::string, std::string> languages {
        { "en_US", "english" },
        { "pt_BR", "portuguese" },
    };

    struct Flags {
        bool include_author;
        bool include_contest;

        Flags() : include_author(true), include_contest(true) {}
    };

    // Auxiliary routines
    std::string usage()
    {
        return "Usage: " NAME " gentex [-h] [-o outfile] [-c doc_class] [-l list]";
    }

    std::string help()
    {
        return usage() + help_message;
    }

    int list_document_classes(std::ostream& out, std::ostream& err)
    {
        const std::string document_classes_dir { "/usr/local/lib/" NAME "/classes" };
        DIR *d = opendir(CP_TOOLS_CLASSES_DIR);

        if (d == nullptr)
        {
            err << "Directory '" << CP_TOOLS_CLASSES_DIR << "' does not exists\n";
            return CP_TOOLS_ERROR_GENTEX_LIST_DOCUMENT_CLASSES;
        }

        out << '\n';
        out << "    Class           Description\n\n";

        while (auto dir = readdir(d))
        {
            if (dir == nullptr)
                break;

            if (dir->d_name[0] == '.')
                continue;

            std::string filename { dir->d_name };
            auto tokens = split(filename, '.');

            if (tokens.size() != 2)
                continue;

            auto name = strip(tokens[0], '"');
            auto ext = strip(tokens[1], '"');

            if (ext != "cls" or name.empty() or name.front() == '.')
                continue;

            std::ifstream in(std::string(CP_TOOLS_CLASSES_DIR) + "/" + filename);
            std::string line;

            if (!in)
                continue;

            getline(in, line);

            if (line.empty())
                continue;

            // % is the LaTeX line comment char
            auto pos = line.find('%');

            if (pos == std::string::npos)
                continue;

            line = strip(line.substr(pos + 1));

            out << "    " << name;
            int count = 4 + name.size();

            while (count < 20)
            {
                out << ' ';
                ++count;
            }

            out << line << '\n';
        }

        out << '\n';

        return CP_TOOLS_OK;
    }

    int generate_latex(const std::string& doc_class, const std::string& language, 
        const Flags& flags, const std::string& label, std::ostream& out, std::ostream& err)
    {
        auto config = config::read("config.json");

        auto lang { languages.at(language) };
        auto event { config::get(config, "problem|contest", std::string()) }; 
        auto author { config::get(config, "problem|author", std::string()) }; 
        auto title { config::get(config, "problem|title|" + language, std::string("Título")) };
        auto timelimit { config::get(config, "problem|timelimit", 1.0) };
        int memorylimit = round(config::get(config, "problem|memory_limit", 256.0));

        auto c1_size = config::get(config, "PDF|first_column_size", std::string("6cm"));
        auto c2_size = config::get(config, "PDF|second_column_size", std::string("8cm"));

        if ((not flags.include_contest) or (not config::get(config, "PDF|include_contest", false)))
            event = "";

        if ((not flags.include_author) or (not config::get(config, "PDF|include_author", false)))
            author = "";

        out << "\\documentclass[" << lang << "]{" << doc_class << "}\n\n";
        out << "\\begin{document}\n\n";

        out << "\\header{" << event << "}{" << author << "}\n\n";

        out.precision(1);
        out << "\\begin{problem}{" << label << "}{" << title << "}{" << std::fixed 
            << (timelimit / 1000.0) << "}{" << memorylimit << "}\n\n";
        out << "\\input{tex/" << language << "/statement}\n\n";
        out << "\\begin{probleminput}{tex/" << language << "/input}\n";
        out << "\\end{probleminput}\n\n";
        out << "\\begin{problemoutput}{tex/" << language << "/output}\n";
        out << "\\end{problemoutput}\n\n";

        out << "\\begin{samples}{" << c1_size << "}{" << c2_size << "}\n";

        auto io_files = cptools::task::generate_io_files("samples", out, err);

        for (auto [infile, outfile] : io_files)
            out << "    \\iosample{" << c1_size << "}{" << c2_size << "}{" 
                << infile << "}{" << outfile << "}\n";

        out << "\\end{samples}\n\n";

        out << "\\begin{problemnotes}{tex/" << language << "/notes}\n";
        out << "\\end{problemnotes}\n\n";

        out << "\\trailer{" << event << "}{" << author << "}\n\n";

        out << "\\end{problem}\n";
        out << "\\end{document}\n";

        return CP_TOOLS_OK;
    }

    // API functions
    int run(int argc, char * const argv[], std::ostream& out, std::ostream& err)
    {
        int option = -1;

        std::string document_class { "cp_modern" }, outfile, language { "en_US" }, label { "A" };
        Flags flags;

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
                return list_document_classes(out, err);

            case 'g':
            {
                language = std::string(optarg);
                auto it = languages.find(language);

                if (it == languages.end())
                {
                    err << "Language " << language << " not find or supported\n";
                    return -1;
                }
        
                break;
            }

            case NO_AUTHOR:
                flags.include_author = false;
                break;

            case NO_CONTEST:
                flags.include_contest = false;
                break;

            default:
                err << help() << '\n';
                return CP_TOOLS_ERROR_GENTEX_INVALID_OPTION;
            }
        }

        if (not outfile.empty())
        {
            std::ofstream of(outfile);

            if (!of)
                return CP_TOOLS_ERROR_GENTEX_INVALID_OUTFILE;

            return generate_latex(document_class, language, flags, label, of, err);
        }

        return generate_latex(document_class, language, flags, label, out, err);
    }

}
