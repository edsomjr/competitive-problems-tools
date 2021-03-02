#include <iostream>
#include <sstream>

#include <unistd.h>
#include <getopt.h>

#include "sh.h"
#include "task.h"
#include "defs.h"
#include "dirs.h"
#include "commands/check.h"
#include "error.h"
#include "config.h"
#include "message.h"

using namespace std;

// Raw strings
static const std::string help_message {
R"message(
Verify problem files and tools. 

    Option          Description

    -a              Validates everything (default option).
    --all

    -c              Validates the checker.
    --checker

    -h              Generates this help message.
    --help

    -s              Validates the solutions.
    --solutions

    -t              Validates the tests.
    --tests

    -v              Validates the validator.
    --validator
)message"
};

namespace cptools::commands::check {

    // Global variables
    static struct option longopts[] = {
       { "all", no_argument, NULL, 'a' },
       { "checker", no_argument, NULL, 'c' },
       { "help", no_argument, NULL, 'h' },
       { "solutions", no_argument, NULL, 's' },
       { "tests", no_argument, NULL, 't' },
       { "validator", no_argument, NULL, 'v' },
       { 0, 0, 0, 0 }
    };

    static std::map<std::string, int> rcodes {
        { "AC", 4 },
        { "PE", 5 },
        { "WA", 6 },
    };

    static std::map<int, std::string> mcodes {
        { 4, "AC" },
        { 5, "PE" },
        { 6, "WA" },
    };

    // Auxiliary routines
    std::string usage()
    {
        return "Usage: " NAME " check [-h] [-a] [-c] [-s] [-t] [-v]";
    }

    std::string help()
    {
        return usage() + help_message;
    }

    int validate_checker(std::ostream& out, std::ostream& err)
    {
        auto res = sh::make_dir(CP_TOOLS_BUILD_DIR);

        if (res.rc != CP_TOOLS_OK)
        {
            err << message::failure("Error creating dir '" + string(CP_TOOLS_BUILD_DIR) + "'\n");
            err << message::trace(res.output) << '\n';
            return res.rc;
        }

        auto validator { std::string(CP_TOOLS_BUILD_DIR) + "/validator" };
        auto config = cptools::config::read("config.json");
        auto source = cptools::config::get(config, "tools|validator", std::string("ERROR"));

        if (source == "ERROR")
        {
            err << "[validate_validator] Validator file not found!\n";
            return CP_TOOLS_ERROR_CHECK_MISSING_VALIDATOR;
        }

        res = cptools::sh::build(validator, source);

        if (res.rc != CP_TOOLS_OK)
        {
            err << message::failure("Can't compile validator '" + source + "'!") << "\n";
            err << message::trace(res.output) << '\n';
            return res.rc;
        }

        source = cptools::config::get(config, "tools|checker", std::string("ERROR"));

        if (source == "ERROR")
        {
            err << "[validate_checker] Checker file not found!\n";
            return CP_TOOLS_ERROR_CHECK_MISSING_CHECKER;
        }

        auto checker { std::string(CP_TOOLS_BUILD_DIR) + "/checker" };

        res = cptools::sh::build(checker, source);

        if (res.rc != CP_TOOLS_OK)
        {
            err << message::failure("Can't compile checker '" + source + "'!") << "\n";
            err << message::trace(res.output) << '\n';
            return res.rc;
        }
 
        source = "solutions/" + 
            cptools::config::get(config, "solutions|default", std::string("ERROR"));

        if (source == "solutions/ERROR")
        {
            err << "[validate_checker] Default solution file not found!\n";
            return CP_TOOLS_ERROR_CHECK_MISSING_VALIDATOR;
        }

        auto solution { std::string(CP_TOOLS_BUILD_DIR) + "/solution" };

        res = cptools::sh::build(solution, source);

        if (res.rc != CP_TOOLS_OK)
        {
            err << message::failure("Can't compile solution '" + source + "'!") << "\n";
            err << message::trace(res.output) << '\n';
            return res.rc;
        }

        auto tests = cptools::config::get(config, "tests|checker", 
            std::map<std::string, std::pair<std::string, std::string>> {});

        if (tests.empty())
        {
            err << "[validate_checker] There are no tests for the validator\n";
            return CP_TOOLS_ERROR_CHECK_MISSING_TESTS;
        }

        out << message::info("Testing the checker (" + to_string(tests.size()) + " tests) ...\n");

        for (auto [input, data] : tests)
        {
            auto [output, verdict] = data;

            if (rcodes.find(verdict) == rcodes.end())
            {
                err << "[validate_checker] Invalid verdict: '" << verdict << "'\n";
                return CP_TOOLS_ERROR_CHECK_INVALID_VEREDICT;
            }

            auto result = sh::execute(validator, "", input, "");

            if (result.rc != CP_TOOLS_OK)
            {
                err << message::failure("Input file '" + input + "' is invalid!") << "\n";
                err << message::trace(result.output) << '\n';
                return CP_TOOLS_ERROR_CHECK_INVALID_INPUT_FILE;
            }

            auto res { std::string(CP_TOOLS_BUILD_DIR) + "/.res" };

            result = sh::execute(solution, "", input, res);

            if (result.rc != CP_TOOLS_OK)
            {
                err << message::failure("Can't generatate output for input '" + input 
                    + "'!") << "\n";
                err << message::trace(result.output) << '\n';
                return result.rc;
            }

            auto args { input + " " + output + " " + res };
            auto expected = rcodes[verdict];
            auto got = sh::execute(checker, args, "", "");

            if (got.rc != expected)
            {
                ostringstream oss;

                oss << got.output;
                oss << "Got: " << mcodes[got.rc] << ", expected: " << mcodes[expected] << '\n';

                err << message::failure("Test '" + input + "' failed!") << "\n";
                err << message::trace(oss.str());

                return CP_TOOLS_ERROR_CHECK_TEST_FAILED;
            }
        }

        out << message::success() << "\n";

        return CP_TOOLS_OK;
    }


