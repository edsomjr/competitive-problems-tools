#include <iostream>

#include "config.h"
#include "dirs.h"
#include "fs.h"
#include "message.h"
#include "plugin.h"
#include "utils.h"


class Check : public Plugin {
public:
    Check();

    int execute(const Args& args);

    int validate_checker();
};

Check::Check() : Plugin("check", "Verify problem files and tools.", "")
{
    _options.emplace_back(
        'a', "all", false, "", "Validates everything (default option)."
    );
    _options.emplace_back('c', "checker", false, "", "Validates the checker.");
    _options.emplace_back('h', "help", false, "", "Generates this help message.");
    _options.emplace_back('s', "solutions", false, "", "Validates the solutions.");
    _options.emplace_back('t', "tests", false, "", "Validates the tests.");
    _options.emplace_back('v', "validator", false, "", "Validates the validator.");
}

int Check::validate_checker()
{
    std::string msg("Creating temporary directory `" CP_TOOLS_BUILD_DIR "`\n");
    std::cout << cptools::message::info(msg);

    cptools::fs::create_directory(CP_TOOLS_BUILD_DIR);

    auto validator{ std::string(CP_TOOLS_BUILD_DIR) + "/validator " };

    auto config = cptools::config::read_config_file();

    auto source = cptools::utils::get_json_value(
                                        config, "tools|validator", std::string("ERROR"));

    if(source == "ERROR") {
        std::cout << cptools::message::error("Validator file not found!\n");
        exit(-1);
    }

    return 0;
}

int Check::execute(const Args& args)
{
    if(args.count("checker"))
    {
        validate_checker();
    }

    if (args.count("solutions"))
    {
        std::cout << "Validating the solutions.\n";
    }

    if (args.count("tests"))
    {
        std::cout << "Validating the tests.\n";
    }

    if (args.count("validator"))
    {
        std::cout << "Validating the validator.\n";
    }

    if(args.count("all"))
    {
        std::cout << "Validating everything.\n";
    }


    return 0;
}

// dynamic function that is called by the plugin manager to build the plugin
extern "C" Plugin *
create()
{
    return new Check();
}

// dynamic function that is called by the plugin manager to destroy the plugin
extern "C" void
destroy(Plugin *p)
{
    delete p;
}
