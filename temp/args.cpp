#include <iostream>
#include <getopt.h>

#include "args.h"

const int MAX_OPTIONS { 30 };

const Option& find_option(int option, const std::vector<Option>& options)
{
    size_t i;

    for (i = 0; i < options.size(); ++i)
        if (options[i]._option == option)
            break;

    return options.at(i);
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

        short_opts.push_back(option);

        if (has_arg)
            short_opts.push_back(':');
    }

    longopts[pos] = { 0, 0, 0, 0 };

    Args args;

    int c;

    while ((c = getopt_long(argc, argv, short_opts.c_str(), longopts, NULL)) != -1)
    {
        auto i = short_opts.find(c);

        if (i == std::string::npos)
        {
            std::cerr << "Opção inválida: " << c << '\n';
            exit(-1);
        } 

        auto opt = find_option(c, options);
        args[opt._name] = opt._has_arg ? std::string(optarg) : "";
    }

    return args;
}
