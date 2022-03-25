#include <fstream>
#include <sstream>
#include <iostream>

#include "fs.h"
#include "dirs.h"
#include "utils.h"
#include "error.h"
#include "config.h"
#include "plugin.h"
#include "message.h"

class GenTex : public Plugin {
public:
    GenTex();

    int execute(const Args& args);
};

GenTex::GenTex() : Plugin("gentex", "Generate a LaTeX file from the problem description.", "")
{
    _description = _brief;

    _options.emplace_back('h', "help", false, "", "Generates this help message.");
    _options.emplace_back('o', "output", true, "output_file", "Output file. If omitted, the output will be written on stdout.");
    _options.emplace_back('b', "label", true, "label", "Defines the problem label. The default value is 'A'.");
    _options.emplace_back('c', "class", true, "class", "Document class that will be used. The default valueis 'cp_modern'.");
    _options.emplace_back('g', "lang", true, "lang", "Problem language. The default value is 'pt_BR'.");
    _options.emplace_back('l', "list", false, "list", "Lists all available document classes.");
    _options.emplace_back('t', "tutorial", false, "tutorial", "Generates LaTeX file for problem's tutorial.");
    _options.emplace_back(-1, "no_author", false, "no_author", "Omits problem's author.");
    _options.emplace_back(-2, "no_contest", false, "no_contest", "Omits problem's contest.");
}

static const std::map<std::string, std::string> languages
{
    {"en_US", "english"},
    {"pt_BR", "portuguese"},
};

std::string
generate_latex(const std::string &doc_class, const std::string &language, const std::string &label, bool no_author, bool no_contest)
{
    std::ostringstream oss;
    std::string empty;

    auto config = cptools::config::read_config_file();

    auto event { cptools::utils::get_json_value(config, "problem|contest", empty) };
    auto author { cptools::utils::get_json_value(config, "author|name", empty) };
    auto title { cptools::utils::get_json_value(config, "problem|title|" + language, std::string("Título")) };
    auto time_limit { cptools::utils::get_json_value(config, "problem|time_limit", 1000) };
    auto memorylimit { round(cptools::utils::get_json_value(config, "problem|memory_limit", 256.0)) };

    auto c1_size { cptools::utils::get_json_value(config, "PDF|first_column_size", std::string("6cm")) };
    auto c2_size { cptools::utils::get_json_value(config, "PDF|second_column_size", std::string("8cm")) };

    if (no_contest or (not cptools::utils::get_json_value(config, "PDF|include_contest", false)))
        event = empty;

    if (no_author or (not cptools::utils::get_json_value(config, "PDF|include_author", false)))
        author = empty;

    oss << "\\documentclass[" << languages.at(language) << "]{" << doc_class << "}\n\n";
    oss << "\\begin{document}\n\n";

    oss << "\\header{" << event << "}{" << author << "}\n\n";

    oss.precision(1);
    oss << "\\begin{problem}{" << label << "}{" << title << "}{" << std::fixed << (time_limit / 1000.0) << "}{" << memorylimit << "}\n\n";

    oss << "\\input{tex/" << language << "/statement}\n\n";
    oss << "\\begin{probleminput}{tex/" << language << "/input}\n";
    oss << "\\end{probleminput}\n\n";
    oss << "\\begin{problemoutput}{tex/" << language << "/output}\n";
    oss << "\\end{problemoutput}\n\n";

    oss << "\\begin{samples}{" << c1_size << "}{" << c2_size << "}\n";

/*
    auto io_files = task::generate_io_files(config::test_type::sample, true);

    auto varsample = cptools::utils::get_json_value(config, "PDF|varsamples", false);
    std::string sample_command { varsample ? "\\variosample{" : "\\iosample{" };

    for (auto [infile, outfile] : io_files)
        oss << "    " << sample_command << c1_size << "}{" << c2_size << "}{" << infile << "}{"
                   << outfile << "}\n";

    oss << "\\end{samples}\n\n";

    auto notes{
        cptools::utils::get_json_value(config, "problem|notes", std::vector<std::string>{"Notas", "Notes"})};

    oss << "\% notes: ";
    for (auto x : notes)
        oss << x << " ";
    oss << '\n';

    if (notes.front() == "Notas") {
        oss << "\\begin{problemnotes}{tex/" << language << "/notes}\n";
        oss << "\\end{problemnotes}\n\n";
    } else {
        oss << "\\begin{customnotes}{" << notes.front() << "}{" << notes.back() << "}{tex/"
                   << language << "/notes}\n";
        oss << "\\end{customnotes}\n\n";
    }

    oss << "\\trailer{" << event << "}{" << author << "}\n\n";

    oss << "\\end{problem}\n";
    oss << "\\end{document}\n";
*/
    return oss.str();
}

int
GenTex::execute(const Args& args)
{
    auto doc_class = args.count("class") ? args.at("class") : std::string("cp_modern");
    auto language = args.count("language") ? args.at("language") : std::string("pt_BR");
    auto label = args.count("label") ? args.at("label") : std::string("A");
    bool no_author = args.count("no_autor");
    bool no_contest = args.count("no_contest");

    if (languages.count(language) == 0)
    {
        std::cout << cptools::message::error(std::string("Invalid language: ") + language);
        exit(CP_TOOLS_ERROR_GENTEX_INVALID_LANGUAGE);
    }

    auto latex = generate_latex(doc_class, language, label, no_author, no_contest);

    if (args.count("output"))
    {
        std::ofstream out(args.at("output"));

        out << latex;
        out.close();
    } else
        std::cout << latex;

    return CP_TOOLS_OK;
}

// dynamic function that is called by the plugin manager to build the plugin
extern "C" Plugin *
create()
{
    return new GenTex();
}

// dynamic function that is called by the plugin manager to destroy the plugin
extern "C" void
destroy(Plugin *p)
{
    delete p;
}
