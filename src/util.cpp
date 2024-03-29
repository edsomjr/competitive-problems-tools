#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <openssl/ssl.h>
#include <sstream>

#include "exceptions.h"
#include "fs.h"
#include "json.hpp"
#include "util.h"

namespace cptools::util {

/**
 * @brief Get the timestamp of the current time using the format "%Y-%m-%d_%H:%M:%S".
 *
 * @return std::string with the timestamp formatted as described above.
 */
std::string get_timestamp() {
    auto t = std::time(nullptr);
    auto tm = std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(tm, "%Y-%m-%d_%H:%M:%S");
    return ss.str();
}

std::vector<std::string> split(const std::string &s, char delim) {
    enum State { NONE, ESCAPED, TOKEN };
    enum State state = NONE;

    std::string temp = s + delim;
    int start = -1, count = 0;
    std::vector<std::string> tokens;

    for (size_t i = 0; i < temp.size(); ++i) {
        auto c = temp[i];

        switch (state) {
        case NONE:
            if (c == '\"') {
                state = ESCAPED;
                start = i + 1;
                count = 0;
            } else if (c != delim) {
                state = TOKEN;
                start = i;
                count = 1;
            }

            break;

        case ESCAPED:
            if (c == '\"')
                state = TOKEN;
            else
                ++count;

            break;

        case TOKEN:
            if (c == delim) {
                if (count)
                    tokens.push_back(temp.substr(start, count));
                else
                    tokens.push_back("");

                count = 0;
                state = NONE;
            } else
                ++count;

            break;
        }
    }

    return tokens;
}

std::string sha_512(const std::string &s) {
    unsigned char hash[SHA512_DIGEST_LENGTH];
    std::ostringstream output;
    SHA512_CTX sha512;

    SHA512_Init(&sha512);
    SHA512_Update(&sha512, s.c_str(), s.size());
    SHA512_Final(hash, &sha512);

    output << std::hex << std::setfill('0');
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
        output << std::setw(2) << (int)hash[i];
    }

    return output.str();
}

static std::string strip(const std::string &s, const std::string &delim) {
    auto i = s.find_first_not_of(delim);
    auto j = s.find_last_not_of(delim);

    return (i == std::string::npos or j < i) ? "" : s.substr(i, j - i + 1);
}

std::string strip(const std::string &s) { return strip(s, " \t\n\r\b"); }

std::string strip(const std::string &s, char c) { return strip(s, std::string(1, c)); }

std::string join(const std::vector<std::string> &v, const char delim) {
    std::stringstream ss;
    const auto delim_str = std::string(1, delim);
    for (size_t i = 0; i < v.size(); ++i) {
        const auto local_delim = i ? delim_str : "";
        ss << local_delim << v[i];
    }
    return ss.str();
}

std::string lower_string(const std::string &s) {
    auto s_copy = std::string(s);
    std::transform(s.begin(), s.end(), s_copy.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s_copy;
}

nlohmann::json read_json_file(const std::string &config_file_path) {
    auto result = fs::exists(config_file_path);
    if (not result.ok)
        throw(exceptions::inexistent_file_error(config_file_path));

    std::ifstream config_file(config_file_path);
    nlohmann::json config;

    config_file >> config;

    return config;
}

std::string to_json_pointer(const std::string &s) {
    if (not s.length())
        return s;

    std::string pointer;
    if (s[0] != '/')
        pointer += "/";

    pointer.append(s);
    std::replace(pointer.begin(), pointer.end(), '|', '/');

    return pointer;
}

/**
 * @brief Gets the value associated with the key in argv
 *
 * @param key Associated key
 * @param argc
 * @param argv
 * @return std::string. If the key exists in argv, the associated value is returned,
 *                      otherwise an empty string is returned.
 */
std::string get_argv_value(std::string key, int argc, char *const argv[]) {
    for (int i = 0; i < argc; i++) {

        std::string str(argv[i]);

        if (str.rfind(key, 0) == 0) {

            // if start with `--` it's need to get what comes after the` = `
            if (str.rfind("--", 0) == 0) {
                size_t pos = str.find("=") + 1;
                return std::string(str.begin() + pos, str.end());
            }

            // if it starts with `-` the value is in the next position
            else {
                return std::string(argv[i + 1]);
            }
        }
    }

    return std::string();
}

/**
 * @brief Get the first value associated with one of the keys
 *
 * @param keys Keys that will be searched from argv
 * @param argc
 * @param argv
 * @param default_value Value returned if none of the keys are found
 *
 * @return std::string.
 */
std::string get_from_argv(int argc, char *const argv[], std::vector<std::string> keys,
                          std::string default_value) {
    for (const auto &key : keys) {
        std::string value = get_argv_value(key, argc, argv);

        if (value.empty() == false) {
            return value;
        }
    }

    return default_value;
}

std::pair<int, int> reduce_fraction(int num, int dem) {
    int d = std::gcd(num, dem);

    num /= d;
    dem /= d;

    return {num, dem};
}

} // namespace cptools::util
