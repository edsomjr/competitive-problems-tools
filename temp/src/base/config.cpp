#include <iostream>
#include <fstream>

#include "config.h"
#include "fs.h"
#include "dirs.h"
#include "defs.h"
#include "message.h"



namespace cptools::config
{
    nlohmann::json
    read_config_file()
    {
        if(not fs::exists(CONFIG_FILE_NAME)) {
            auto msg = "Config file `" CONFIG_FILE_NAME "` not found\n";
            std::cout << message::error(msg);
            exit(-1);
        }

        std::ifstream config_file(CONFIG_FILE_NAME);
        nlohmann::json config;
        config_file >> config;

        return config;
    }

}