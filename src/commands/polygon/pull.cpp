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
void get_tool_file(const std::string tool_name, const types::polygon::Credentials &creds,
                   const std::string &problem_id) {
    auto polygon_file_name = api::polygon::get_problem_file_name(tool_name, creds, problem_id);
    auto file_content =
        api::polygon::get_problem_file(polygon_file_name, tool_name, creds, problem_id);
    auto config = config::read_config_file();
    auto local_file_name = config::get_tool_file_name(config, tool_name);
    fs::overwrite_file(local_file_name, file_content);
}

void get_solutions(const types::polygon::Credentials &creds, const std::string &problem_id) {
    auto solutions = api::polygon::get_problem_solutions(creds, problem_id);
    for (const auto &solution : solutions) {
        save_solution(solution, creds, problem_id);
    }
}

void save_solution(const types::polygon::Solution &s, const types::polygon::Credentials &creds,
                   const std::string &problem_id) {
    auto config = config::read_config_file();
    auto files_with_same_tag = config::get_solutions_file_names(config, s.tag);
    auto file_content = api::polygon::get_problem_file(s.name, "solution", creds, problem_id);

    // TODO: if the solution has tag default (main solution) and the config.json
    // default solution is not the same name, the overwrite doesnt happen.
    // It is also possible for the default to have an empty string as return.

    for (const auto &file_name : files_with_same_tag) {
        auto splitted_file_name = util::split(file_name, '/');
        auto real_file_name = splitted_file_name.back();
        if (real_file_name != s.name)
            continue;
        fs::overwrite_file(file_name, file_content);
    }

    // TODO: if got here then that's a new file, we have to add to config.json
    // right tag, and then write the content to it. If the problem's tag is
    // default we overwrite the value instead of appending.
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
        err << message::failure(e.what());
        return CP_TOOLS_EXCEPTION_INEXISTENT_FILE;
    }

    if (problem_id == "") {
        err << message::failure("Couldn't find the problem id in the configuration file.");
        return CP_TOOLS_ERROR_POLYGON_NO_PROBLEM_ID;
    }

    auto config_path = fs::get_default_config_path();
    auto creds = polygon::get_credentials_from_file(config_path);

    try {
        get_tool_file("checker", creds, problem_id);
        get_tool_file("validator", creds, problem_id);
        get_solutions(creds, problem_id);
        // get_tests();
    } catch (const exceptions::polygon_api_error &e) {
        err << message::failure(e.what());
        return CP_TOOLS_ERROR_POLYGON_API;
    }

    out << message::success("Pull completed");

    return CP_TOOLS_OK;
}
} // namespace cptools::commands::polygon::pull