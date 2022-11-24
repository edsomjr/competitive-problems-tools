#ifndef CP_TOOLS_PLUGIN_H
#define CP_TOOLS_PLUGIN_H

#include <string>
#include <vector>

#include "args.h"

class Plugin {
public:
    virtual ~Plugin() { }

    Plugin(
        const std::string& name,
        const std::string& brief,
        const std::string& description
    ) : _command(name),
        _brief(brief),
        _description(description) {
            if(description.empty())
                _description = brief;
        }

    virtual int execute(const Args& args) = 0;
    virtual std::string help() const;

    std::string usage() const;
    std::string command() const { return _command; }
    std::string brief() const { return _brief; }
    std::string description() const { return _description; }

    std::vector<Option> options() { return _options; };

protected:
    std::vector<Option> _options;
    std::string _command, _brief, _description;
};

typedef Plugin* create_t();
typedef void destroy_t(Plugin *);

#endif
