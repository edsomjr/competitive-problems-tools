#ifndef CP_TOOLS_ARGS_H
#define CP_TOOLS_ARGS_H

#include <map>
#include <string>
#include <vector>

struct Option {
    Option(
        int option,
        const std::string& name,
        bool has_arg,
        const std::string& arg_name,
        const std::string& description
    ) :
        _option(option),
        _name(name),
        _has_arg(has_arg),
        _arg_name(arg_name),
        _description(description) { }

    int _option;
    std::string _name;
    bool _has_arg;
    std::string _arg_name;
    std::string _description;
};

using Args = std::map<std::string, std::string>;

Args
parse_args(int argc, char *argv[], const std::vector<Option>& options);

#endif
