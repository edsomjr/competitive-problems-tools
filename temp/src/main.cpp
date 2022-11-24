#include <iostream>

#include "args.h"
#include "pluginmanager.h"

const std::string get_subcommand(int argc, char *argv[])
{
    if (argc < 2 || argv[1][0] == '-')
        return "default";

    return argv[1];
}

int main(int argc, char *argv[])
{
    auto& manager = PluginManager::get_instance();
    manager.find_and_load_plugins();

    auto subcommand = get_subcommand(argc, argv);
    auto plugin = manager.get_plugin(subcommand);

    auto args = parse_args(argc, argv, plugin->options());

    if(args.count("help"))
        std::cout << plugin->help() << '\n';
    else
        plugin->execute(args);
}
