#include <iostream>

#include "fs.h"
#include "dirs.h"
#include "plugin.h"
#include "message.h"

class Init : public Plugin {
public:
    Init();

    int execute(const Args& args);
};

Init::Init() : Plugin("init", "Initialize an empty directory with template files.", "")
{
    _description = _brief + " If a file already exists, it is not overridden.";

    _options.emplace_back('h', "help", false, "", "Generates this help message.");
    _options.emplace_back('o', "output", true, "output_dir", "Directory to be initialized.");
}

static int
copy_template_files(const std::string &dest)
{
    std::cout << cptools::message::info("Initializing directory '" + dest + "' ...\n");

    cptools::fs::create_directory(dest);

    // Copy templates to the dest
    cptools::fs::copy_recursive(CP_TOOLS_PROBLEM_TEMPLATE_DIR, dest);

    return 0;
}

int
Init::execute(const Args& args)
{
    if (args.count("help"))
    {
        std::cout << help() << '\n';
        return 0;
    }

    std::string dest = args.count("output") ? args.at("output") : std::string(".");

    return copy_template_files(dest);
}

// dynamic function that is called by the plugin manager to build the plugin
extern "C" Plugin *
create()
{
    return new Init();
}

// dynamic function that is called by the plugin manager to destroy the plugin
extern "C" void
destroy(Plugin *p)
{
    delete p;
}
