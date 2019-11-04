#include <cmath>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <unistd.h>
#include <getopt.h>

#include "defs.h"
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
)message" };


namespace cptools::gentex {

    // Global variables
    static struct option longopts[] = {
        { "help", no_argument, NULL, 'h' },
        { "label", no_argument, NULL, 'b' },
        { "lang", no_argument, NULL, 'g' },
        { "list", no_argument, NULL, 'l' },
        { "class", required_argument, NULL, 'c' },
        { "output", required_argument, NULL, 'o' },
        { 0, 0, 0, 0 }
    };

    static const std::map<std::string, std::string> languages {
        { "en_US", "english" },
        { "pt_BR", "portuguese" },
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

        out << '\n';

        try {
            for (const auto& it : std::filesystem::directory_iterator(document_classes_dir))
            {
                if (not it.is_regular_file())
                    continue;

                auto filename = it.path().filename(); 
                auto tokens = split(filename, '.');

                if (tokens.size() != 2)
                    continue;

                auto name = strip(tokens[0], '"');
                auto ext = strip(tokens[1], '"');

                if (ext != "cls" or name.empty() or name.front() == '.')
                    continue;

                std::ifstream in(it.path().string());
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
        } catch (const std::exception& e)
        {
            err << e.what() << '\n';
            return CP_TOOLS_ERROR_GENTEX_LIST_DOCUMENT_CLASSES;
        }

        out << '\n';

        return CP_TOOLS_OK;
    }

    int generate_latex(const std::string& doc_class, const std::string& language,
        const std::string& label, std::ostream& out)
    {
        auto config = config::read("config.json");

        auto lang = languages.at(language);
        auto event { config::get(config, "problem|contest", std::string()) }; 
        auto title = config::get(config, "problem|title|" + language, std::string("Título"));
        auto timelimit = config::get(config, "problem|timelimit", 1.0);
        int memorylimit = round(config::get(config, "problem|memory_limit", 256.0));

        auto c1_size = config::get(config, "PDF|first_column_size", std::string("6cm"));
        auto c2_size = config::get(config, "PDF|second_column_size", std::string("8cm"));

        out << "\\documentclass[" << lang << "]{" << doc_class << "}\n\n";
        out << "\\event{" << event << "}\n\n";
        out << "\\begin{document}\n\n";

        out.precision(1);
        out << "\\begin{problem}{" << label << "}{" << title << "}{" << std::fixed 
            << (timelimit / 1000.0) << "}{" << memorylimit << "}\n\n";
        out << "\\input{tex/" << language << "/statement}\n\n";
        out << "\\begin{probleminput}{tex/" << language << "/input}\n";
        out << "\\end{probleminput}\n\n";
        out << "\\begin{problemoutput}{tex/" << language << "/output}\n";
        out << "\\end{problemoutput}\n\n";

        out << "\\begin{samples}{" << c1_size << "}{" << c2_size << "}\n";

        for (auto io : config::get(config, "tests|samples", std::vector<std::string> {}))
            out << "    \\iosample{" << c1_size << "}{" << c2_size << "}{" 
                << io << "}{" << io << "}\n";

        out << "\\end{samples}\n\n";

        out << "\\begin{problemnotes}{tex/" << language << "/notes}\n";
        out << "\\end{problemnotes}\n\n";

        out << "\\end{problem}\n";
        out << "\\end{document}\n";

        return CP_TOOLS_OK;
    }

    // API functions
    int run(int argc, char * const argv[], std::ostream& out, std::ostream& err)
    {
        int option = -1;

        std::string document_class { "cp_modern" }, outfile, language { "en_US" }, label { "A" };

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

            return generate_latex(document_class, language, label, of);
        }

        return generate_latex(document_class, language, label, out);
    }

}
