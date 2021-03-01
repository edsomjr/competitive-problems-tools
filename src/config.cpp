#include "config.h"
#include "util.h"
#include "fs.h"
#include "exceptions.h"

#include <fstream>

namespace cptools::config {

    json read(const std::string& config_file_path)
    {
        if(fs::file_exists(config_file_path)) {
            std::ifstream config_file(config_file_path);
            json config;

            config_file >> config;

            return config;
        }

        throw(exceptions::inexistent_file(config_file_path));
    }

    template<typename T>
    T get(const json& config, const std::string& fields, T default_value) {
        auto fs = util::split(fields, '|');
        auto js = config;

        for (const auto& f : fs)
        {
            auto it = js.find(f);

            if (it == js.end())
                return default_value;
            else
                js = *it;
        }

        try {
            auto value = js.get<T>();
            return value;
        } catch (std::exception& e)
        {
        }

        return default_value;
    }
}
