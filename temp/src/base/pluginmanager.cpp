#include <iostream>
#include <dlfcn.h>
#include <filesystem>
#include <limits>
#include <sstream>


#include "dirs.h"
#include "utils.h"
#include "pluginmanager.h"


static PluginManager *instance = nullptr;

PluginManager *
PluginManager::get_instance()
{
    if (!instance) {
        // std::cout << "[THERE SHOULD BE ONLY ONE] - Creating PluginManager instance\n";
        instance = new PluginManager();
    }

    // std::cout << "Returning PluginManager instance\n";

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
    // std::cout << "[THERE SHOULD BE ONLY ONE] - Destrutor\n";

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

    // std::cout << "      - '" << plugin->command() << "' loaded successfully.\n";
    _plugins.emplace_back(handle, plugin, destroy);
}

void PluginManager::find_and_load_plugins() {
    // std::cout << "Founded plugins:\n";

    for(const auto& entry : std::filesystem::directory_iterator(CP_TOOLS_PLUGIN_DIR))
    {
        if (entry.path().extension() == ".so") {
            // std::cout << "  - " << entry.path().filename() << '\n';
            load_plugin(entry.path());
        }
    }
}


Plugin* PluginManager::get_plugin(const std::string& plugin_name) {
    for(const auto& [_, plugin, __] : _plugins) {
        if(plugin->command() == plugin_name)
            return plugin;
    }

    std::cerr << "cp-tools: '" << plugin_name
              << "' is not a cp-tools command. See 'cp-tools --help'.\n";

    auto [command_suggestion, proximity] = get_command_suggestion(plugin_name);

    // TODO: Pegar o valor máximo de proximity do arquivo de configuração
    if(proximity < 3) {
        std::cerr << "\nThe most similar command is \n\t" << command_suggestion << "\n";
    }

    exit(-1);
}

std::pair<std::string, size_t>
PluginManager::get_command_suggestion(const std::string& plugin_name) {
    std::string command_suggestion;
    size_t proximity = std::numeric_limits<std::size_t>::max();

    for(const auto& [_, plugin, __] : _plugins) {
        auto command_name = plugin->command();

        // default command is not suggested
        if(command_name == "default")
            continue;

        auto distance = levenshtein_distance(command_name, plugin_name);

        if(distance < proximity) {
            command_suggestion = command_name;
            proximity = distance;
        }
    }

    return std::make_pair(command_suggestion, proximity);
}


std::string
PluginManager::get_plugins_briefs() const {
    std::ostringstream oss;

    size_t start_of_column_brief = 24;

    bool first = true;

    for(const auto & [_, plugin, __] : _plugins) {
        // Trick to avoid printing the default command. TODO: Think of a smarter way
        if(!first) oss << "\n\n";
        else first = false;

        oss << "  " << plugin->command();

        size_t remaining = start_of_column_brief - plugin->command().size() - 2;
        while(remaining--) oss << ' ';

         oss << plugin->brief();
    }

    return oss.str();
}