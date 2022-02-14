#include "plugin.h"

class Init : public Plugin {
public:
    Init();
};

Init::Init() : Plugin("init", "Generates template files on "
                              "current directory.", "")
{
    _description = _brief + ". If a file already exists, it is not overridden.";

    _options.emplace_back('h', "help", false, "");
    _options.emplace_back('o', "output", true, "output_dir");
}

// dynamic function that is called by the plugin manager to build the plugin
extern "C" Plugin *
create()
{
    return new Init();
}

// dynamic function that is called by the plugin manager to destroy the plugin
extern "C" void
destroy(Plugin *p)
{
    delete p;
}
