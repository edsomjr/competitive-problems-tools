#include <sstream>
#include <iostream>

#include "plugin.h"

std::string
Plugin::usage() const
{
    std::ostringstream oss;

    oss << "Usage: cp-tools " << command();

    for (auto [option, _, has_arg, arg_name, __] : _options)
    {
        oss << " [-" << option;

        if (has_arg)
            oss << " " << arg_name;

        oss << "]";
    }

    return oss.str();
}

std::string
Plugin::help()
{
    std::ostringstream oss;

    oss << usage() << '\n';
    oss << description() << "\n\n";
    oss << "Options\t\t\tDescription";

    for (auto [option, name, _, __, description] : _options)
    {
        oss << "\n\n";
        oss << "  -" << option << "\t\t\t" << description << '\n';
        oss << "  --" << name;
    }

    oss << '\n';

    return oss.str();
}
