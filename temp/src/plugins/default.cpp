#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

#include "plugin.h"
#include "default.h"
#include "pluginmanager.h"


Default::Default() : Plugin("default", "Format, test and pack competitive programming problems.", "")
{
    _options.emplace_back('h', "help", false, "", "Print this help text");
    _options.emplace_back('v', "version", false, "", "Print version information and quit");
}

std::string
Default::help()
{
    auto& manager = PluginManager::get_instance();
    auto plugins_briefs =  manager.get_plugins_briefs();

    std::ostringstream oss;

    // Dirty trick to hide plugin original name from help text
    // TODO: find a better way
    // To do this I had to take the const out of the help() method
    std::string tmp;
    std::swap(tmp, _command);
    oss << Plugin::help();
    std::swap(tmp, _command);

    oss << "\n\nCommands:\n\n";

    for (auto [name, description] : plugins_briefs)
    {
        if(name == "default") continue;
        size_t width = 24;

        width -= (name.size() + 1);
        oss << "  " << name << std::string(width, ' ') << description << '\n';
    }

    return oss.str();
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
