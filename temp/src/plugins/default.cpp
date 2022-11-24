#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

#include "defs.h"
#include "plugin.h"
#include "default.h"
#include "pluginmanager.h"


Default::Default() : Plugin("default", "Format, test and pack competitive programming problems.", "")
{
    _options.emplace_back('h', "help", false, "", "Print this help text");
    _options.emplace_back('v', "version", false, "", "Print version information and quit");
}

std::string
Default::help() const
{
    auto& manager = PluginManager::get_instance();
    auto plugins_briefs =  manager.get_plugins_briefs();

    std::ostringstream oss;
   
    oss << Plugin::help();
    oss << "\n\nCommands:\n\n";

    for (auto [name, description] : plugins_briefs)
    {
        if(name == "default")
            continue;

        size_t width = 24;

        width -= (name.size() + 1);
        oss << "  " << name << std::string(width, ' ') << description << '\n';
    }

    return oss.str();
}

static const std::string version_header { NAME " " VERSION "\n" };

static const std::string version_body {
R"body(License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

Written by Edson Alves.)body"};

std::string
Default::version()
{
    return version_header + version_body;
}

int Default::execute(const Args& args)
{
    if (args.count("version"))
        std::cout << version() << '\n';
    else
        std::cout << help() << '\n';

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
