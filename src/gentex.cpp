#include <iostream>
#include <filesystem>

#include <unistd.h>
#include <getopt.h>

#include "defs.h"
#include "init.h"
#include "error.h"


static struct option longopts[] = {
   { "help", no_argument, NULL, 'h' },
   { "input", required_argument, NULL, 'i' },
   { "output", required_argument, NULL, 'o' },
   { "class", required_argument, NULL, 'c' },
   { 0, 0, 0, 0 }
};

static std::string usage()
{
    return "Usage: " NAME " gentex [-h] [-i input_file] [-o output_file] [-c document_class]";
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

    -c              Selected the document class that will be used. The default value is TEP.
    --class

    -l              List all available document classes.
    --list
)message" };

    return usage() + message;
}

std::string generate_latex()
{
    return "LaTeX test";
}

int gentex(int argc, char * const argv[])
{
    int option = -1;
    std::string infile { "problem.md" }, outfile, document_class { "TEP" };

    while ((option = getopt_long(argc, argv, "hi:o:c:", longopts, NULL)) != -1)
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

        default:
            std::cout << help() << '\n';
            return CP_TOOLS_ERROR_GENTEX_INVALID_OPTION;
        }
    }

//    std::cout << "infile = [" << infile << "]\n";
//    std::cout << "outfile = [" << outfile << "]\n";
//    std::cout << "document_class = [" << document_class << "]\n";

    auto tex = generate_latex();

    return CP_TOOLS_OK;
}
