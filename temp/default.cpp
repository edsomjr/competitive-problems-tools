#include <sstream>
#include <iostream>

#include "plugin.h"
#include "default.h"
#include "pluginmanager.h"


Default::Default() : Plugin("default", "Format, test and pack competitive programming problems.", "")
{
    _options.emplace_back('h', "help", false, "", "");
    _options.emplace_back('v', "version", false, "", "");
}

std::string Default::help() const
{
    std::ostringstream oss( Plugin::help() );

    oss << "Commands:\n";
    auto manager = PluginManager::get_instance();

    // symbol lookup error: ./plugins/default.so: undefined symbol: _ZN13PluginManager12get_instanceEv
    std::cout << manager->get_plugins_briefs() << '\n';

    oss << manager->get_plugins_briefs();

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
