#include "plugin.h"

class Default : public Plugin {
public:
    Default();


    std::string command() const { return _command; }

    std::string brief() const { return _brief; }
    std::string description() const { return _description; }

private:
    std::string _command, _brief, _description;
};

Default::Default() : _command("default"), _brief(""), _description("")
{
    _options.emplace_back('h', "help", false, "");
    _options.emplace_back('v', "version", false, "");
}

extern "C" Plugin *
create()
{
    return new Default();
}

extern "C" void
destroy(Plugin *p)
{
    delete p;
}
