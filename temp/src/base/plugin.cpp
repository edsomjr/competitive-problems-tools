#include <sstream>
#include <iostream>

#include "plugin.h"

std::string
Plugin::usage() const
{
    std::ostringstream oss;

    oss << "Usage: cp-tools";

    if (command() != "default")
        oss << ' ' << command();

    for (auto [option, name, has_arg, arg_name, _] : _options)
    {
        if (option > 0)
            oss << " [-" << (char) option;
        else
            oss << " [--" << name;

        if (has_arg)
            oss << " " << arg_name;

        oss << "]";
    }

    return oss.str();
}

std::string
Plugin::help() const
{
    std::ostringstream oss;

    oss << usage() << '\n';
    oss << description() << "\n\n";
    oss << "Options\t\t\tDescription";

    for (auto [option, name, _, __, description] : _options)
    {
        oss << "\n\n";

        if (option > 0)
        {
            oss << "  -" << (char) option << "\t\t\t" << description << '\n';
            oss << "  --" << name;
        } else
            oss << "  --" << name << "\t\t" << description << '\n';
    }

    oss << '\n';

    return oss.str();
}
