#include <algorithm>
#include <filesystem>

#include "cli/cli.h"
#include "config.h"
#include "error.h"
#include "fs.h"
#include "sh.h"
#include "task.h"
#include "util.h"

namespace cptools::task
{

    const Result
    build_default_solution()
    {
        const auto solution_src = config::get_solutions_file_paths("default")[0];
        const auto dest_file = std::string(CP_TOOLS_BUILD_DIR) + "/solution";
        const auto build_res = sh::build(dest_file, solution_src);
        return build_res;
    }

    iovector
    generate_all_io_files(bool gen_output)
    {
        iovector results;
        for (const auto &type : config::all_test_types)
            {
                const auto res = generate_io_files(type, gen_output);
                results.insert(results.end(), res.begin(), res.end());
            }
        return results;
    }

    void
    create_io_dirs()
    {
        const auto dirs = { std::string(CP_TOOLS_BUILD_DIR) + "/input",
                            std::string(CP_TOOLS_BUILD_DIR) + "/output" };

        for (const auto &dir : dirs)
            {
                const auto res = fs::create_directory(dir);
                if (not res.ok)
                    {
                        cli::write(cli::fmt::error,
                                   "Could not create directory: " + res.error_message);
                        throw std::runtime_error("Could not create directory");
                    }
            }
    }

    iovector
    generate_random_io_files(bool gen_output)
    {
        create_io_dirs();

        const auto tests_filenames = config::get_all_tests_file_names();
        int start = 0;
        for (const auto &path : tests_filenames)
            {
                const auto filename = std::filesystem::path(path).filename();
                start = std::max(start, std::stoi(filename));
            }
        start++;

        iovector results;
        const auto random_inputs = config::get_random_tests_inputs();

        int test_number = start;
        for (const auto &input : random_inputs)
            {
//cli::Trace(std::string("Input: ") + input);
//cli::Trace(std::string("Output file: ") + std::string(output_file));
                const auto input_file = dirs::input_dir / std::to_string(test_number++);
//cli::Trace(std::string("Input file: ") + std::string(input_file));
                sh::execute_program(files::generator_executable, input, "", input_file);
                results.emplace_back(input_file, "");
            }

        if (not gen_output)
            return results;

        const auto generator_path = config::get_tool_file_name(config::tool_type::generator);

        const auto build_res = build_tool(config::tool_type::generator);
        if (not build_res.ok)
            {
                cli::write(cli::fmt::error, "Failed to build generator");
                cli::write_trace(build_res.error_message);
                throw std::runtime_error("Failed to build generator");
            }

        test_number = start;
        for (const auto &input : random_inputs)
            {
                const auto input_file = dirs::input_dir / std::to_string(test_number);
                const auto output_file = dirs::output_dir / std::to_string(test_number);
                sh::execute_program(files::generator_executable, input, "", output_file);
                for (auto &p : results)
                    {
                        if (p.first == input_file)
                            {
                                p.second = output_file.string();
                                break;
                            }
                    }
                test_number++;
            }

        return results;
    }

    iovector
    generate_io_files(const config::test_type &testset, bool gen_output)
    {
        create_io_dirs();

        if (testset == config::test_type::random)
            return generate_random_io_files(gen_output);

        iovector results;
        const auto inputs = config::get_tests_file_names(testset);
        for (const auto &input_path : inputs)
            {
                const auto input_filename = std::filesystem::path(input_path).filename();
                const auto input_dest = dirs::input_dir / input_filename;

                const auto copy_res = fs::copy(input_path, input_dest, true);
                if (not copy_res.ok)
                    {
                        cli::write(cli::fmt::error,
                                   "Failed to copy input file: " + copy_res.error_message);
                        throw std::runtime_error("Failed to copy input file");
                    }

                results.emplace_back(input_dest.string(), "");
            }

        if (not gen_output)
            return results;

        auto res = build_default_solution();
        if (not res.ok)
            {
                cli::write(cli::fmt::error, "Failed to build default solution");
                cli::write_trace(res.error_message);
                throw std::runtime_error("Failed to build default solution");
            }

        for (auto &it : results)
            {
                const auto input_file = it.first;
                const auto filename = std::filesystem::path(input_file).filename();
                const auto output_dest = dirs::output_dir / filename;

                auto solution_res
                    = sh::execute_program(files::solution_executable, "", input_file, output_dest);
                if (not solution_res.ok)
                    {
                        cli::write(cli::fmt::error, "Failed to run solution");
                        cli::write_trace(solution_res.error_message);
                        throw std::runtime_error("Failed to run solution");
                    }
                it.second = output_dest.string();
            }

        return results;
    }

    const Result
    build_tool(config::tool_type tool_type)
    {
        auto program = config::tool_type_tag.at(tool_type);
        auto dest_dir = std::filesystem::path(CP_TOOLS_BUILD_DIR);
        auto dest = dest_dir / program;

        auto fs_res = fs::create_directory(dest_dir);
        if (not fs_res.ok)
            return fs_res;

        auto src_file = config::get_tool_file_name(tool_type);
        if (src_file.empty())
            {
                auto err = "Can't find source for '" + program + "' in config file";
                return make_result(false, CP_TOOLS_ERROR_TASK_INVALID_TOOL, "", err);
            }

        auto res = sh::build(dest, src_file);
        return res;
    }

    const Result
    gen_exe(const std::string &source, const std::string &dest)
    {
        auto dest_dir = std::filesystem::path(CP_TOOLS_BUILD_DIR);

        auto fs_res = fs::create_directory(dest_dir);
        if (not fs_res.ok)
            return fs_res;

        auto program = dest_dir / dest;

        auto removed_result = fs::remove(program);
        if (not removed_result.ok)
            return removed_result;

        auto res = sh::build(program, source);

        if (!res.ok)
            {
                cli::write(cli::fmt::error, "Can't build solution '" + source + "'!");
                cli::write_trace(res.error_message);
                return res;
            }

        return res;
    }
} // namespace cptools::task
