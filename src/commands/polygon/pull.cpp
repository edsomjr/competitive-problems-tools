#include <filesystem>
#include <getopt.h>
#include <string>

#include "commands/polygon/polygon.h"
#include "commands/polygon/pull.h"
#include "config.h"
#include "error.h"
#include "exceptions.h"
#include "fs.h"
#include "message.h"
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
)message"};

static struct option longopts[] = {{"help", no_argument, NULL, 'h'}};

// Functions
void pull_tool_file(const std::string tool_name, const types::polygon::Credentials &creds,
                    const std::string &problem_id) {
    auto polygon_file_name = api::polygon::get_problem_file_name(tool_name, creds, problem_id);
    auto file_content =
        api::polygon::get_problem_file(polygon_file_name, tool_name, creds, problem_id);
    auto config = config::read_config_file();
    auto local_file_name = config::pull_tool_file_name(config, tool_name);
    fs::overwrite_file(local_file_name, file_content);
}

/**
 * @brief Get the solutions objects from the Polygon API and writes them to the filesystem.
 *
 * @param creds credentials data structure.
 * @param problem_id` Polygon's problem ID.
 */
void pull_solutions(const types::polygon::Credentials &creds, const std::string &problem_id) {
    auto solutions = api::polygon::get_problem_solutions(creds, problem_id);

    for (const auto &solution : solutions) {
        auto files_with_same_tag = config::get_solutions_file_names(s.tag);
        auto file_content = 
            api::polygon::get_problem_file(solution.name, "solution", creds, problem_id);

        fs::overwrite_file(solution.name, file_content);

        auto found = std::find(files_with_same_tag.begin(), files_with_same_tag.end(), s.name);

        if (solution.tag == "default") {
            config::modify_config_file("solutions|default", s.name);
        } else if (found == files_with_same_tag.end()) {
            config::modify_config_file("solutions|" + s.tag, s.name, "add");
        }
    }
}

/**
 * @brief Receives a Polygon problem solution and saves it to the local file system.
 *
 * @param s solution data structure.
 * @param creds credentials data structure.
 * @param problem_id Polygon's problem ID.
 */
void save_solution(const types::polygon::Solution &s, const types::polygon::Credentials &creds,
                   const std::string &problem_id) {
    auto files_with_same_tag = config::get_solutions_file_names(s.tag);
    auto file_content = api::polygon::get_problem_file(s.name, "solution", creds, problem_id);

    fs::overwrite_file(s.name, file_content);

    auto found = std::find(files_with_same_tag.begin(), files_with_same_tag.end(), s.name);
    if (found != files_with_same_tag.end()) {
        config::modify_config_file("solutions|" + s.tag, s.name, "add");
    }
}

// API
int run(int argc, char *const argv[], std::ostream &out, std::ostream &err) {
    int option = -1;
    while ((option = getopt_long(argc, argv, "h", longopts, NULL)) != -1) {
        switch (option) {
        case 'h':
            out << help_message << "\n";
            return CP_TOOLS_OK;

        default:
            err << help_message << "\n";
            return CP_TOOLS_ERROR_POLYGON_INVALID_OPTION;
        }
    }

    std::string problem_id = "";

    try {
        auto config_json = config::read_config_file();
        problem_id = config::get_polygon_problem_id(config_json);
    } catch (const exceptions::inexistent_file_error &e) {
        err << message::failure(e.what()) << "\n";
        return CP_TOOLS_EXCEPTION_INEXISTENT_FILE;
    }

    if (problem_id == "") {
        err << message::failure("Couldn't find the problem id in the configuration file.");
        return CP_TOOLS_ERROR_POLYGON_NO_PROBLEM_ID;
    }

    auto config_path = fs::get_default_config_path();
    auto creds = polygon::get_credentials_from_file(config_path);

    try {
        pull_tool_file("checker", creds, problem_id);
        pull_tool_file("validator", creds, problem_id);
        pull_solutions(creds, problem_id);
        // get_tests();
    } catch (const exceptions::polygon_api_error &e) {
        err << message::failure(e.what());
        return CP_TOOLS_ERROR_POLYGON_API;
    }

    out << message::success("Pull completed");

    return CP_TOOLS_OK;
}
} // namespace cptools::commands::polygon::pull