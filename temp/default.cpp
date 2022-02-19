#include <iostream>

#include "plugin.h"
#include "default.h"


Default::Default() : Plugin("default", "Default command that runs when user "
                                       "does not type any subcommands ", "")
{
    _options.emplace_back('h', "help", false, "", "");
    _options.emplace_back('v', "version", false, "", "");
}

int
Default::execute(const Args& args)
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
    return new Default();
}

// dynamic function that is called by the plugin manager to destroy the plugin
extern "C" void
destroy(Plugin *p)
{
    delete p;
}
