#ifndef CP_TOOLS_PLUGIN_MANAGER_H
#define CP_TOOLS_PLUGIN_MANAGER_H

#include <string>
#include <vector>
#include "plugin.h"


class PluginManager {
public:
    ~PluginManager();

    static PluginManager * get_instance();
    static void release();

    void load_plugin(const std::string& path);
    void find_and_load_plugins();

    Plugin* get_plugin(const std::string& plugin_name);

    std::pair<std::string, size_t>
    get_command_suggestion(const std::string& plugin_name);

private:
    struct PluginInfo {
        void *_handle;
        Plugin *_plugin;
        destroy_t *_destroy;

        PluginInfo(void *handle, Plugin *plugin, destroy_t *destroy)
            : _handle(handle), _plugin(plugin), _destroy(destroy) { }
    };

    std::vector<PluginInfo> _plugins;

    PluginManager() {}

    void release_plugin(void *handle);
};

#endif
