#include <iostream>
#include <optional>
#include <getopt.h>

#include "args.h"

const int MAX_OPTIONS { 30 };

std::optional<Option> find_option(int option, const std::vector<Option>& options)
{
    for (size_t i = 0; i < options.size(); ++i)
        if (options[i]._option == option)
            return options.at(i); 

    return { };
}

Args
parse_args(int argc, char *argv[], const std::vector<Option>& options)
{
    struct option longopts[MAX_OPTIONS];
    std::string short_opts;
    size_t pos;

    for (pos = 0; pos < options.size(); ++pos)
    {
        auto [option, _, has_arg, arg_name, __] = options[pos];

        longopts[pos] = { options[pos]._name.c_str(), has_arg ? required_argument : no_argument, NULL, option };

        if (option)
        {
            short_opts.push_back(option);

            if (has_arg)
                short_opts.push_back(':');
        }
    }

    longopts[pos] = { 0, 0, 0, 0 };

    Args args;

    int c;

    while ((c = getopt_long(argc, argv, short_opts.c_str(), longopts, NULL)) != -1)
    {
        auto opt = find_option(c, options);

        if (opt)
            args[opt.value()._name] = opt.value()._has_arg ? std::string(optarg) : "";
        else
        {
            std::cerr << "Opção inválida: " << c << '\n';
            exit(-1);
        }
    }

    return args;
}
