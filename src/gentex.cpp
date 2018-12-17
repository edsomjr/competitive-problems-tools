#include <fstream>
#include <iostream>
#include <filesystem>

#include <unistd.h>
#include <getopt.h>

#include "defs.h"
#include "init.h"
#include "util.h"
#include "error.h"


static struct option longopts[] = {
   { "help", no_argument, NULL, 'h' },
   { "list", no_argument, NULL, 'l' },
   { "input", required_argument, NULL, 'i' },
   { "class", required_argument, NULL, 'c' },
   { "output", required_argument, NULL, 'o' },
   { "metadata", required_argument, NULL, 'm' },
   { 0, 0, 0, 0 }
};

static std::string usage()
{
    return "Usage: " NAME " gentex [-h] [-i infile] [-o outfile] [-c doc_class] [-l list] [-m metadata]";
}

static std::string help()
{
    const std::string message {
R"message(
Generate a LaTeX file from the problem description. The options are:

    -h              Display this help.
    --help

    -i              Input file with a problem description in Markdown notation. 
    --input         The default is 'problem.md'

    -o              Output file. If omitted, the output will be written on
    --output        stdout.

    -c              Selected the document class that will be used. The default value is 'TEP'.
    --class

    -l              List all available document classes.
    --list

    -m              Metadata file. The default value is 'config.json'.
    --metadata
)message" };

    return usage() + message;
}

static int list_document_classes()
{
    const std::string document_classes_dir { "/usr/local/lib/" NAME "/classes" };

    std::cout << '\n';

    try {
        for (const auto& it : std::filesystem::directory_iterator(document_classes_dir))
        {
            if (not it.is_regular_file())
                continue;

            auto filename = it.path().filename(); 
            auto tokens = split(filename, '.');

            if (tokens.size() < 2)
                continue;

            auto name = strip(tokens[0], '"');
            auto ext = strip(tokens[1], '"');

            if (ext != "cls" or name.empty())
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

            std::cout << "    " << name;
            std::cout << (name.size() < 8 ? "\t\t" : "\t") << line << '\n';
        }
    } catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return CP_TOOLS_ERROR_GENTEX_LIST_DOCUMENT_CLASSES;
    }

    std::cout << '\n';

    return CP_TOOLS_OK;
}

std::string generate_latex()
{
    return "LaTeX test";
}

int gentex(int argc, char * const argv[])
{
    int option = -1;
    std::string infile { "problem.md" }, document_class { "TEP" }, metadata { "config.json" },
        outfile;

    while ((option = getopt_long(argc, argv, "hi:o:c:lm:", longopts, NULL)) != -1)
    {
        switch (option) {
        case 'h':
            std::cout << help() << '\n';
            return 0;

        case 'i':
            infile = std::string(optarg);
            break;

        case 'o':
            outfile = std::string(optarg);
            break;

        case 'c':
            document_class = std::string(optarg);
            break;

        case 'm':
            metadata = std::string(optarg);
            break;

        case 'l':
            return list_document_classes();

        default:
            std::cout << help() << '\n';
            return CP_TOOLS_ERROR_GENTEX_INVALID_OPTION;
        }
    }

//    std::cout << "infile = [" << infile << "]\n";
//    std::cout << "outfile = [" << outfile << "]\n";
//    std::cout << "document_class = [" << document_class << "]\n";
    std::cout << "metadata = [" << metadata << "]\n";

    auto tex = generate_latex();

    return CP_TOOLS_OK;
}
