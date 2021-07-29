#include <filesystem>
#include <getopt.h>
#include <set>
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
    auto polygon_file_name = api::polygon::get_problem_file_name(tool_name, creds, problem_id);
    auto local_file_name = config::get_tool_file_name(tool_name);

    auto local_file_path = std::filesystem::path{local_file_name};
    auto polygon_file_path = std::filesystem::path{polygon_file_name};

    auto new_file_path = std::filesystem::path{local_file_name};
    new_file_path.replace_filename(polygon_file_path.filename());

    auto file_content =
        api::polygon::get_problem_file(polygon_file_path, tool_name, creds, problem_id);

    auto local_file_sha_512 = fs::sha_512_file(local_file_path);
    auto polygon_file_sha_512 = util::sha_512(file_content);

    auto different_hashes = local_file_sha_512 != polygon_file_sha_512;
    auto equal_names = fs::equivalent(local_file_path.filename(), polygon_file_path.filename()).ok;

    if (forced) {
        fs::remove(local_file_path);
        fs::overwrite_file(new_file_path, file_content);
    } else if (different_hashes and equal_names) {
        // TODO: conflict, same names but different content
    } else if (not equal_names) {
        fs::overwrite_file(new_file_path, file_content);
        if (not different_hashes) {
            fs::remove(local_file_path);
        }
    }
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
    auto solutions = api::polygon::get_problem_solutions(creds, problem_id);

    if (!forced)
        return; // TODO

    for (const auto &solution : solutions) {
        auto files_with_same_tag = config::get_solutions_file_names(solution.tag);
        auto file_content =
            api::polygon::get_problem_file(solution.name, "solution", creds, problem_id);

        fs::overwrite_file(solution.name, file_content);

        auto found =
            std::find(files_with_same_tag.begin(), files_with_same_tag.end(), solution.name);

        if (solution.tag == "default") {
            config::modify_config_file("solutions|default", solution.name);
        } else if (found == files_with_same_tag.end()) {
            std::set<std::string> files(files_with_same_tag.begin(), files_with_same_tag.end());
            files.insert(solution.name);

            config::modify_config_file("solutions|" + solution.tag, files);
        }
    }
}

// API
int run(int argc, char *const argv[], std::ostream &out, std::ostream &err) {
    int option = -1;
    auto forced = false;

    while ((option = getopt_long(argc, argv, "hf", longopts, NULL)) != -1) {
        switch (option) {
        case 'h':
            out << help_message << "\n";
            return CP_TOOLS_OK;

        case 'f':
            forced = true;
            break;

        default:
            err << help_message << "\n";
            return CP_TOOLS_ERROR_POLYGON_INVALID_OPTION;
        }
    }

    std::string problem_id = "";

    try {
        problem_id = config::get_polygon_problem_id();
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
        pull_tool_file("checker", creds, problem_id, forced);
        pull_tool_file("validator", creds, problem_id, forced);
        pull_solutions(creds, problem_id, forced);
    } catch (const exceptions::polygon_api_error &e) {
        err << message::failure(e.what());
        return CP_TOOLS_ERROR_POLYGON_API;
    }

    out << message::success("Pull completed");

    return CP_TOOLS_OK;
}
} // namespace cptools::commands::polygon::pull