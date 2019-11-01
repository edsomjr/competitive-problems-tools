#include "config.h"
#include "util.h"

#include <fstream>

namespace cptools::config {

    // TODO: criar um objeto com os valores default caso o arquivo de 
    // configurações não exista
    json read(const std::string& config_file_path)
    {
        std::ifstream config_file(config_file_path);
        json config;

        config_file >> config;

        return config;
    }

}
