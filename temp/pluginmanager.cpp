#include <iostream>
#include <dlfcn.h>
#include <filesystem>

#include "dirs.h"
#include "utils.h"
#include "pluginmanager.h"

static PluginManager *instance = nullptr;

PluginManager *
PluginManager::get_instance()
{
    if (!instance)
        instance = new PluginManager();

    return instance;
}

void
PluginManager::release()
{
    if (instance)
        delete instance;

    instance = nullptr;
}

PluginManager::~PluginManager()
{
    // std::cout << "Destrutor\n";

    for (auto [handle, plugin, destroy] : _plugins)
    {
        if (plugin)
        {
            // std::cout << "Liberando o plugin: " << plugin->command() << ": " << plugin->description() << '\n';
            destroy(plugin);
        }

        if (handle)
		    dlclose(handle);
    }
}

void
PluginManager::load_plugin(const std::string& path)
{
    // std::cout << "Carregando o plugin: " << path << "\n";

	auto handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);

	if (!handle)
	{
		std::cerr << "Load plugin error: " << dlerror() << '\n';
		exit(-1);
	}

    // getting the plugin constructor
	auto create = (create_t *) dlsym(handle, "create");

	if (!create)
	{
		std::cerr << "Load plugin error: " << dlerror() << '\n';
		dlclose(handle);
		exit(-1);
	}

    // getting the plugin create function
	auto destroy = (destroy_t *) dlsym(handle, "destroy");

	if (!destroy)
	{
		std::cerr << "Load plugin error: " << dlerror() << '\n';
		dlclose(handle);
		exit(-1);
	}

    auto plugin = create();

    if (!plugin)
    {
        std::cerr << "Can't create plugin: " << path << '\n';
        exit(-1);
    }

    _plugins.emplace_back(handle, plugin, destroy);
}

void PluginManager::find_and_load_plugins() {
    for(const auto& entry : std::filesystem::directory_iterator(CP_TOOLS_PLUGIN_DIR))
    {
        if (entry.path().extension() == ".so") {
            load_plugin(entry.path());
        }
    }

}


Plugin* PluginManager::get_plugin(const std::string& plugin_name) {
    for(const auto& [handle, plugin, destroy] : _plugins) {
        if(plugin->command() == plugin_name)
            return plugin;
    }

    std::cerr << "cp-tools: '" << plugin_name << "' is not a cp-tools command. See 'cp-tools --help'.\n";

    auto [command_suggestion, proximity] = get_command_suggestion(plugin_name);

    if(proximity < 3) {
        std::cerr << "\nThe most similar command is \n\t"
                  << command_suggestion << "\n";
    }

    exit(-1);
}

std::pair<std::string, size_t>
PluginManager::get_command_suggestion(const std::string& plugin_name) {
    std::string command_suggestion;
    size_t proximity = 0;

    for(const auto& [handle, plugin, destroy] : _plugins) {
        auto command_name = plugin->command();

        auto distance = levenshtein_distance(command_name, plugin_name);

        if(distance < proximity || proximity == 0) {
            command_suggestion = command_name;
            proximity = distance;
        }
    }

    return std::make_pair(command_suggestion, proximity);
}