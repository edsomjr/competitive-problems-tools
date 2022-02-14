#include <iostream>
#include "pluginmanager.h"

const std::string get_subcommand(int argc, char *argv[])
{
    if (argc < 2)
        return "default";

    if(argv[1][0] == '-')
        return "default";

    return argv[1];
}

int main(int argc, char *argv[])
{
    auto manager = PluginManager::get_instance();
    manager->find_and_load_plugins();

    atexit(PluginManager::release);

    auto subcommand = get_subcommand(argc, argv);
    std::cout << "subcommand: " << subcommand << '\n';

    auto plugin = manager->get_plugin(subcommand);

    std::cout << "plugin->command(): " << plugin->command() << '\n';

    std::cout << "Ok\n";

    return 0;
}
