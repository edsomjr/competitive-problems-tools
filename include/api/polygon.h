#ifndef CP_TOOLS_API_POLYGON_H
#define CP_TOOLS_API_POLYGON_H

#include <string>

#include "httplib.h"

using std::string;

namespace cptools::api::polygon {

    struct Credentials {
        std::string key;
        std::string secret;
    };

    bool test_connection(const Credentials& creds);

    string sha_512(const string& s);

    string generate_api_sig(const string& method_name, const httplib::Params& params, const Credentials& creds);

}

#endif
