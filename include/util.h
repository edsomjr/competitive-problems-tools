#ifndef CP_TOOLS_UTIL_H
#define CP_TOOLS_UTIL_H

#include <string>
#include <vector>

#include "json.hpp"

using std::string;
using std::vector;

namespace cptools::util {
vector<string> split(const string &s, char delim = ' ');

string strip(const string &s);
string strip(const string &s, char c);

nlohmann::json read_json_file(const std::string &config_file_path);

string sha_512(const string &s);

template <typename T>
T get_json_value(const nlohmann::json &config, const std::string &fields,
                 T default_value) {
  auto fs = util::split(fields, '|');
  auto js = config;

  for (const auto &f : fs) {
    auto it = js.find(f);

    if (it == js.end())
      return default_value;
    else
      js = *it;
  }

  try {
    auto value = js.get<T>();
    return value;
  } catch (std::exception &e) {
  }

  return default_value;
}

} // namespace cptools::util

#endif
