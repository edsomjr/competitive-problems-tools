#include <iostream>
#include <dlfcn.h>
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
    std::cout << "Destrutor\n";

    for (auto [handle, plugin, destroy] : _plugins)
    {
        if (plugin)
        {
            std::cout << "Liberando o plugin: " << plugin->command() << ": " << plugin->description() << '\n';
            destroy(plugin);
        }

        if (handle)
		    dlclose(handle);
    }	
}

void
PluginManager::load_plugin(const std::string& path)
{
    std::cout << "Carregando o plugin: " << path << "\n";

	auto handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
	
	if (!handle)
	{
		std::cerr << "Load plugin error: " << dlerror() << '\n';
		return;
	}

	auto create = (create_t *) dlsym(handle, "create");
	
	if (!create)
	{
		std::cerr << "Load plugin error: " << dlerror() << '\n';
		dlclose(handle);
		return;
	} 

	auto destroy = (destroy_t *) dlsym(handle, "destroy");
	
	if (!destroy)
	{
		std::cerr << "Load plugin error: " << dlerror() << '\n';
		dlclose(handle);
		return;
	}
	
    auto plugin = create();

    if (!plugin)
    {
        std::cerr << "Can't create plugin: " << path << '\n';
        return;
    }
 
    _plugins.emplace_back(handle, plugin, destroy);
}
