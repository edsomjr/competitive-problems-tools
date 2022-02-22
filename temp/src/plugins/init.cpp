#include <iostream>

#include "plugin.h"

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

int
Init::execute(const Args& args)
{
    if (args.count("help"))
    {
        std::cout << help() << '\n';
        return 0;
    }

    return 0;
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
