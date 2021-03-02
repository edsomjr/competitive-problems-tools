#ifndef CP_TOOLS_API_POLYGON_H
#define CP_TOOLS_API_POLYGON_H

#include <string>

namespace cptools::api::polygon {

    struct Credentials {
        std::string key;
        std::string secret;
    };

    bool test_connection(const Credentials& creds);

}

#endif
