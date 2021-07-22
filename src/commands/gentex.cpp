#include <cmath>
#include <fstream>
#include <iostream>

#include <dirent.h>
#include <getopt.h>
#include <unistd.h>

#include "commands/gentex.h"
#include "commands/init.h"
#include "config.h"
#include "defs.h"
#include "dirs.h"
#include "error.h"
#include "message.h"
#include "task.h"
#include "util.h"

// Raw strings
static const std::string help_message{
    R"message(
Generate a LaTeX file from the problem description. The options are:

    -h              Displays this help.
    --help

    -o              Output file. If omitted, the output will be written on stdout.
    --output        

    -b              Defines the problem label. The default value is 'A'.
    --label         

    -c              Document class that will be used. The default value
    --class         is 'cp_modern'.

    -g              Problem language. The default value is 'en_US'.
    --lang

    -l              Lists all available document classes.
    --list

    -t              Generates LaTeX file for problem's tutorial.
    --tutorial

    --no_author     Omits problem's author.

    --no_contest    Omits problem's contest.

)message"};

namespace cptools::commands::gentex {

constexpr int NO_AUTHOR = 1000;
constexpr int NO_CONTEST = 2000;

// Global variables
static struct option longopts[] = {{"help", no_argument, NULL, 'h'},
                                   {"label", no_argument, NULL, 'b'},
                                   {"lang", no_argument, NULL, 'g'},
                                   {"list", no_argument, NULL, 'l'},
                                   {"class", required_argument, NULL, 'c'},
                                   {"output", required_argument, NULL, 'o'},
                                   {"tutorial", no_argument, NULL, 't'},
                                   {"no_author", no_argument, NULL, NO_AUTHOR},
                                   {"no_contest", no_argument, NULL, NO_CONTEST},
                                   {0, 0, 0, 0}};

static const std::map<std::string, std::string> languages{
    {"en_US", "english"},
    {"pt_BR", "portuguese"},
};

// Auxiliary routines
std::string usage() {
    return "Usage: " NAME " gentex [-h] [-o outfile] [-c doc_class] [-l list] "
           "[-g lang] [-b label] [--no_author] [--no_contest]";
}

std::string help() { return usage() + help_message; }

bool validate_language(const std::string &lang) { return languages.find(lang) != languages.end(); }

int list_document_classes(std::ostream &out, std::ostream &err) {
    std::string classes_dir{CP_TOOLS_CLASSES_DIR};
    DIR *d = opendir(classes_dir.c_str());

    if (d == nullptr) {
        err << message::failure("Directory '" + classes_dir + "' does not exist\n");
        return CP_TOOLS_ERROR_GENTEX_LIST_DOCUMENT_CLASSES;
    }

    out << '\n';
    out << message::header("    Class           Description\n\n");

    while (auto dir = readdir(d)) {
        if (dir == nullptr)
            break;

        if (dir->d_name[0] == '.')
            continue;

        std::string filename{dir->d_name};
        auto tokens = util::split(filename, '.');

        if (tokens.size() != 2)
            continue;

        auto name = util::strip(tokens[0], '"');
        auto ext = util::strip(tokens[1], '"');

        if (ext != "cls" or name.empty() or name.front() == '.')
            continue;

        std::ifstream in(classes_dir + "/" + filename);
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

        line = util::strip(line.substr(pos + 1));

        out << message::info("    " + name);
        int count = 4 + name.size();

        while (count < 20) {
            out << ' ';
            ++count;
        }

        out << message::info(line) << '\n';
    }

    out << '\n';

    return CP_TOOLS_OK;
}

int generate_tutorial_latex(const std::string &doc_class, const std::string &language, int flags,
                            const std::string &label, std::ostream &out, std::ostream &) {
    auto config = config::read_config_file();

    auto lang{languages.at(language)};
    auto event{util::get_json_value(config, "problem|contest", std::string())};
    auto author{util::get_json_value(config, "problem|author", std::string())};
    auto title{util::get_json_value(config, "problem|title|" + language, std::string("Título"))};

    if ((not(flags & flag::INCLUDE_CONTEST)) or
        (not util::get_json_value(config, "PDF|include_contest", false)))
        event = "";

    if ((not(flags & flag::INCLUDE_AUTHOR)) or
        (not util::get_json_value(config, "PDF|include_author", false)))
        author = "";

    out << "\\documentclass[" << lang << "]{" << doc_class << "}\n\n";
    out << "\\usepackage{amsmath}\n";
    out << "\\usepackage{tikz}\n\n";

    out << "\\newcommand{\\Mod}[1]{\\ (\\mathrm{mod}\\ #1)}\n\n";

    out << "\\begin{document}\n\n";

    out << "\\header{" << event << "}{" << author << "}\n\n";

    out << "\\begin{flushleft}\n";
    out << "\\textbf{\\Large{" << label << ". " << title << "}}\n";
    out << "\\end{flushleft}\n";

    out << "\\vspace{0.2in}\n";

    out << "\\input{tex/" << language << "/tutorial}\n\n";

    out << "\\trailer{" << event << "}{" << author << "}\n\n";

    out << "\\end{document}\n";

    return CP_TOOLS_OK;
}

int generate_latex(const std::string &doc_class, const std::string &language, int flags,
                   const std::string &label, std::ostream &out, std::ostream &err) {
    auto config = config::read_config_file();

    auto lang{languages.at(language)};
    auto event{util::get_json_value(config, "problem|contest", std::string())};
    auto author{util::get_json_value(config, "problem|author", std::string())};
    auto title{util::get_json_value(config, "problem|title|" + language, std::string("Título"))};
    auto timelimit{util::get_json_value(config, "problem|timelimit", 1.0)};
    int memorylimit = round(util::get_json_value(config, "problem|memory_limit", 256.0));

    auto c1_size = util::get_json_value(config, "PDF|first_column_size", std::string("6cm"));
    auto c2_size = util::get_json_value(config, "PDF|second_column_size", std::string("8cm"));

    if ((not(flags & flag::INCLUDE_CONTEST)) or
        (not util::get_json_value(config, "PDF|include_contest", false)))
        event = "";

    if ((not(flags & flag::INCLUDE_AUTHOR)) or
        (not util::get_json_value(config, "PDF|include_author", false)))
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

    auto io_files = task::generate_io_files("samples", out, err);

    for (auto [infile, outfile] : io_files)
        out << "    \\iosample{" << c1_size << "}{" << c2_size << "}{" << infile << "}{" << outfile
            << "}\n";

    out << "\\end{samples}\n\n";

    auto notes{
        util::get_json_value(config, "problem|notes", std::vector<std::string>{"Notas", "Notes"})};

    out << "\% notes: ";
    for (auto x : notes)
        out << x << " ";
    out << '\n';

    if (notes.front() == "Notas") {
        out << "\\begin{problemnotes}{tex/" << language << "/notes}\n";
        out << "\\end{problemnotes}\n\n";
    } else {
        out << "\\begin{customnotes}{" << notes.front() << "}{" << notes.back() << "}{tex/"
            << language << "/notes}\n";
        out << "\\end{customnotes}\n\n";
    }

    out << "\\trailer{" << event << "}{" << author << "}\n\n";

    out << "\\end{problem}\n";
    out << "\\end{document}\n";

    return CP_TOOLS_OK;
}

// API functions
int run(int argc, char *const argv[], std::ostream &out, std::ostream &err) {
    int option = -1;

    std::string document_class{"cp_modern"}, outfile, language{"pt_BR"}, label{"A"};
    int flags = flag::INCLUDE_AUTHOR | flag::INCLUDE_CONTEST;

    auto f = generate_latex;

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
            return list_document_classes(out, err);

        case 'g': {
            language = std::string(optarg);

            if (not validate_language(language)) {
                err << message::failure("Language " + language + " not find or supported\n");
                return -1;
            }

            break;
        }

        case 't':
            f = generate_tutorial_latex;
            break;

        case NO_AUTHOR:
            flags &= (~flag::INCLUDE_AUTHOR);
            break;

        case NO_CONTEST:
            flags &= (~flag::INCLUDE_CONTEST);
            break;

        default:
            err << help() << '\n';
            return CP_TOOLS_ERROR_GENTEX_INVALID_OPTION;
        }
    }

    if (not outfile.empty()) {
        std::ofstream of(outfile);

        if (!of)
            return CP_TOOLS_ERROR_GENTEX_INVALID_OUTFILE;

        return f(document_class, language, flags, label, of, err);
    }

    return f(document_class, language, flags, label, out, err);
}

} // namespace cptools::commands::gentex
