#include <filesystem>
#include <getopt.h>
#include <set>
#include <string>
#include <unordered_map>

#include "cli/cli.h"
#include "commands/polygon/polygon.h"
#include "commands/polygon/pull.h"
#include "config.h"
#include "conflicts.h"
#include "error.h"
#include "exceptions.h"
#include "fs.h"
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
    cli::write(cli::message_type::info, "Pulling " + tool_name + "...");
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
    cli::write(cli::message_type::info, "Pulling solutions...");
    auto solutions = api::polygon::get_problem_solutions(creds, problem_id);

    for (const auto &solution : solutions) {
        auto files_with_same_tag = config::get_solutions_file_names(solution.tag);
        auto file_content =
            api::polygon::get_problem_file(solution.name, "solution", creds, problem_id);
        auto remote_file_name = std::filesystem::path(solution.name).filename().string();

        auto file_exists = std::find(files_with_same_tag.begin(), files_with_same_tag.end(),
                                     remote_file_name) != files_with_same_tag.end();

        if (!file_exists) {
            fs::overwrite_file(remote_file_name, "");
        }
        auto new_file_path =
            conflicts::solve_files(remote_file_name, remote_file_name, file_content, forced);
        config::insert_solution_file_name(solution.tag, new_file_path);
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

    cli::write(cli::message_type::info, "Pulling problem title...");
    auto statements = api::polygon::get_problem_statement(creds, problem_id);

    std::unordered_map<std::string, std::string> titles;

    for (const auto &statement : statements) {
        auto lang = statement.language;
        titles.emplace(lang, statement.title);
    }

    config::modify_config_file("problem|title", titles);
}

void pull_infos(const types::polygon::Credentials &creds, const std::string &problem_id) {
    cli::write(cli::message_type::info, "Pulling problem informations...");
    auto info = api::polygon::get_problem_information(creds, problem_id);

    config::modify_config_file("problem|memory_limit", info.memory_limit);
    config::modify_config_file("problem|time_limit", info.time_limit);
}

// API
int run(int argc, char *const argv[], std::ostream &, std::ostream &) {
    int option = -1;
    auto forced = false;

    while ((option = getopt_long(argc, argv, "hf", longopts, NULL)) != -1) {
        switch (option) {
        case 'h':
            cli::write(cli::message_type::none, help_message);
            return CP_TOOLS_OK;

        case 'f':
            forced = true;
            break;

        default:
            cli::write(cli::message_type::error, help_message);
            return CP_TOOLS_ERROR_POLYGON_INVALID_OPTION;
        }
    }

    std::string problem_id = "";

    try {
        problem_id = config::get_polygon_problem_id();
    } catch (const exceptions::inexistent_file_error &e) {
        cli::write(cli::message_type::error, e.what());
        return CP_TOOLS_EXCEPTION_INEXISTENT_FILE;
    }

    if (problem_id == "") {
        cli::write(cli::message_type::error,
                   "Couldn't find the problem id in the configuration file.");
        return CP_TOOLS_ERROR_POLYGON_NO_PROBLEM_ID;
    }

    auto config_path = fs::get_default_config_path();
    auto creds = polygon::get_credentials_from_file(config_path);

    auto ext = ".old";
    auto old_config_path = config::config_path_name + ext;

    if (forced)
        cli::write(cli::message_type::warning, "Forced update: files will be overwritten");
    else {
        cli::write(cli::message_type::info,
                   "Local " + config::config_path_name + " copied to " + old_config_path);
        fs::copy(config::config_path_name, old_config_path);
    }

    try {
        pull_tool_file("checker", creds, problem_id, forced);
        pull_tool_file("validator", creds, problem_id, forced);
        pull_solutions(creds, problem_id, forced);
        pull_titles(creds, problem_id);
        pull_infos(creds, problem_id);
    } catch (const exceptions::polygon_api_error &e) {
        cli::write(cli::message_type::error, e.what());
        cli::write(cli::message_type::warning,
                   "Pull aborted, some files may not be updated correctly");

        if (not forced) {
            cli::write(cli::message_type::warning,
                       "Recovering " + config::config_path_name + " from " + old_config_path);
            fs::copy(old_config_path, config::config_path_name);
        }

        return CP_TOOLS_ERROR_POLYGON_API;
    }

    cli::write(cli::message_type::ok, "Pull completed");

    return CP_TOOLS_OK;
}
} // namespace cptools::commands::polygon::pull