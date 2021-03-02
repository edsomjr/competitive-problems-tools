#include <iostream>
#include <map>

#include <unistd.h>
#include <getopt.h>

#include "sh.h"
#include "defs.h"
#include "dirs.h"
#include "task.h"
#include "commands/clean.h"
#include "error.h"
#include "commands/judge.h"
#include "table.h"
#include "config.h"
#include "format.h"
#include "message.h"

using std::map;
using std::max;
using std::to_string;

// Raw strings
static const std::string help_message {
R"message(
Runs a solution against all test sets and gives you a veredict.

    Option          Description

    -h              Generates this help message.
    --help

)message"
};

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
    }

    // Global variables
    static struct option longopts[] = {
       { "help", no_argument, NULL, 'h' },
       { 0, 0, 0, 0 }
    };

    map<int, std::string> ver_string {
        { verdict::AC, "Accepted" },
        { verdict::PE, "Presentation Error" },
        { verdict::WA, "Wrong Answer" },
        { verdict::CE, "Compilation Error" },
        { verdict::TLE, "Time Limit Exceeded" },
        { verdict::RTE, "Runtime Error" },
        { verdict::MLE, "Memory Limit Exceeded" },
        { verdict::FAIL, "Failure" },
        { verdict::UNDEF, "Undefined Error" },
    };
    
    const map<int, long long> ver_style {
        { verdict::AC, format::style::AC },
        { verdict::PE, format::style::PE },
        { verdict::WA, format::style::WA },
        { verdict::CE, format::style::CE },
        { verdict::TLE, format::style::TLE },
        { verdict::RTE, format::style::RTE },
        { verdict::MLE, format::style::MLE },
        { verdict::FAIL, format::style::FAIL },
        { verdict::UNDEF, format::style::UNDEF },
    };

    // Auxiliary routines
    std::string usage()
    {
        return "Usage: " NAME " problem judge solution.[cpp|c|java|py]";
    }

    std::string help()
    {
        return usage() + help_message;
    }

    int judge(const std::string& solution_path, std::ostream& out, std::ostream& err)
    {
        table::Table report { {
            { "#", 4, format::align::RIGHT | format::emph::BOLD },
            { "Verdict", 32, format::align::LEFT | format::emph::BOLD },
            { "Time (s)", 12, format::align::RIGHT | format::emph::BOLD },
            { "Memory (MB)", 12, format::align::RIGHT | format::emph::BOLD },
        } };

        out << message::info("Judging solution '" + solution_path + "'...") << "\n";

        // Constrói as ferramentas necessárias
        string error;
        auto rc = task::build_tools(error, task::tools::VALIDATOR | task::tools::CHECKER);

        if (rc != CP_TOOLS_OK)
        {
            err << message::failure("Can't build the required tools") << '\n';
            err << message::trace(error);
            return CP_TOOLS_ERROR_JUDGE_MISSING_TOOL;
        }

        // Gera o executável da solução
        rc = task::gen_exe(error, solution_path, "sol");

        if (rc != CP_TOOLS_OK)
        {
            err << message::failure("Error on solution '" + solution_path + "' compilation") << '\n';
            err << message::trace(error) << '\n';
            return verdict::CE;
        }

        auto config = cptools::config::read("config.json");
        auto timelimit = cptools::config::get(config, "problem|timelimit", 1000);
        auto memory_limit = cptools::config::get(config, "problem|memory_limit", 1000);

        auto checker { std::string(CP_TOOLS_BUILD_DIR) + "/checker" };
        auto program { std::string(CP_TOOLS_BUILD_DIR) + "/sol" };
        auto validator { std::string(CP_TOOLS_BUILD_DIR) + "/validator" };

        auto files = task::generate_io_files("all", out, err);
        int ans = verdict::AC, passed = 0;
        double tmax = 0.0, mmax = 0.0;

        auto as_string = [](double x, int places) {
            char buffer[64];
            sprintf(buffer, "%.*f", places, x);

            return string(buffer);
        };

        for (auto [input, answer] : files)
        {
            auto number = util::split(input, '/').back();
            auto output { std::string(CP_TOOLS_BUILD_DIR) + "/out" };

            auto res = sh::execute(validator, "", input); 

            if (res.rc != CP_TOOLS_OK)
            {
                err << message::failure("Input file '" + input + "' is invalid") << "\n";
                err << message::trace(res.output) << '\n';
                return CP_TOOLS_ERROR_JUDGE_INVALID_INPUT_FILE;
            }

            auto info = sh::profile(program, "", 2*timelimit / 1000.0, input, output);

            int ver = verdict::AC;

            if (info.rc != CP_TOOLS_OK)
                ver = verdict::RTE;

            if (info.elapsed > timelimit / 1000.0)
            {
                ver = verdict::TLE;
            }

            if (info.memory > memory_limit)
                ver = verdict::MLE;

            if (ver == verdict::AC)
            {
                auto args { input + " " + output + " " + answer };
     
                res = sh::execute(checker, args, "", "/dev/null", 2*timelimit / 1000.0);

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

            ans = max(ans, ver);
            tmax = max(tmax, info.elapsed);
            mmax = max(mmax, info.memory);
            passed += ver == verdict::AC ? 1 : 0;

            report.add_row({ 
                { number, format::style::COUNTER }, 
                { ver_string[ver], ver_style.at(ver) },
                { as_string(info.elapsed, 6), format::style::FLOAT }, 
                { as_string(info.memory, 3), format::style::INT } 
            });
        }
 
        out << report << '\n';

        int col_size = 12;

        out << format::apply("Verdict:", format::emph::BOLD + format::align::LEFT, col_size) <<
            format::apply(ver_string[ans], ver_style.at(ans) + format::align::LEFT) << '\n';

        out << format::apply("Passed:", format::emph::BOLD + format::align::LEFT, col_size) <<
            format::apply(to_string(passed), format::style::INT) << '\n';

        out << format::apply("Max time:", format::emph::BOLD + format::align::LEFT, col_size) <<
            format::apply(as_string(tmax, 6), format::style::FLOAT) << '\n';

        out << format::apply("Max memory:", format::emph::BOLD + format::align::LEFT, col_size) <<
            format::apply(as_string(mmax, 3), format::style::FLOAT) << '\n';

        return ans;
    }

    // API functions
    int run(int argc, char* const argv[], std::ostream& out, std::ostream& err)
    {
        int option = -1;
        std::string target { "." };

        while ((option = getopt_long(argc, argv, "h", longopts, NULL)) != -1)
        {
            switch (option) {
            case 'h':
                out << help() << '\n';
                return 0;

            default:
                err << help() << '\n';
                return CP_TOOLS_ERROR_CLEAN_INVALID_OPTION;
            }
        }

        if (argc < 3)
        {
            err << usage() << '\n';
            return CP_TOOLS_ERROR_MISSING_ARGUMENT;
        }

        auto solution_path = argv[2];

        return judge(solution_path, out, err);
    }
}
