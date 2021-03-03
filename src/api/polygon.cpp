#include <chrono>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>

#include "api/polygon.h"
#include "httplib.h"

namespace cptools::api::polygon {

    bool test_connection(const Credentials& creds) {
        httplib::Client client { "https://polygon.codeforces.com" };
        httplib::Params params;
        httplib::Headers headers;
        std::time_t now { std::time(nullptr) };

        params.emplace("time", std::to_string(now));
        params.emplace("apiKey", creds.key);
        params.emplace("apiSig", generate_api_sig("problems.list", params, creds));

        auto result = client.Get("/api/problems.list", params, headers);

        return result->status == 200;
    }

    string sha_512(const string& s) {
        unsigned char hash[SHA512_DIGEST_LENGTH];
        char output[129];
        SHA512_CTX sha512;
        SHA512_Init(&sha512);
        SHA512_Update(&sha512, s.c_str(), s.size());
        SHA512_Final(hash, &sha512);
        for(int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
            sprintf(output + (i * 2), "%02x", hash[i]);
        }
        output[128] = 0;
        return string(output);
    }

    string generate_api_sig(const string& method_name, const httplib::Params& params, const Credentials& creds) {
        string params_str;
        string delimiters { "?&" };

        auto rand_int = rand() % 1000000 + 100000;
        rand_int = std::max(rand_int, 999999);
        string rand_str { std::to_string(rand_int) };

        for(const auto& [key, val] : params) {
            auto delimiter = delimiters[params_str.size() > 0];
            params_str += delimiter + key + "=" + val;
        }

        string hashed = sha_512(rand_str + "/" + method_name + params_str + "#" + creds.secret);

        return rand_str + hashed;
    }

}