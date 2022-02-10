#ifndef CP_TOOLS_PLUGIN_H
#define CP_TOOLS_PLUGIN_H

#include <string>
#include <vector>

struct Option {
    Option(char option, const std::string& name, bool has_arg, const std::string& arg_name) :
        _option(option), _name(name), _has_arg(has_arg), _arg_name(arg_name) { }

    char _option;
    std::string _name;
    bool _has_arg;
    std::string _arg_name;
};

class Plugin {
public:
    virtual ~Plugin() { }

    virtual std::string command() const = 0;

    virtual std::string brief() const = 0;
    virtual std::string description() const = 0;

    std::vector<Option> options() { return _options; };
    
protected:
    std::vector<Option> _options;
};

typedef Plugin* create_t();
typedef void destroy_t(Plugin *);

#endif
