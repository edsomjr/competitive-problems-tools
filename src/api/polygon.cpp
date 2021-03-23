#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <string>
#include <vector>

#include "api/polygon.h"
#include "httplib.h"
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

  auto status_json = nlohmann::json::parse(result->body);
  auto status =
      util::get_json_value<std::string>(status_json, "status", "FAILED");
  if (status != "OK")
    assert(0 == 1); // TODO THROW
  
  return result;
}

bool test_connection(const Credentials &creds) {
  httplib::Params params{};
  auto result = get("problems.list", creds, params);

  return result->status == 200;
}

std::string get_problem_checker(const Credentials &creds,
                                const std::string &problem_id) {
  httplib::Params params;
  params.emplace("problemId", problem_id);
  auto result = get("problem.checker", creds, params);
  return result->body;
}

string generate_api_sig(const string &method_name,
                        const httplib::Params &params,
                        const Credentials &creds) {
  string params_str;
  string delimiters{"?&"};

  auto rand_int = rand() % 1000000 + 100000;
  rand_int = std::max(rand_int, 999999);
  string rand_str{std::to_string(rand_int)};

  for (const auto &[key, val] : params) {
    auto delimiter = delimiters[params_str.size() > 0];
    params_str += delimiter + key + "=" + val;
  }

  string hashed = util::sha_512(rand_str + "/" + method_name + params_str +
                                "#" + creds.secret);

  return rand_str + hashed;
}

} // namespace cptools::api::polygon