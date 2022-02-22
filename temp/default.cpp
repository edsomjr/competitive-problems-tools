#include <sstream>
#include <iostream>
#include <algorithm>

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
    std::ostringstream oss;
    oss << Plugin::help();

    oss << "\n\nCommands:\n\n";
    auto manager = PluginManager::get_instance();

    oss << manager->get_plugins_briefs();

    // Remove qualquer referência ao nome do plugin ("default")
    return strip_plugin_name_from_message(oss.str());
}

std::string
Default::strip_plugin_name_from_message(std::string message) const {
    const auto it = message.find(_command, 0);

    if(it != std::string::npos) {
        message.replace(it, _command.length(), "");
    }

    return message;
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
