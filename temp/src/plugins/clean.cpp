#include <iostream>

#include "fs.h"
#include "dirs.h"
#include "plugin.h"
#include "message.h"

class Clean : public Plugin {
public:
    Clean();

    int execute(const Args& args);
};

Clean::Clean() : Plugin("clean", "Remove autogenerated files.", "")
{
    _description = _brief + " These files are located in '.cp-build' and '.cp-tmp' directories of the working folder.";

    _options.emplace_back('h', "help", false, "", "Generates this help message.");
    _options.emplace_back('w', "working_dir", true, "working_dir", "Directory to be cleaned.");
}

static int
remove_autogenerated_files(const std::string &target)
{
    std::cout << cptools::message::info("Cleaning autogenerated files...\n");

    // Finds directories that store the generated files
    std::string build_dir{ target + "/" + CP_TOOLS_BUILD_DIR };
    std::string temp_dir{ target + "/" + CP_TOOLS_TEMP_DIR };

    if (not (cptools::fs::is_directory(build_dir) or cptools::fs::is_directory(temp_dir)))
    {
        std::cout << cptools::message::warning("No autogenerated files found!\n");
        return CP_TOOLS_OK;
    }

    // Deletes the directories
    for (auto dir : { build_dir, temp_dir })
        cptools::fs::remove(dir);

    std::cout << cptools::message::ok() << '\n';

    return CP_TOOLS_OK;
}

int
Clean::execute(const Args& args)
{
    std::string dest = args.count("working_dir") ? args.at("working_dir") : std::string(".");

    return remove_autogenerated_files(dest);
}

// dynamic function that is called by the plugin manager to build the plugin
extern "C" Plugin *
create()
{
    return new Clean();
}

// dynamic function that is called by the plugin manager to destroy the plugin
extern "C" void
destroy(Plugin *p)
{
    delete p;
}