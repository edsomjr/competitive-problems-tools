#include <iostream>
#include <map>

#include <getopt.h>
#include <unistd.h>

#include "cli/cli.h"
#include "cli/format.h"
#include "commands/clean.h"
#include "commands/judge.h"
#include "config.h"
#include "defs.h"
#include "dirs.h"
#include "error.h"
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
    {verdict::AC, cli::format::style::AC},       {verdict::PE, cli::format::style::PE},
    {verdict::WA, cli::format::style::WA},       {verdict::CE, cli::format::style::CE},
    {verdict::TLE, cli::format::style::TLE},     {verdict::RTE, cli::format::style::RTE},
    {verdict::MLE, cli::format::style::MLE},     {verdict::FAIL, cli::format::style::FAIL},
    {verdict::UNDEF, cli::format::style::UNDEF},
};

// Auxiliary routines
std::string usage() { return "Usage: " NAME " problem judge solution.[cpp|c|java|py]"; }

std::string help() { return usage() + help_message; }

int judge(const std::string &solution_path) {
    table::Table report{{
        {"#", 4, cli::format::align::RIGHT | cli::format::emph::BOLD},
        {"Verdict", 32, cli::format::align::LEFT | cli::format::emph::BOLD},
        {"Time (s)", 12, cli::format::align::RIGHT | cli::format::emph::BOLD},
        {"Memory (MB)", 12, cli::format::align::RIGHT | cli::format::emph::BOLD},
    }};

    cli::write(cli::fmt::info, "Judging solution '" + solution_path + "'...");

    auto res = task::build_tool(config::tool_type::checker);
    if (not res.ok) {
        cli::write(cli::fmt::error, "Could not build checker: " + res.error_message);
        return CP_TOOLS_ERROR_JUDGE_MISSING_TOOL;
    }

    res = task::build_tool(config::tool_type::validator);
    if (not res.ok) {
        cli::write(cli::fmt::error, "Could not build validator: " + res.error_message);
        return CP_TOOLS_ERROR_JUDGE_MISSING_TOOL;
    }

    res = task::gen_exe(solution_path, "sol");

    if (not res.ok) {
        cli::write(cli::fmt::error, "Could not generate executable: " + res.error_message);
        return verdict::CE;
    }

    auto config = cptools::config::read_config_file();
    auto time_limit = cptools::util::get_json_value(config, "problem|time_limit", 1000);
    auto memory_limit = cptools::util::get_json_value(config, "problem|memory_limit", 1000);

    auto checker{std::string(CP_TOOLS_BUILD_DIR) + "/checker"};
    auto program{std::string(CP_TOOLS_BUILD_DIR) + "/sol"};
    auto validator{std::string(CP_TOOLS_BUILD_DIR) + "/validator"};

    auto files = task::generate_io_files("all");
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

        auto res = sh::execute_program(validator, "", input);

        if (!res.ok) {
            cli::write(cli::fmt::error, "Input file '" + input + "' is invalid");
            cli::write_trace(res.error_message);
            return CP_TOOLS_ERROR_JUDGE_INVALID_INPUT_FILE;
        }

        auto info = sh::profile(program, "", 2 * time_limit / 1000.0, input, output);

        int ver = verdict::AC;

        if (info.rc != CP_TOOLS_OK)
            ver = verdict::RTE;

        if (info.elapsed > time_limit / 1000.0) {
            ver = verdict::TLE;
        }

        if (info.memory > memory_limit)
            ver = verdict::MLE;

        if (ver == verdict::AC) {
            auto args{input + " " + output + " " + answer};

            res = sh::execute_program(checker, args, "", "/dev/null", 2 * time_limit / 1000.0);

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

        report.add_row({{number, cli::format::style::COUNTER},
                        {ver_string[ver], ver_style.at(ver)},
                        {as_string(info.elapsed, 6), cli::format::style::FLOAT},
                        {as_string(info.memory, 3), cli::format::style::INT}});
    }

    std::ostringstream oss;
    int col_size = 12;

    oss << report << std::endl;

    oss << cli::format::apply("Verdict:", cli::format::emph::BOLD + cli::format::align::LEFT,
                              col_size)
        << cli::format::apply(ver_string[ans], ver_style.at(ans) + cli::format::align::LEFT)
        << std::endl;

    oss << cli::format::apply("Passed:", cli::format::emph::BOLD + cli::format::align::LEFT,
                              col_size)
        << cli::format::apply(std::to_string(passed), cli::format::style::INT) << std::endl;

    oss << cli::format::apply("Max time:", cli::format::emph::BOLD + cli::format::align::LEFT,
                              col_size)
        << cli::format::apply(as_string(tmax, 6), cli::format::style::FLOAT) << std::endl;

    oss << cli::format::apply("Max memory:", cli::format::emph::BOLD + cli::format::align::LEFT,
                              col_size)
        << cli::format::apply(as_string(mmax, 3), cli::format::style::FLOAT) << std::endl;

    cli::write(cli::fmt::none, oss.str());

    return ans;
}

// API functions
int run(int argc, char *const argv[]) {
    int option = -1;
    std::string target{"."};

    while ((option = getopt_long(argc, argv, "h", longopts, NULL)) != -1) {
        switch (option) {
        case 'h':
            cli::write(cli::fmt::none, help());
            return 0;

        default:
            cli::write(cli::fmt::none, help(), true);
            return CP_TOOLS_ERROR_CLEAN_INVALID_OPTION;
        }
    }

    if (argc < 3) {
        cli::write(cli::fmt::none, usage(), true);
        return CP_TOOLS_ERROR_MISSING_ARGUMENT;
    }

    auto solution_path = argv[2];

    return judge(solution_path);
}
} // namespace cptools::commands::judge
