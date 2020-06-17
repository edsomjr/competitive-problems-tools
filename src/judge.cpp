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
#include "table.h"
#include "config.h"
#include "format.h"
#include "message.h"

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

    namespace verdict {
        int AC = 0;
        int PE = 1;
        int WA = 2;
        int TLE = 3;
        int RTE = 4;
        int MLE = 5;
        int UNDEF = 6;
        int CE = 7;
        int FAIL = 8;
    }

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
        table::Table report { {
            { "#", 4, format::align::RIGHT | format::emph::BOLD },
            { "Verdict", 32, format::align::LEFT | format::emph::BOLD },
            { "Time (s)", 12, format::align::RIGHT | format::emph::BOLD },
            { "Memory (KB)", 12, format::align::RIGHT | format::emph::BOLD },
        } };

//        t.add_row({ { "1", format::style::COUNTER }, { "Accepted", format::style::AC }, 
//            { "0.344", format::style::FLOAT }, { "100", format::style::INT } });

//        t.add_row({ { "2", format::style::COUNTER }, { "Wrong Answer", format::style::WA }, 
//            { "0.2", format::style::FLOAT }, { "60", format::style::INT } });

//        t.add_row({ { "3", format::style::COUNTER }, { "Presentation Error", format::style::PE }, 
 //           { "0.5142", format::style::FLOAT }, { "2318", format::style::INT } });

//        out << t << '\n';

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

        auto checker { std::string(CP_TOOLS_BUILD_DIR) + "/checker" };
        auto program { std::string(CP_TOOLS_BUILD_DIR) + "/sol" };
        auto validator { std::string(CP_TOOLS_BUILD_DIR) + "/validator" };

        auto files = task::generate_io_files("all", out, err);

        for (auto [input, answer] : files)
        {
            auto number = util::split(input, '/').back();
            auto output { std::string(CP_TOOLS_BUILD_DIR) + "/out" };

            out << "[judge] Test '" << input << "': ";

            // TODO: atualizar a função para que a saída fica na string do 3º parâmetro
            rc = sh::process(input, validator, "/dev/null");

            if (rc != CP_TOOLS_OK)
            {
                err << message::failure("Input file '" + input + "' is invalid") << "\n";
                err << message::trace(error) << '\n';
                return CP_TOOLS_ERROR_JUDGE_INVALID_INPUT_FILE;
            }

            auto start = timer::now();
            rc = sh::process(input, program, output, 3*timelimit / 1000.0);
            auto end = timer::now();

            std::chrono::duration<double> t = end - start;

            std::ostringstream oss;
            oss << std::setprecision(5) << std::fixed << t.count();
            auto elapsed = oss.str();

            out << std::setprecision(6) << std::fixed 
                << t.count()
                << "s, veredict: ";

            if (t.count() > timelimit / 1000.0)
            {
                out << "Time Limit Exceeded\n";
    //            return verdict::TLE;
            }

            if (rc != CP_TOOLS_OK)
            {
                err << "[judge] Can't generate output for input '" << input << "'\n";
   //             return verdict::RTE;
            }

            auto args { input + " " + output + " " + answer };
 
            rc = sh::exec(checker, args, "/dev/null", 3*timelimit / 1000.0);

            switch (rc) {
            case 6:
                out << "Wrong Answer!\n";
  //              return verdict::WA;
                break;

            case 5:
                out << "Presentation Error\n";
 //               return verdict::PE;
                break;

            case 4:
                out << "Ok!\n";
                break;

            default:
                out << "Undefined Error!\n";
//                return verdict::UNDEF;
                break;
            };


        report.add_row({ { number, format::style::COUNTER }, { "Accepted", format::style::AC }, 
            { elapsed, format::style::FLOAT }, { "100", format::style::INT } });
        }
 
        out << report << '\n';

        return verdict::AC;
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
