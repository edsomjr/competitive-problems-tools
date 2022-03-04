#include <getopt.h>
#include <iostream>
#include <sstream>
#include <unistd.h>

#include "cli/cli.h"
#include "commands/check.h"
#include "config.h"
#include "defs.h"
#include "dirs.h"
#include "error.h"
#include "fs.h"
#include "sh.h"
#include "task.h"
#include "util.h"

// Raw strings
static const std::string help_message
{
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
)message"};

namespace cptools::commands::check
    {

        // Global variables
        static struct option longopts[] = { { "all", no_argument, NULL, 'a' },
                                            { "checker", no_argument, NULL, 'c' },
                                            { "help", no_argument, NULL, 'h' },
                                            { "solutions", no_argument, NULL, 's' },
                                            { "tests", no_argument, NULL, 't' },
                                            { "validator", no_argument, NULL, 'v' },
                                            { 0, 0, 0, 0 } };

        static std::map<std::string, int> rcodes{
            { "AC", 4 },
            { "PE", 5 },
            { "WA", 6 },
        };

        static std::map<int, std::string> mcodes{
            { 4, "AC" },
            { 5, "PE" },
            { 6, "WA" },
        };

        // Auxiliary routines
        std::string usage() { return "Usage: " NAME " check [-h] [-a] [-c] [-s] [-t] [-v]"; }

        std::string help() { return usage() + help_message; }

        int validate_checker()
        {
            cli::write(cli::fmt::info, "Creating directory " CP_TOOLS_BUILD_DIR);
            auto fs_res = fs::create_directory(CP_TOOLS_BUILD_DIR);
            if (not fs_res.ok)
                {
                    cli::write(cli::fmt::error, fs_res.error_message);
                    return fs_res.rc;
                }

            auto validator{ std::string(CP_TOOLS_BUILD_DIR) + "/validator" };
            auto config = cptools::config::read_config_file();

            auto source
                = cptools::util::get_json_value(config, "tools|validator", std::string("ERROR"));

            if (source == "ERROR")
                {
                    cli::write(cli::fmt::error, "[validate_validator] Validator file not found!\n");
                    return CP_TOOLS_ERROR_CHECK_MISSING_VALIDATOR;
                }

            auto res = cptools::sh::build(validator, source);

            if (!res.ok)
                {
                    cli::write(cli::fmt::error, "Can't compile validator '" + source + "'!");
                    cli::write_trace(res.error_message);
                    return res.rc;
                }

            source = cptools::util::get_json_value(config, "tools|checker", std::string("ERROR"));

            if (source == "ERROR")
                {
                    cli::write(cli::fmt::error, "[validate_checker] Checker file not found!\n");
                    return CP_TOOLS_ERROR_CHECK_MISSING_CHECKER;
                }

            auto checker{ std::string(CP_TOOLS_BUILD_DIR) + "/checker" };

            res = cptools::sh::build(checker, source);

            if (!res.ok)
                {
                    cli::write(cli::fmt::error, "Can't compile checker '" + source + "'!");
                    cli::write_trace(res.error_message);
                    return res.rc;
                }

            source = config::get_solutions_file_paths("default")[0];

            if (source == "")
                {
                    cli::write(cli::fmt::error,
                               "[validate_checker] Default solution file not found!\n");
                    return CP_TOOLS_ERROR_CHECK_MISSING_VALIDATOR;
                }

            auto solution{ std::string(CP_TOOLS_BUILD_DIR) + "/solution" };

            res = cptools::sh::build(solution, source);

            if (!res.ok)
                {
                    cli::write(cli::fmt::error, "Can't compile solution '" + source + "'!");
                    cli::write_trace(res.error_message);
                    return res.rc;
                }

            auto tests = cptools::util::get_json_value(
                config, "tests|checker",
                std::map<std::string, std::pair<std::string, std::string> >{});

            if (tests.empty())
                {
                    cli::write(cli::fmt::error,
                               "[validate_checker] There are no tests for the validator\n");
                    return CP_TOOLS_ERROR_CHECK_MISSING_TESTS;
                }

            cli::write(cli::fmt::info,
                       "Testing the checker (" + std::to_string(tests.size()) + " tests) ...");

            for (auto [input, data] : tests)
                {
                    auto [output, verdict] = data;

                    if (rcodes.find(verdict) == rcodes.end())
                        {
                            cli::write(cli::fmt::error,
                                       "[validate_checker] Invalid verdict: '" + verdict + "'\n");
                            return CP_TOOLS_ERROR_CHECK_INVALID_VEREDICT;
                        }

                    auto result = sh::execute_program(validator, "", input, "");

                    if (result.rc != CP_TOOLS_OK)
                        {
                            cli::write(cli::fmt::error, "Input file '" + input + "' is invalid!");
                            cli::write_trace(result.output);
                            return CP_TOOLS_ERROR_CHECK_INVALID_INPUT_FILE;
                        }

                    auto res{ std::string(CP_TOOLS_BUILD_DIR) + "/.res" };

                    result = sh::execute_program(solution, "", input, res);

                    if (result.rc != CP_TOOLS_OK)
                        {
                            cli::write(cli::fmt::error,
                                       "Can't generatate output for input '" + input + "'!");
                            cli::write_trace(result.output);
                            return result.rc;
                        }

                    auto args{ input + " " + output + " " + res };
                    auto expected = rcodes[verdict];
                    auto got = sh::execute_program(checker, args, "", "");

                    if (got.rc != expected)
                        {
                            std::ostringstream oss;

                            oss << got.output;
                            oss << "Got: " << mcodes[got.rc] << ", expected: " << mcodes[expected]
                                << '\n';

                            cli::write(cli::fmt::error, "Test '" + input + "' failed!");
                            cli::write_trace(oss.str());

                            return CP_TOOLS_ERROR_CHECK_TEST_FAILED;
                        }
                }

            cli::write(cli::fmt::ok);

            return CP_TOOLS_OK;
        }

        int validate_validator()
        {
            cli::write(cli::fmt::info, "Creating directory " CP_TOOLS_BUILD_DIR);
            auto fs_res = fs::create_directory(CP_TOOLS_BUILD_DIR);
            if (not fs_res.ok)
                {
                    cli::write(cli::fmt::error, fs_res.error_message);
                    return fs_res.rc;
                }

            auto program{ std::string(CP_TOOLS_BUILD_DIR) + "/validator" };
            auto config = cptools::config::read_config_file();
            auto source
                = cptools::util::get_json_value(config, "tools|validator", std::string("ERROR"));

            if (source == "ERROR")
                {
                    cli::write(cli::fmt::error,
                               "[validate_validator] Default solution file not found!\n");
                    return CP_TOOLS_ERROR_CHECK_MISSING_VALIDATOR;
                }

            auto res = cptools::sh::build(program, source);

            if (!res.ok)
                {
                    cli::write(cli::fmt::error, "Can't compile validator '" + source + "'!");
                    cli::write_trace(res.error_message);
                    return res.rc;
                }

            auto tests = cptools::util::get_json_value(config, "tests|validator",
                                                       std::map<std::string, std::string>{});

            if (tests.empty())
                {
                    cli::write(cli::fmt::error,
                               "[validate_validator] There are no tests for the validator\n");
                    return CP_TOOLS_ERROR_CHECK_MISSING_TESTS;
                }

            cli::write(cli::fmt::info,
                       "Testing the validator (" + std::to_string(tests.size()) + " tests) ...");

            for (auto [input, verdict] : tests)
                {
                    auto result = sh::execute_program(program, "", input, "");

                    std::string res
                        = (result.output.find("FAIL") == std::string::npos ? "OK" : "INVALID");

                    if (verdict != res)
                        {
                            cli::write(cli::fmt::error, "Input '" + input
                                                            + " is invalid: expected = '" + verdict
                                                            + "', got = '" + res + "'");
                            cli::write_trace(result.output.empty() ? "Test valid!" : result.output);
                            return result.rc;
                        }
                }

            cli::write(cli::fmt::ok);

            return CP_TOOLS_OK;
        }

        int validate_tests()
        {
            cli::write(cli::fmt::info, "Creating directory " CP_TOOLS_BUILD_DIR);
            auto fs_res = fs::create_directory(CP_TOOLS_BUILD_DIR);
            if (not fs_res.ok)
                {
                    cli::write(cli::fmt::error, fs_res.error_message);
                    return fs_res.rc;
                }

            auto program{ std::string(CP_TOOLS_BUILD_DIR) + "/validator" };
            auto config = cptools::config::read_config_file();
            auto source
                = cptools::util::get_json_value(config, "tools|validator", std::string("ERROR"));

            if (source == "ERROR")
                {
                    cli::write(cli::fmt::error, "Default solution file not found!");
                    return CP_TOOLS_ERROR_CHECK_MISSING_VALIDATOR;
                }

            auto res = cptools::sh::build(program, source);

            if (!res.ok)
                {
                    cli::write(cli::fmt::error, "Can't compile validator '" + source + "'!");
                    cli::write_trace(res.error_message);
                    return res.rc;
                }

            auto io_files = task::generate_all_io_files(true);

            if (io_files.empty())
                {
                    cli::write(cli::fmt::error, "There are no io files to validate!");
                    return CP_TOOLS_ERROR_CHECK_MISSING_IO_FILES;
                }

            cli::write(cli::fmt::info, "Validating the input files ("
                                           + std::to_string(io_files.size()) + " tests) ...");

            for (auto [input, _] : io_files)
                {
                    auto result = sh::execute_program(program, "", input, "");

                    if (result.rc != CP_TOOLS_OK)
                        {
                            cli::write(cli::fmt::error, "Input file '" + input + "' is invalid!");
                            cli::write_trace(result.output);
                            return CP_TOOLS_ERROR_CHECK_INVALID_INPUT_FILE;
                        }
                }

            cli::write(cli::fmt::ok);

            return CP_TOOLS_OK;
        }

        // API functions
        int run(int argc, char *const argv[])
        {
            int option = -1;

            while ((option = getopt_long(argc, argv, "achstv", longopts, NULL)) != -1)
                {
                    switch (option)
                        {
                        case 'c':
                            return validate_checker();

                        case 'h':
                            cli::write(cli::fmt::none, help());
                            return 0;

                        case 't':
                            return validate_tests();

                        case 'v':
                            return validate_validator();

                        case 'a':
                            for (auto func :
                                 { validate_checker, validate_tests, validate_validator })
                                {
                                    auto rtn = func();

                                    if (rtn)
                                        {
                                            return rtn;
                                        }
                                }
                            return CP_TOOLS_OK;

                        default:
                            cli::write(cli::fmt::none, help(), true);
                            return CP_TOOLS_ERROR_CLEAN_INVALID_OPTION;
                        }
                }

            return CP_TOOLS_OK;
        }
    } // namespace cptools::commands::check
