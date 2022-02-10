#include <iostream>
#include "pluginmanager.h"

int main()
{
    auto manager = PluginManager::get_instance();

    atexit(PluginManager::release);

    manager->load_plugin("default.so");
    manager->load_plugin("init.so");

    std::cout << "Ok\n";


    return 0;
}
