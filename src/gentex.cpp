#include <iostream>
#include <filesystem>

#include <unistd.h>
#include <getopt.h>

#include "defs.h"
#include "init.h"
#include "error.h"


static struct option longopts[] = {
   { "help", no_argument, NULL, 'h' },
   { "version", no_argument, NULL, 'v' },
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

    -c              Selected the document class that will be used. The options
    --class         are: default, UVA.
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

    while ((option = getopt_long(argc, argv, "h", longopts, NULL)) != -1)
    {
        switch (option) {
        case 'h':
            std::cout << help() << '\n';
            return 0;

        default:
            std::cout << help() << '\n';
            return CP_TOOLS_ERROR_GENTEX_INVALID_OPTION;
        }
    }

    auto tex = generate_latex();

    return CP_TOOLS_OK;
}
