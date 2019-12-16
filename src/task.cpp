#include <algorithm>

#include "sh.h"
#include "task.h"
#include "dirs.h"
#include "error.h"
#include "config.h"

namespace cptools::task {

    std::vector<std::pair<std::string, std::string>> generate_io_files(const std::string& testset,
        std::ostream&, std::ostream& err)    
    {
        std::vector<std::string> sets { "samples", "manual", "random" };

        auto it = std::find(sets.begin(), sets.end(), testset);

        if (it != sets.end())
        {
            sets.clear();
            sets.push_back(testset);
        }

        std::vector<std::pair<std::string, std::string>> io_files;

        auto input_dir { std::string(CP_TOOLS_BUILD_DIR) + "/input/" };
        auto output_dir { std::string(CP_TOOLS_BUILD_DIR) + "/output/" };
        auto program { std::string(CP_TOOLS_BUILD_DIR) + "/solution" };

        auto config = cptools::config::read("config.json");
        auto source = "solutions/" + cptools::config::get(config, "solutions|default", 
            std::string("ERROR"));

        if (source == "solutions/ERROR")
        {
            err << "[generate_io_files] Default solution file not found!\n";
            return { };
        }

        int next = 1;

        auto rc = cptools::sh::make_dir(input_dir);

        if (rc != CP_TOOLS_OK)
        {
            err << "[generate_io_files] Can't create '" << input_dir << "'\n";
            return { };
        }

        rc = cptools::sh::make_dir(output_dir);

        if (rc != CP_TOOLS_OK)
        {
            err << "[generate_io_files] Can't create '" << output_dir << "'\n";
            return { };
        }

        rc = cptools::sh::build(program, source);

        if (rc != CP_TOOLS_OK)
        {
            err << "[generate_io_files] Can't compile solution '" << source << "'\n";
            return { };
        }

        for (auto s : sets)
        {
err << "Generating testes on set '" << s << "'\n";
            if (s == "random")
            {
                // TODO gerar os tests randômicos via gerador
            } else
            {
                auto inputs = cptools::config::get(config, "tests|" + s, std::map<std::string, std::string> {});

                for (auto [input, comment] : inputs)
                {
                    std::string dest { input_dir + std::to_string(next) };
err << "Copy " << input << " on " << dest << '\n';
                    rc = cptools::sh::copy_file(dest, input);

                    if (rc != CP_TOOLS_OK)
                    {
                        err << "[generate_io_files] Can't copy input '" << input << " on '"
                            << dest << "'\n";
                        return { };
                    }

                    std::string output { output_dir + std::to_string(next) };

                    rc = cptools::sh::process(input, program, output);

                    if (rc != CP_TOOLS_OK)
                    {
                        err << "[generate_io_files] Can't generate output for input '" 
                            << input << "'\n";
                        return { };
                    }

                    io_files.emplace_back(std::make_pair(dest, output));
                    next++;
                }
            }
        }

        return io_files;
    }

}
