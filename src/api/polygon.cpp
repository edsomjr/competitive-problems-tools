#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <string>
#include <vector>

#include "api/polygon.h"
#include "exceptions.h"
#include "httplib.h"
#include "json.hpp"
#include "util.h"

namespace cptools::api::polygon {

httplib::Result get(std::string method, const Credentials &creds,
                    httplib::Params &params) {
  std::time_t now{std::time(nullptr)};
  std::string path{"/api/" + method};

  params.emplace("time", std::to_string(now));
  params.emplace("apiKey", creds.key);
  params.emplace("apiSig", generate_api_sig(method, params, creds));

  httplib::Client client{"https://polygon.codeforces.com"};
  auto result = client.Get(path.c_str(), params, httplib::Headers{});

  if (result->status != 200) {
    throw(exceptions::polygon_api_error(result));
  }

  return result;
}

bool test_connection(const Credentials &creds) {
  httplib::Params params{};
  auto result = get("problems.list", creds, params);

  return result->status == 200;
}

std::string get_problem_file(const std::string &tool_name,
                             const Credentials &creds,
                             const std::string &problem_id) {
  const std::vector<std::string> valid_tools = {"checker", "validator"};
  auto found = std::find(valid_tools.begin(), valid_tools.end(), tool_name);
  if (found == valid_tools.end())
    throw(exceptions::polygon_api_error(
        "Trying to get file for invalid resource: " + tool_name));

  httplib::Params params;
  params.emplace("problemId", problem_id);
  auto result = get("problem." + tool_name, creds, params);

  auto request_json = nlohmann::json::parse(result->body);
  auto file_name =
      util::get_json_value<std::string>(request_json, "result", "");

  if (file_name == "" || file_name == "std::none") {
    throw(exceptions::polygon_api_error("Expected a valid file name for " +
                                        tool_name + " but got \"" + file_name +
                                        "\""));
  }

  params.clear();
  params.emplace("problemId", problem_id);
  params.emplace("type", "source");
  params.emplace("name", file_name);
  result = get("problem.viewFile", creds, params);
  return result->body;
}

string generate_api_sig(const string &method_name,
                        const httplib::Params &params,
                        const Credentials &creds) {
  string params_str;
  string delimiters{"?&"};

  auto rand_int = rand() % 1000000;

  string rand_str = std::to_string(rand_int);
  string zero_padding;
  zero_padding.append(6 - rand_str.size(), '0');
  zero_padding += rand_str;

  for (const auto &[key, val] : params) {
    auto delimiter = delimiters[params_str.size() > 0];
    auto val_encoded = httplib::detail::encode_query_param(val);
    params_str += delimiter + key + "=" + val_encoded;
  }

  auto to_hash =
      zero_padding + "/" + method_name + params_str + "#" + creds.secret;
  string hashed = util::sha_512(to_hash);

  return zero_padding + hashed;
}

} // namespace cptools::api::polygon