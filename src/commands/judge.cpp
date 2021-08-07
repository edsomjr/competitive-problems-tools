#include <iostream>
#include <map>

#include <getopt.h>
#include <unistd.h>

#include "commands/clean.h"
#include "commands/judge.h"
#include "config.h"
#include "defs.h"
#include "dirs.h"
#include "error.h"
#include "logger/format.h"
#include "logger/logger.h"
#include "logger/message.h"
#include "sh.h"
#include "table.h"
#include "task.h"
#include "util.h"

// Raw strings
static const std::string help_message{
    R"message(
Runs a solution against all test sets and gives you a veredict.

    Option          Description

    -h              Generates this help message.
    --help

)message"};

namespace cptools::commands::judge {

// Constantes
namespace verdict {
const int AC = 0;
const int PE = 1;
const int WA = 2;
const int TLE = 3;
const int MLE = 4;
const int RTE = 5;
const int UNDEF = 6;
const int CE = 7;
const int FAIL = 8;
} // namespace verdict

// Global variables
static struct option longopts[] = {{"help", no_argument, NULL, 'h'}, {0, 0, 0, 0}};

std::map<int, std::string> ver_string{
    {verdict::AC, "Accepted"},
    {verdict::PE, "Presentation Error"},
    {verdict::WA, "Wrong Answer"},
    {verdict::CE, "Compilation Error"},
    {verdict::TLE, "Time Limit Exceeded"},
    {verdict::RTE, "Runtime Error"},
    {verdict::MLE, "Memory Limit Exceeded"},
    {verdict::FAIL, "Failure"},
    {verdict::UNDEF, "Undefined Error"},
};

const std::map<int, long long> ver_style{
    {verdict::AC, logger::format::style::AC},       {verdict::PE, logger::format::style::PE},
    {verdict::WA, logger::format::style::WA},       {verdict::CE, logger::format::style::CE},
    {verdict::TLE, logger::format::style::TLE},     {verdict::RTE, logger::format::style::RTE},
    {verdict::MLE, logger::format::style::MLE},     {verdict::FAIL, logger::format::style::FAIL},
    {verdict::UNDEF, logger::format::style::UNDEF},
};

// Auxiliary routines
std::string usage() { return "Usage: " NAME " problem judge solution.[cpp|c|java|py]"; }

std::string help() { return usage() + help_message; }

int judge(const std::string &solution_path, std::ostream &out) {
    table::Table report{{
        {"#", 4, logger::format::align::RIGHT | logger::format::emph::BOLD},
        {"Verdict", 32, logger::format::align::LEFT | logger::format::emph::BOLD},
        {"Time (s)", 12, logger::format::align::RIGHT | logger::format::emph::BOLD},
        {"Memory (MB)", 12, logger::format::align::RIGHT | logger::format::emph::BOLD},
    }};

    logger::log(logger::INFO, "Judging solution '" + solution_path + "'...");

    // Constrói as ferramentas necessárias
    std::string error;
    auto rc = task::build_tools(error, task::tools::VALIDATOR | task::tools::CHECKER);

    if (rc != CP_TOOLS_OK) {
        logger::log(logger::ERROR, "Can't build the required tools");
        logger::log(logger::TRACE, error);
        return CP_TOOLS_ERROR_JUDGE_MISSING_TOOL;
    }

    // Gera o executável da solução
    rc = task::gen_exe(error, solution_path, "sol");

    if (rc != CP_TOOLS_OK) {
        logger::log(logger::ERROR, "Error on solution '" + solution_path + "' compilation");
        logger::log(logger::TRACE, error);
        return verdict::CE;
    }

    auto config = cptools::config::read_config_file();
    auto timelimit = cptools::util::get_json_value(config, "problem|timelimit", 1000);
    auto memory_limit = cptools::util::get_json_value(config, "problem|memory_limit", 1000);

    auto checker{std::string(CP_TOOLS_BUILD_DIR) + "/checker"};
    auto program{std::string(CP_TOOLS_BUILD_DIR) + "/sol"};
    auto validator{std::string(CP_TOOLS_BUILD_DIR) + "/validator"};

    auto files = task::generate_io_files("all", out);
    int ans = verdict::AC, passed = 0;
    double tmax = 0.0, mmax = 0.0;

    auto as_string = [](double x, int places) {
        char buffer[64];
        sprintf(buffer, "%.*f", places, x);

        return std::string(buffer);
    };

    for (auto [input, answer] : files) {
        auto number = util::split(input, '/').back();
        auto output{std::string(CP_TOOLS_BUILD_DIR) + "/out"};

        auto res = sh::execute(validator, "", input);

        if (res.rc != CP_TOOLS_OK) {
            logger::log(logger::ERROR, "Input file '" + input + "' is invalid");
            logger::log(logger::TRACE, res.output);
            return CP_TOOLS_ERROR_JUDGE_INVALID_INPUT_FILE;
        }

        auto info = sh::profile(program, "", 2 * timelimit / 1000.0, input, output);

        int ver = verdict::AC;

        if (info.rc != CP_TOOLS_OK)
            ver = verdict::RTE;

        if (info.elapsed > timelimit / 1000.0) {
            ver = verdict::TLE;
        }

        if (info.memory > memory_limit)
            ver = verdict::MLE;

        if (ver == verdict::AC) {
            auto args{input + " " + output + " " + answer};

            res = sh::execute(checker, args, "", "/dev/null", 2 * timelimit / 1000.0);

            switch (res.rc) {
            case 6:
                ver = verdict::WA;
                break;

            case 5:
                ver = verdict::PE;
                break;

            case 4:
                ver = verdict::AC;
                break;

            default:
                ver = verdict::UNDEF;
                break;
            };
        }

        ans = std::max(ans, ver);
        tmax = std::max(tmax, info.elapsed);
        mmax = std::max(mmax, info.memory);
        passed += ver == verdict::AC ? 1 : 0;

        report.add_row({{number, logger::format::style::COUNTER},
                        {ver_string[ver], ver_style.at(ver)},
                        {as_string(info.elapsed, 6), logger::format::style::FLOAT},
                        {as_string(info.memory, 3), logger::format::style::INT}});
    }

    out << report << '\n';

    int col_size = 12;

    out << logger::format::apply(
               "Verdict:", logger::format::emph::BOLD + logger::format::align::LEFT, col_size)
        << logger::format::apply(ver_string[ans], ver_style.at(ans) + logger::format::align::LEFT)
        << '\n';

    out << logger::format::apply(
               "Passed:", logger::format::emph::BOLD + logger::format::align::LEFT, col_size)
        << logger::format::apply(std::to_string(passed), logger::format::style::INT) << '\n';

    out << logger::format::apply(
               "Max time:", logger::format::emph::BOLD + logger::format::align::LEFT, col_size)
        << logger::format::apply(as_string(tmax, 6), logger::format::style::FLOAT) << '\n';

    out << logger::format::apply(
               "Max memory:", logger::format::emph::BOLD + logger::format::align::LEFT, col_size)
        << logger::format::apply(as_string(mmax, 3), logger::format::style::FLOAT) << '\n';

    return ans;
}

// API functions
int run(int argc, char *const argv[], std::ostream &out, std::ostream &err) {
    int option = -1;
    std::string target{"."};

    while ((option = getopt_long(argc, argv, "h", longopts, NULL)) != -1) {
        switch (option) {
        case 'h':
            out << help() << '\n';
            return 0;

        default:
            err << help() << '\n';
            return CP_TOOLS_ERROR_CLEAN_INVALID_OPTION;
        }
    }

    if (argc < 3) {
        err << usage() << '\n';
        return CP_TOOLS_ERROR_MISSING_ARGUMENT;
    }

    auto solution_path = argv[2];

    return judge(solution_path, out);
}
} // namespace cptools::commands::judge
