#ifndef CP_TOOLS_PLUGIN_H
#define CP_TOOLS_PLUGIN_H

#include <string>
#include <vector>

struct Option {
    Option(
        char option,
        const std::string& name,
        bool has_arg,
        const std::string& arg_name
    ) :
        _option(option),
        _name(name),
        _has_arg(has_arg),
        _arg_name(arg_name) { }

    char _option;
    std::string _name;
    bool _has_arg;
    std::string _arg_name;
};

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
