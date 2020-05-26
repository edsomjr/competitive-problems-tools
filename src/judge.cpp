#include <iostream>
#include <iomanip>
#include <chrono>

#include <unistd.h>
#include <getopt.h>

#include "sh.h"
#include "defs.h"
#include "dirs.h"
#include "task.h"
#include "clean.h"
#include "error.h"
#include "judge.h"
#include "config.h"

using timer = std::chrono::high_resolution_clock;

// Raw strings
static const std::string help_message {
R"message(
Runs a solution against all test sets and gives you a veredict.

    Option          Description

    -h              Generates this help message.
    --help

)message"
};

namespace cptools::judge {

    // Global variables
    static struct option longopts[] = {
       { "help", no_argument, NULL, 'h' },
       { 0, 0, 0, 0 }
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
        auto config = cptools::config::read("config.json");
        auto timelimit = cptools::config::get(config, "problem|timelimit", 1000);

        auto validator { std::string(CP_TOOLS_BUILD_DIR) + "/validator" };
        auto source = cptools::config::get(config, "tools|validator", std::string("ERROR"));

        if (source == "ERROR")
        {
            err << "[judge] Validator file not found!\n";
            return CP_TOOLS_ERROR_JUDGE_MISSING_VALIDATOR;
        }

        auto rc = cptools::sh::build(validator, source);

        if (rc != CP_TOOLS_OK)
        {
            err << "[validate_checker] Can't compile validator '" << source << "'\n";
            return rc;
        }

        source = cptools::config::get(config, "tools|checker", std::string("ERROR"));

        if (source == "ERROR")
        {
            err << "[judge] Checker file not found!\n";
            return CP_TOOLS_ERROR_JUDGE_MISSING_CHECKER;
        }

        auto checker { std::string(CP_TOOLS_BUILD_DIR) + "/checker" };

        rc = cptools::sh::build(checker, source);

        if (rc != CP_TOOLS_OK)
        {
            err << "[judge] Can't compile checker '" << source << "'\n";
            return rc;
        }

        auto program { std::string(CP_TOOLS_BUILD_DIR) + "/sol" };

        sh::remove_file(program);
        rc = sh::build(program, solution_path);

        if (rc != CP_TOOLS_OK)
        {
            err << "[judge] Can't build solution '" << solution_path << "'\n";
            return CE;
        }

        out << "[judge] Judging solution '" << solution_path << "'...\n";

        auto files = task::generate_io_files("all", out, err);

        for (auto [input, answer] : files)
        {
            auto output { std::string(CP_TOOLS_BUILD_DIR) + "/out" };

            out << "[judge] Test '" << input << "': ";

            rc = sh::process(input, validator, "/dev/null");

            if (rc != CP_TOOLS_OK)
            {
                err << "[judge] Input file '" << input << "' is invalid\n";
                return CP_TOOLS_ERROR_JUDGE_INVALID_INPUT_FILE;
            }

            auto start = timer::now();
            rc = sh::process(input, program, output, 3*timelimit / 1000.0);
            auto end = timer::now();

            std::chrono::duration<double> t = end - start;
            out << std::setprecision(6) << std::fixed 
                << t.count()
                << "s, veredict: ";

            if (t.count() > timelimit / 1000.0)
            {
                out << "Time Limit Exceeded\n";
                return TLE;
            }

            if (rc != CP_TOOLS_OK)
            {
                err << "[judge] Can't generate output for input '" << input << "'\n";
                return RTE;
            }

            auto args { input + " " + output + " " + answer };
 
            rc = sh::exec(checker, args, "/dev/null", 3*timelimit / 1000.0);

                        switch (rc) {
            case 6:
                out << "Wrong Answer!\n";
                return WA;

            case 4:
                out << "Ok!\n";
                break;

            default:
                out << "Undefined Error!\n";
                return UNDEF;
            };
        }
 
        return AC;
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

        if (argc < 4)
        {
            err << usage() << '\n';
            return CP_TOOLS_ERROR_MISSING_ARGUMENT;
        }

        auto solution_path = argv[3];

        return judge(solution_path, out, err);
    }
}
