#include <algorithm>
#include <filesystem>

#include "cli/cli.h"
#include "config.h"
#include "dirs.h"
#include "error.h"
#include "fs.h"
#include "sh.h"
#include "task.h"
#include "util.h"

namespace cptools::task {

std::vector<std::pair<std::string, std::string>> generate_io_files(const std::string &testset,
                                                                   bool gen_output) {

    std::vector<std::string> sets{"samples", "manual", "random"};

    auto it = std::find(sets.begin(), sets.end(), testset);

    if (it != sets.end()) {
        sets.clear();
        sets.push_back(testset);
    }

    auto input_dir{std::string(CP_TOOLS_BUILD_DIR) + "/input/"};
    auto output_dir{std::string(CP_TOOLS_BUILD_DIR) + "/output/"};
    auto program{std::string(CP_TOOLS_BUILD_DIR) + "/solution"};

    auto config = config::read_config_file();
    auto source = util::get_json_value(config, "solutions|default", std::string("ERROR"));

    auto directories = {input_dir, output_dir};
    for (auto &dir : directories) {
        auto fs_res = fs::create_directory(dir);
        if (not fs_res.ok) {
            cli::write(cli::fmt::error, fs_res.error_message);
            return {};
        }
    }

    if (source == "ERROR") {
        cli::write(cli::fmt::error, "Default solution file not found!");
        return {};
    }

    auto res = sh::build(program, source);

    if (!res.ok) {
        cli::write(cli::fmt::error, "Can't compile solution '" + source + "'!");
        cli::write_trace(res.error_message);
        return {};
    }

    std::vector<std::pair<std::string, std::string>> io_files;
    int next = 1;

    for (auto s : sets) {
        if (s == "random") {
            source = util::get_json_value(config, "tools|generator", std::string("ERROR"));

            if (source == "ERROR") {
                cli::write(cli::fmt::error, "Generator file not found!");
                return {};
            }

            auto generator = std::string(CP_TOOLS_BUILD_DIR) + "/generator";

            res = sh::build(generator, source);

            if (!res.ok) {
                cli::write(cli::fmt::error, "Can't compile generator '" + source + "'!");
                cli::write_trace(res.error_message);
                return {};
            }

            auto inputs = config::get_random_tests_inputs();

            for (auto parameters : inputs) {
                std::string dest{input_dir + std::to_string(next++)};

                auto res = sh::execute_program(generator, parameters, "", dest);

                if (!res.ok) {
                    cli::write(cli::fmt::error,
                               "Error generating '" + dest + "' with parameters " + parameters);
                    cli::write_trace(res.error_message);
                    return {};
                }

                io_files.emplace_back(std::make_pair(dest, ""));
            }
        } else {
            auto inputs =
                util::get_json_value(config, "tests|" + s, std::map<std::string, std::string>{});

            for (auto [input, comment] : inputs) {
                std::string dest{input_dir + std::to_string(next++)};

                auto res = fs::copy(input, dest, true);
                if (not res.ok) {
                    cli::write(cli::fmt::error, res.error_message);
                    return {};
                }

                io_files.emplace_back(std::make_pair(dest, ""));
            }
        }
    }

    if (gen_output) {
        for (int i = 1; i < next; ++i) {
            std::string input{io_files[i - 1].first};
            std::string output{output_dir + std::to_string(i)};

            auto res = sh::execute_program(program, "", input, output);

            if (!res.ok) {
                cli::write(cli::fmt::error, "Can't generate output for input '" + input + "'!");
                cli::write_trace(res.error_message);
                return {};
            }

            io_files[i - 1].second = output;
        }
    }

    return io_files;
}

const Result build_tool(config::tool_type tool_type) {
    auto program = config::tool_type_tag.at(tool_type);
    auto dest_dir = std::filesystem::path(CP_TOOLS_BUILD_DIR);
    auto dest = dest_dir / program;

    auto fs_res = fs::create_directory(dest_dir);
    if (not fs_res.ok)
        return fs_res;

    auto src_file = config::get_tool_file_name(tool_type);
    if (src_file.empty()) {
        auto err = "Can't find source for '" + program + "' in config file";
        return make_result(false, CP_TOOLS_ERROR_TASK_INVALID_TOOL, "", err);
    }

    auto res = sh::build(dest, src_file);
    return res;
}

const Result gen_exe(const std::string &source, const std::string &dest) {
    auto dest_dir = std::filesystem::path(CP_TOOLS_BUILD_DIR);

    auto fs_res = fs::create_directory(dest_dir);
    if (not fs_res.ok)
        return fs_res;

    auto program = dest_dir / dest;

    auto removed_result = fs::remove(program);
    if (not removed_result.ok)
        return removed_result;

    auto res = sh::build(program, source);

    if (!res.ok) {
        cli::write(cli::fmt::error, "Can't build solution '" + source + "'!");
        cli::write_trace(res.error_message);
        return res;
    }

    return res;
}
} // namespace cptools::task
