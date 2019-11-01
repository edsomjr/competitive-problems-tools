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

    -c              Selected the document class that will be used. The default value 
    --class         is 'cp_modern'

    -l              List all available document classes.
    --list
)message" };


namespace cptools::gentex {

    // Global variables
    static struct option longopts[] = {
       { "help", no_argument, NULL, 'h' },
       { "list", no_argument, NULL, 'l' },
       { "class", required_argument, NULL, 'c' },
       { "output", required_argument, NULL, 'o' },
       { 0, 0, 0, 0 }
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

    int generate_latex(const std::string& doc_class, std::ostream& out)
    {
        auto config = config::read("config.json");
 
        out << "\\documentclass{" << doc_class << "}\n\n";
        out << "\\event{" << config::get(config, "problem|contest", std::string()) << "}\n\n";
        out << "\\begin{document}\n\n";
        out << "Teste\n\n";
        out << "\\end{document}\n";

        return CP_TOOLS_OK;
    }

    // API functions
    int run(int argc, char * const argv[], std::ostream& out, std::ostream& err)
    {
        int option = -1;

        std::string document_class { "cp_modern" }, outfile;

        while ((option = getopt_long(argc, argv, "ho:c:l", longopts, NULL)) != -1)
        {
            switch (option) {
            case 'h':
                out << help() << '\n';
                return 0;

            case 'o':
                outfile = std::string(optarg);
                break;

            case 'c':
                document_class = std::string(optarg);
                break;

            case 'l':
                return list_document_classes(out, err);

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

            return generate_latex(document_class, of);
        }

        return generate_latex(document_class, out);
    }

}
