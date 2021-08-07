#include <algorithm>

#include "config.h"
#include "dirs.h"
#include "error.h"
#include "fs.h"
#include "logger/logger.h"
#include "logger/message.h"
#include "sh.h"
#include "task.h"
#include "util.h"

namespace cptools::task {

std::vector<std::pair<std::string, std::string>>
generate_io_files(const std::string &testset, std::ostream &, std::ostream &err, bool gen_output) {

    std::vector<std::string> sets{"samples", "manual", "random"};

    auto it = std::find(sets.begin(), sets.end(), testset);

    if (it != sets.end()) {
        sets.clear();
        sets.push_back(testset);
    }

    auto input_dir{std::string(CP_TOOLS_BUILD_DIR) + "/input/"};
    auto output_dir{std::string(CP_TOOLS_BUILD_DIR) + "/output/"};
    auto program{std::string(CP_TOOLS_BUILD_DIR) + "/solution"};

    auto config = cptools::config::read_config_file();
    auto source = cptools::util::get_json_value(config, "solutions|default", std::string("ERROR"));

    auto directories = {input_dir, output_dir};
    for (auto &dir : directories) {
        auto fs_res = fs::create_directory(dir);
        if (not fs_res.ok) {
            err << logger::message::failure(fs_res.error_message);
            return {};
        }
    }

    if (source == "ERROR") {
        err << logger::message::failure("Default solution file not found!\n");
        return {};
    }

    auto res = cptools::sh::build(program, source);

    if (res.rc != CP_TOOLS_OK) {
        err << logger::message::failure("Can't compile solution '" + source + "'!") << "\n";
        logger::log(logger::TRACE, res.output);
        return {};
    }

    std::vector<std::pair<std::string, std::string>> io_files;
    int next = 1;

    for (auto s : sets) {
        if (s == "random") {
            source = cptools::util::get_json_value(config, "tools|generator", std::string("ERROR"));

            if (source == "tools/ERROR") {
                err << logger::message::failure("Generator file not found!\n");
                return {};
            }

            auto generator = std::string(CP_TOOLS_BUILD_DIR) + "/generator";

            res = cptools::sh::build(generator, source);

            if (res.rc != CP_TOOLS_OK) {
                err << logger::message::failure("Can't compile generator '" + source + "'!")
                    << "\n";
                logger::log(logger::TRACE, res.output);
                return {};
            }

            auto inputs =
                cptools::util::get_json_value(config, "tests|random", std::vector<std::string>{});

            for (auto parameters : inputs) {
                std::string dest{input_dir + std::to_string(next++)};

                auto res = sh::execute(generator, parameters, "", dest);

                if (res.rc != CP_TOOLS_OK) {
                    err << logger::message::failure("Error generating '" + dest +
                                                    "' with parameters " + parameters)
                        << "\n";
                    logger::log(logger::TRACE, res.output);
                    return {};
                }

                io_files.emplace_back(std::make_pair(dest, ""));
            }
        } else {
            auto inputs = cptools::util::get_json_value(config, "tests|" + s,
                                                        std::map<std::string, std::string>{});

            for (auto [input, comment] : inputs) {
                std::string dest{input_dir + std::to_string(next++)};

                auto res = fs::copy(input, dest, true);
                if (not res.ok) {
                    err << logger::message::failure(res.error_message);
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

            auto res = cptools::sh::execute(program, "", input, output);

            if (res.rc != CP_TOOLS_OK) {
                err << logger::message::failure("Can't generate output for input '" + input + "'!")
                    << "\n";
                logger::log(logger::TRACE, res.output);
                return {};
            }

            io_files[i - 1].second = output;
        }
    }

    return io_files;
}

int build_tools(std::string &error, int tools, const std::string &where) {
    auto dest_dir{where + "/" + CP_TOOLS_BUILD_DIR + "/"};

    auto fs_res = fs::create_directory(dest_dir);
    if (not fs_res.ok) {
        return fs_res.rc;
    }

    auto config = cptools::config::read_config_file();

    for (int mask = 1; mask <= tools; mask <<= 1) {
        int tool = tools & mask;
        std::string program = "";

        switch (tool) {
        case tools::VALIDATOR:
            program = "validator";
            break;

        case tools::CHECKER:
            program = "checker";
            break;

        case tools::GENERATOR:
            program = "generator";
            break;

        case tools::INTERACTOR:
            program = "interactor";
            break;

        default:
            error = "Invalid tool required: (" + std::to_string(tools) + ")";
            return CP_TOOLS_ERROR_TASK_INVALID_TOOL;
        }

        auto source = cptools::util::get_json_value(config, "tools|" + program, std::string(""));

        if (source.empty()) {
            error = "Can't find source for '" + program + "' in config file";
            return CP_TOOLS_ERROR_TASK_INVALID_TOOL;
        }

        auto dest = dest_dir + program;

        auto res = cptools::sh::build(dest, source);

        if (res.rc != CP_TOOLS_OK) {
            // TODO
            error + logger::message::failure("Can't compile '" + source + "'!") + "\n";
            error + logger::message::trace(res.output) + '\n';
            return res.rc;
        }
    }

    return CP_TOOLS_OK;
}

int gen_exe(std::string &error, const std::string &source, const std::string &dest,
            const std::string &where) {
    auto dest_dir{where + "/" + CP_TOOLS_BUILD_DIR + "/"};

    auto fs_res = fs::create_directory(dest_dir);
    if (not fs_res.ok) {
        return fs_res.rc;
    }

    auto program{dest_dir + dest};

    auto removed_result = fs::remove(program);
    if (not removed_result.ok) {
        error += logger::message::failure(removed_result.error_message);
        return removed_result.rc;
    }

    auto res = sh::build(program, source);

    if (res.rc != CP_TOOLS_OK) {
        // TODO
        error += logger::message::failure("Can't build solution '" + source + "'!") + "\n";
        error += logger::message::trace(res.output) + '\n';
        return res.rc;
    }

    return CP_TOOLS_OK;
}
} // namespace cptools::task
