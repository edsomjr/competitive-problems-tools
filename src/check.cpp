#include <iostream>

#include <unistd.h>
#include <getopt.h>

#include "sh.h"
#include "task.h"
#include "defs.h"
#include "dirs.h"
#include "check.h"
#include "error.h"
#include "config.h"


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

namespace cptools::check {

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
        auto validator { std::string(CP_TOOLS_BUILD_DIR) + "/validator" };

        auto config = cptools::config::read("config.json");
        auto source = cptools::config::get(config, "tools|validator", std::string("ERROR"));

        if (source == "ERROR")
        {
            err << "[validate_validator] Validator file not found!\n";
            return CP_TOOLS_ERROR_CHECK_MISSING_VALIDATOR;
        }

        auto rc = cptools::sh::build(validator, source);

        if (rc != CP_TOOLS_OK)
        {
            err << "[judge] Can't compile validator '" << source << "'\n";
            return rc;
        }

        source = cptools::config::get(config, "tools|checker", std::string("ERROR"));

        if (source == "ERROR")
        {
            err << "[validate_checker] Checker file not found!\n";
            return CP_TOOLS_ERROR_CHECK_MISSING_CHECKER;
        }

        auto checker { std::string(CP_TOOLS_BUILD_DIR) + "/checker" };

        rc = cptools::sh::build(checker, source);

        if (rc != CP_TOOLS_OK)
        {
            err << "[validate_checker] Can't compile checker '" << source << "'\n";
            return rc;
        }

        source = "solutions/" + 
            cptools::config::get(config, "solutions|default", std::string("ERROR"));

        if (source == "solutions/ERROR")
        {
            err << "[validate_checker] Default solution file not found!\n";
            return CP_TOOLS_ERROR_CHECK_MISSING_VALIDATOR;
        }

        auto solution { std::string(CP_TOOLS_BUILD_DIR) + "/solution" };

        rc = cptools::sh::build(solution, source);

        if (rc != CP_TOOLS_OK)
        {
            err << "[validate_checker] Can't compile solution '" << source << "'\n";
            return rc;
        }

        auto tests = cptools::config::get(config, "tests|checker", 
            std::map<std::string, std::pair<std::string, std::string>> {});

        if (tests.empty())
        {
            err << "[validate_checker] There are no tests for the validator\n";
            return CP_TOOLS_ERROR_CHECK_MISSING_TESTS;
        }

        out << "Testing the checker (" << tests.size() << " tests) ...\n";

        for (auto [input, data] : tests)
        {
            auto [output, veredict] = data;

            if (rcodes.find(veredict) == rcodes.end())
            {
                err << "[validate_checker] Invalid veredict: '" << veredict << "'\n";
                return CP_TOOLS_ERROR_CHECK_INVALID_VEREDICT;
            }

            auto rc = sh::process(input, validator, "/dev/null");

            if (rc != CP_TOOLS_OK)
            {
                err << "[validate_checker] Input file '" << input << "' is invalid\n";
                return CP_TOOLS_ERROR_CHECK_INVALID_INPUT_FILE;
            }

            auto res { std::string(CP_TOOLS_BUILD_DIR) + "/.res" };

            rc = sh::process(input, solution, res);

            if (rc != CP_TOOLS_OK)
            {
                err << "[validate_checker] Can't generate output for input '" << input << "'\n";
                return CP_TOOLS_ERROR_CHECK_INVALID_INPUT_FILE;
            }

            auto args { input + " " + output + " " + res };
            auto expected = rcodes[veredict];
            auto got = sh::exec(checker, args, "/dev/null");

            if (got != expected)
            {
                err << "[validate_checker] Test '" << input << "' failed!\n";
                return CP_TOOLS_ERROR_CHECK_TEST_FAILED;
            }
        }

        out << "Ok!\n";

        return CP_TOOLS_OK;
    }


    int validate_validator(std::ostream& out, std::ostream& err)
    {
        auto program { std::string(CP_TOOLS_BUILD_DIR) + "/validator" };

        auto config = cptools::config::read("config.json");
        auto source = cptools::config::get(config, "tools|validator", std::string("ERROR"));

        if (source == "ERROR")
        {
            err << "[validate_validator] Default solution file not found!\n";
            return CP_TOOLS_ERROR_CHECK_MISSING_VALIDATOR;
        }

        auto rc = cptools::sh::build(program, source);

        if (rc != CP_TOOLS_OK)
        {
            err << "[validate_validator] Can't compile validator '" << source << "'\n";
            return rc;
        }

        auto tests = cptools::config::get(config, "tests|validator", 
            std::map<std::string, std::string> {});

        if (tests.empty())
        {
            err << "[validate_validator] There are no tests for the validator\n";
            return CP_TOOLS_ERROR_CHECK_MISSING_TESTS;
        }

        out << "Testing the validator (" << tests.size() << " tests) ...\n";

        for (auto [input, veredict] : tests)
        {
            auto rc = sh::process(input, program, "/dev/null");

            if ((veredict == "VALID" and rc != CP_TOOLS_OK) or
                (veredict == "INVALID" and rc == CP_TOOLS_OK))
            {
                err << "[validate_validator] input = '" << input << "' is invalid: expected = '" 
                    << veredict << "'\n";
                return CP_TOOLS_ERROR_CHECK_INVALID_INPUT_FILE;
            }
        }

        out << "Ok!\n";

        return CP_TOOLS_OK;
    }

    int validate_tests(std::ostream& out, std::ostream& err)
    {
        auto program { std::string(CP_TOOLS_BUILD_DIR) + "/validator" };

        auto config = cptools::config::read("config.json");
        auto source = cptools::config::get(config, "tools|validator", std::string("ERROR"));

        out << "[validate_tests] source = '" << source << "'\n";

        if (source == "ERROR")
        {
            err << "[validate_tests] Default solution file not found!\n";
            return CP_TOOLS_ERROR_CHECK_MISSING_VALIDATOR;
        }

        auto rc = cptools::sh::build(program, source);

        if (rc != CP_TOOLS_OK)
        {
            err << "[validate_tests] Can't compile validator '" << source << "'\n";
            return rc;
        }


        auto io_files = task::generate_io_files("all", out, err, false);

        if (io_files.empty())
        {
            err << "[validate_tests] There are no io files to validate!\n";
            return CP_TOOLS_ERROR_CHECK_MISSING_IO_FILES;
        }

        out << "Checking " << io_files.size() << " input files...\n";

        for (auto [input, _] : io_files)
        {
            err << "[check tests] Validating " << input << "...\n";

            auto rc = cptools::sh::process(input, program, "/dev/null");

            if (rc != CP_TOOLS_OK)
            {
                out << "Input file '" << input << "' is invalid!\n";
                return CP_TOOLS_ERROR_CHECK_INVALID_INPUT_FILE;
            }
        }

        out << "Ok!\n";

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
