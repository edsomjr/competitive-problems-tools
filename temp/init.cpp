#include "plugin.h"

class Init : public Plugin {
public:
    Init();

    std::string command() const { return _command; }

    std::string brief() const { return _brief; }
    std::string description() const { return _description; }

private:
    std::string _command, _brief, _description;
};

Init::Init() : _command("init"), _brief("Generates template files on current directory."), _description(_brief)
{
    _description += " ";
    _description += "If a file already exists, it is not overridden.";


    _options.emplace_back('h', "help", false, "");
    _options.emplace_back('o', "output", true, "output_dir");
}

extern "C" Plugin *
create()
{
    return new Init();
}

extern "C" void
destroy(Plugin *p)
{
    delete p;
}
