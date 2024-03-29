#include <filesystem>
#include <getopt.h>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "cli/cli.h"
#include "commands/polygon/polygon.h"
#include "commands/polygon/pull.h"
#include "config.h"
#include "conflicts.h"
#include "error.h"
#include "exceptions.h"
#include "fs.h"
#include "task.h"
#include "types/polygon.h"
#include "util.h"

namespace cptools::commands::polygon::pull {

// Static variables
static const std::string help_message{
    R"message(
Pulls a problem checker, validator and tests from the Polygon problem
set in config.json. It uses the credentials in ~/.cp-tools-config.json to
connect to Polygon's API.
The options are:

    Option          Description

    -h              Generates this help message.
    --help

    -f              Overwrites the existing problem's files.
    --force
)message"};

static struct option longopts[] = {{"help", no_argument, NULL, 'h'},
                                   {"force", no_argument, NULL, 'f'}};

// Functions
/**
 * @brief Pulls from the Polygon API the file for the given tool.
 *
 * @param tool_name name of the tool to pull
 * @param creds credentials data structure
 * @param problem_id Polygon's problem ID
 * @param forced if it should overwrite the existing files
 */
void pull_tool_file(const std::string tool_name, const types::polygon::Credentials &creds,
                    const std::string &problem_id, bool forced) {
    cli::write(cli::fmt::info, "Pulling " + tool_name + "...");
    auto polygon_file_name = api::polygon::get_problem_file_name(tool_name, creds, problem_id);
    auto polygon_file_content =
        api::polygon::get_problem_file(polygon_file_name, tool_name, creds, problem_id);
    auto local_file_name = config::get_tool_file_name(tool_name);

    auto new_file_path =
        conflicts::solve_files(local_file_name, polygon_file_name, polygon_file_content, forced);
    config::modify_config_file("tools|" + tool_name, new_file_path);
}

/**
 * @brief Get the solutions objects from the Polygon API and writes them to the filesystem.
 *
 * @param creds credentials data structure
 * @param problem_id` Polygon's problem ID
 * @param forced if it should overwrite the existing files
 */
void pull_solutions(const types::polygon::Credentials &creds, const std::string &problem_id,
                    bool forced) {
    cli::write(cli::fmt::info, "Pulling solutions...");
    auto solutions = api::polygon::get_problem_solutions(creds, problem_id);

    for (const auto &solution : solutions) {
        auto files_with_same_tag = config::get_solutions_file_paths(solution.tag);

        auto file_content =
            api::polygon::get_problem_file(solution.name, "solution", creds, problem_id);

        const auto remote_file_name = std::filesystem::path(solution.name).filename().string();
        const auto remote_file_path = "solutions/" + remote_file_name;

        auto file_exists = std::find(files_with_same_tag.begin(), files_with_same_tag.end(),
                                     remote_file_name) != files_with_same_tag.end();

        if (!file_exists)
            fs::overwrite_file(remote_file_path, file_content);
        else
            conflicts::solve_files(remote_file_path, remote_file_name, file_content, forced);
        config::insert_solution_file_name(solution.tag, remote_file_name);
    }
}

/**
 * @brief Get the statement objects from the Polygon API and writes them to the filesystem.
 *
 * @param creds credentials data structure
 * @param problem_id Polygon's problem ID
 * @param forced if it should overwrite the existing statement
 */
void pull_titles(const types::polygon::Credentials &creds, const std::string &problem_id) {
    auto config_json = config::read_config_file();

    cli::write(cli::fmt::info, "Pulling problem title...");
    auto statements = api::polygon::get_problem_statement(creds, problem_id);

    std::unordered_map<std::string, std::string> titles;

    for (const auto &statement : statements) {
        auto lang = statement.language;
        titles.emplace(lang, statement.title);
    }

    config::modify_config_file("problem|title", titles);
}

void pull_infos(const types::polygon::Credentials &creds, const std::string &problem_id) {
    cli::write(cli::fmt::info, "Pulling problem informations...");
    auto info = api::polygon::get_problem_information(creds, problem_id);

    config::modify_config_file("problem|memory_limit", info.memory_limit);
    config::modify_config_file("problem|time_limit", info.time_limit);
}

void pull_tags(const types::polygon::Credentials &creds, const std::string &problem_id) {
    cli::write(cli::fmt::info, "Pulling problem tags...");
    auto tags = api::polygon::get_problem_tags(creds, problem_id);
    config::modify_config_file("problem|tags", tags);
}

void pull_tests(const types::polygon::Credentials &creds, const std::string &problem_id,
                bool forced) {
    cli::write(cli::fmt::info, "Pulling tests...");

    auto remote_tests = api::polygon::get_problem_tests(creds, problem_id);

    auto is_manual = [](const types::polygon::Test &t) { return t.manual; };
    auto local_sample_tests = config::get_tests_file_names(config::test_type::sample);
    auto l_manual_tests = config::get_tests_file_names(config::test_type::manual);
    l_manual_tests.insert(l_manual_tests.end(), local_sample_tests.begin(),
                          local_sample_tests.end());

    for (const auto &rt : remote_tests | std::views::filter(is_manual)) {
        auto found = std::find(l_manual_tests.begin(), l_manual_tests.end(), rt.file_name);
        if (found != l_manual_tests.end())
            conflicts::solve_files(*found, rt.file_name, rt.input, forced);
    }

    auto is_random = [](const types::polygon::Test &t) { return !t.manual; };
    std::unordered_set<std::string> unique_inputs;
    auto l_random_inputs = config::get_random_tests_inputs();
    for (const auto &rt : remote_tests | std::views::filter(is_random)) {
        const auto cmd = util::split(rt.input);
        std::vector<std::string> args(cmd.begin() + 1, cmd.end());
        const auto args_str = util::join(args);
        unique_inputs.insert(args_str);
    }

    if (not forced)
        unique_inputs.insert(l_random_inputs.begin(), l_random_inputs.end());

    const std::vector<std::string> inputs_result(unique_inputs.begin(), unique_inputs.end());
    config::modify_config_file("tests|random", inputs_result);
}

// API
int run(int argc, char *const argv[]) {
    int option = -1;
    auto forced = false;

    while ((option = getopt_long(argc, argv, "hf", longopts, NULL)) != -1) {
        switch (option) {
        case 'h':
            cli::write(cli::fmt::none, help_message);
            return CP_TOOLS_OK;

        case 'f':
            forced = true;
            break;

        default:
            cli::write(cli::fmt::error, help_message);
            return CP_TOOLS_ERROR_POLYGON_INVALID_OPTION;
        }
    }

    std::string problem_id = "";

    try {
        problem_id = config::get_polygon_problem_id();
    } catch (const exceptions::inexistent_file_error &e) {
        cli::write(cli::fmt::error, e.what());
        return CP_TOOLS_EXCEPTION_INEXISTENT_FILE;
    }

    if (problem_id == "") {
        cli::write(cli::fmt::error, "Couldn't find the problem id in the configuration file.");
        return CP_TOOLS_ERROR_POLYGON_NO_PROBLEM_ID;
    }

    auto config_path = fs::get_default_config_path();
    auto creds = polygon::get_credentials_from_file(config_path);

    auto old_config_path = config::generate_old_config_filename();

    if (forced)
        cli::write(cli::fmt::warning, "Forced update: files will be overwritten");
    else {
        cli::write(cli::fmt::info,
                   "Local " + config::config_path_name + " copied to " + old_config_path);
        fs::copy(config::config_path_name, old_config_path);
    }

    try {
        pull_tool_file("checker", creds, problem_id, forced);
        pull_tool_file("validator", creds, problem_id, forced);
        pull_solutions(creds, problem_id, forced);
        pull_tests(creds, problem_id, forced);
        pull_titles(creds, problem_id);
        pull_infos(creds, problem_id);
        pull_tags(creds, problem_id);
    } catch (const exceptions::polygon_api_error &e) {
        cli::write(cli::fmt::error, e.what());
        cli::write(cli::fmt::warning, "Pull aborted, some files may not be updated correctly");

        if (not forced) {
            cli::write(cli::fmt::warning,
                       "Recovering " + config::config_path_name + " from " + old_config_path);
            fs::copy(old_config_path, config::config_path_name);
        }

        return CP_TOOLS_ERROR_POLYGON_API;
    }

    cli::write(cli::fmt::ok, "Pull completed");

    return CP_TOOLS_OK;
}
} // namespace cptools::commands::polygon::pull