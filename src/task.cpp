#include <algorithm>

#include "sh.h"
#include "task.h"
#include "dirs.h"
#include "error.h"
#include "config.h"

using std::to_string;

namespace cptools::task {

    namespace tools {
        const int CHECKER = 1;
        const int VALIDATOR = 2;
        const int GENERATOR = 4;
        const int INTERACTOR = 8;
        const int ALL = CHECKER | VALIDATOR | GENERATOR | INTERACTOR;
    }

    std::vector<std::pair<std::string, std::string>> generate_io_files(const std::string& testset,
        std::ostream&, std::ostream& err, bool gen_output)    
    {
        std::vector<std::string> sets { "samples", "manual", "random" };

        auto it = std::find(sets.begin(), sets.end(), testset);

        if (it != sets.end())
        {
            sets.clear();
            sets.push_back(testset);
        }

        auto input_dir { std::string(CP_TOOLS_BUILD_DIR) + "/input/" };
        auto output_dir { std::string(CP_TOOLS_BUILD_DIR) + "/output/" };
        auto program { std::string(CP_TOOLS_BUILD_DIR) + "/solution" };

        auto config = cptools::config::read("config.json");
        auto source = "solutions/" + cptools::config::get(config, "solutions|default", 
            std::string("ERROR"));

        string error;
        auto rc = cptools::sh::make_dir(input_dir, error);

        if (rc != CP_TOOLS_OK)
        {
            err << "[generate_io_files] Can't create '" << input_dir << "'\n";
            return { };
        }

        error = "";
        rc = cptools::sh::make_dir(output_dir, error);

        if (rc != CP_TOOLS_OK)
        {
            err << "[generate_io_files] Can't create '" << output_dir << "'\n";
            return { };
        }

        if (source == "solutions/ERROR")
        {
            err << "[generate_io_files] Default solution file not found!\n";
            return { };
        }

        rc = cptools::sh::build(program, source);

        if (rc != CP_TOOLS_OK)
        {
            err << "[generate_io_files] Can't compile solution '" << source << "'\n";
            return { };
        }

        std::vector<std::pair<std::string, std::string>> io_files;
        int next = 1;

        for (auto s : sets)
        {
            if (s == "random")
            {
                source = cptools::config::get(config, "tools|generator", std::string("ERROR"));

                if (source == "tools/ERROR")
                {
                    err << "[generate_io_files] Generator file not found!\n";
                    return { };
                }

                auto generator = std::string(CP_TOOLS_BUILD_DIR) + "/generator";

                rc = cptools::sh::build(generator, source);

                if (rc != CP_TOOLS_OK)
                {
                    err << "[generate_io_files] Can't compile generator '" << source << "'\n";
                    return { };
                }

                auto inputs = cptools::config::get(config, "tests|random", 
                    std::vector<std::string> {});

                for (auto parameters : inputs)
                {
                    std::string dest { input_dir + std::to_string(next++) };

                    auto rc = sh::exec(generator, parameters, dest);

                    if (rc != CP_TOOLS_OK)
                    {
                        err << "[generate_io_files] Error generating " << dest 
                            << " with parameters '" << parameters << "'\n";

                        return { };
                    }

                    io_files.emplace_back(std::make_pair(dest, ""));
                }
            } else
            {
                auto inputs = cptools::config::get(config, "tests|" + s, 
                    std::map<std::string, std::string> {});

                for (auto [input, comment] : inputs)
                {
                    std::string dest { input_dir + std::to_string(next++) };

                    rc = cptools::sh::copy_file(dest, input);

                    if (rc != CP_TOOLS_OK)
                    {
                        err << "[generate_io_files] Can't copy input '" << input << " on '"
                            << dest << "'\n";
                        return { };
                    }

                    io_files.emplace_back(std::make_pair(dest, ""));
                }
            }
        }

        if (gen_output)
        {
            for (int i = 1; i < next; ++i)
            {
                std::string input { io_files[i - 1].first };
                std::string output { output_dir + std::to_string(i) };

                rc = cptools::sh::process(input, program, output);

                if (rc != CP_TOOLS_OK)
                {
                    err << "[generate_io_files] Can't generate output for input '" << input << "'\n";
                    return { };
                }

                io_files[i - 1].second = output;
            }
        }

        return io_files;
    }

    int build_tools(string& error, int tools, const string& where)
    {
        auto dest_dir { where + "/" + CP_TOOLS_BUILD_DIR + "/" };

        auto rc = sh::make_dir(dest_dir, error);

        if (rc != CP_TOOLS_OK)
            return rc;

        auto config = cptools::config::read("config.json");

        for (int mask = 1; mask <= tools; mask <<= 1)
        {
            int tool = tools & mask;
            string program = "";

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
                error = "Invalid tool required: (" + to_string(tools) + ")";
                return CP_TOOLS_ERROR_TASK_INVALID_TOOL;
            }

            auto source = cptools::config::get(config, "tools|" + program, std::string(""));

            if (source.empty())
            {
                error = "Can't find source for '" + program + "' in config file";
                return CP_TOOLS_ERROR_TASK_INVALID_TOOL;
            }

            auto dest = dest_dir + program;

            auto rc = cptools::sh::build(dest, source);

            if (rc != CP_TOOLS_OK)
            {
                error = "Can't compile '" + source + "'\n";
                return rc;
            }
        }

        return CP_TOOLS_OK;
    }

}