    int validate_validator(std::ostream& out, std::ostream& err)
    {
        auto res = sh::make_dir(CP_TOOLS_BUILD_DIR);

        if (res.rc != CP_TOOLS_OK)
        {
            err << message::failure("Error creating dir '" + string(CP_TOOLS_BUILD_DIR) + "'\n");
            err << message::trace(res.output) << '\n';
            return res.rc;
        }

        auto program { std::string(CP_TOOLS_BUILD_DIR) + "/validator" };
        auto config = cptools::config::read("config.json");
        auto source = cptools::config::get(config, "tools|validator", std::string("ERROR"));

        if (source == "ERROR")
        {
            err << "[validate_validator] Default solution file not found!\n";
            return CP_TOOLS_ERROR_CHECK_MISSING_VALIDATOR;
        }

        res = cptools::sh::build(program, source);

        if (res.rc != CP_TOOLS_OK)
        {
            err << message::failure("Can't compile validator '" + source + "'!") << "\n";
            err << message::trace(res.output) << '\n';
            return res.rc;
        }

        auto tests = cptools::config::get(config, "tests|validator", 
            std::map<std::string, std::string> {});

        if (tests.empty())
        {
            err << "[validate_validator] There are no tests for the validator\n";
            return CP_TOOLS_ERROR_CHECK_MISSING_TESTS;
        }

        out << message::info("Testing the validator (" + to_string(tests.size()) + " tests) ...\n");

        for (auto [input, verdict] : tests)
        {
            auto result = sh::execute(program, "", input, "");

            string res = (result.output.find("FAIL") == string::npos ? "OK" : "INVALID");

            if (verdict != res)
            {
                err << message::failure("Input '" + input + " is invalid: expected = '" +
                    verdict + "', got = '" + res + "'\n");
                err << message::trace(result.output.empty() ? "Test valid!" : result.output) << '\n';
                return result.rc;
            }
        }

        out << message::success() << '\n';

        return CP_TOOLS_OK;
    }

    int validate_tests(std::ostream& out, std::ostream& err)
    {
        auto res = sh::make_dir(CP_TOOLS_BUILD_DIR);

        if (res.rc != CP_TOOLS_OK)
        {
            err << message::failure("Error creating dir '" + string(CP_TOOLS_BUILD_DIR) + "'\n");
            err << message::trace(res.output) << '\n';
            return res.rc;
        }

        auto program { std::string(CP_TOOLS_BUILD_DIR) + "/validator" };
        auto config = cptools::config::read("config.json");
        auto source = cptools::config::get(config, "tools|validator", std::string("ERROR"));

        if (source == "ERROR")
        {
            err << message::failure("Default solution file not found!\n");
            return CP_TOOLS_ERROR_CHECK_MISSING_VALIDATOR;
        }

        res = cptools::sh::build(program, source);

        if (res.rc != CP_TOOLS_OK)
        {
            err << message::failure("Can't compile validator '" + source + "'!") << "\n";
            err << message::trace(res.output) << '\n';
            return res.rc;
        }

        auto io_files = task::generate_io_files("all", out, err, false);

        if (io_files.empty())
        {
            err << message::failure("There are no io files to validate!\n");
            return CP_TOOLS_ERROR_CHECK_MISSING_IO_FILES;
        }

        out << message::info("Validating the input files (" + to_string(io_files.size()) +
            " tests) ...\n");

        for (auto [input, _] : io_files)
        {
            auto result = sh::execute(program, "", input, "");

            if (result.rc != CP_TOOLS_OK)
            {
                err << message::failure("Input file '" + input + "' is invalid!\n");
                err << message::trace(result.output) << '\n';
                return CP_TOOLS_ERROR_CHECK_INVALID_INPUT_FILE;
            }
        }

        out << message::success() << "\n";

        return CP_TOOLS_OK;
    }

    // API functions
    int run(int argc, char* const argv[], std::ostream& out, std::ostream& err)
    {
        int option = -1;

        while ((option = getopt_long(argc, argv, "achstv", longopts, NULL)) != -1)
        {
            switch (option) {
            case 'c':
                return validate_checker(out, err);

            case 'h':
                out << help() << '\n';
                return 0;

            case 't':
                return validate_tests(out, err);

            case 'v':
                return validate_validator(out, err);

            default:
                err << help() << '\n';
                return CP_TOOLS_ERROR_CLEAN_INVALID_OPTION;
            }
        }

        return CP_TOOLS_OK;
    }
}
