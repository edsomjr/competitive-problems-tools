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

httplib::Result get(std::string method, const types::polygon::Credentials &creds,
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

bool test_connection(const types::polygon::Credentials &creds) {
    httplib::Params params{};

    try {
        get("problems.list", creds, params);
    } catch (const exceptions::polygon_api_error &e) {
        return false;
    }

    return true;
}

std::string get_problem_file_name(const std::string &tool_type,
                                  const types::polygon::Credentials &creds,
                                  const std::string &problem_id) {
    const std::vector<std::string> valid_tools = {"checker", "validator"};
    auto found = std::find(valid_tools.begin(), valid_tools.end(), tool_type);
    if (found == valid_tools.end())
        throw(
            exceptions::polygon_api_error("Trying to get file for invalid resource: " + tool_type));

    httplib::Params params;
    params.emplace("problemId", problem_id);
    auto result = get("problem." + tool_type, creds, params);

    auto request_json = nlohmann::json::parse(result->body);
    auto file_name = util::get_json_value<std::string>(request_json, "result", "");

    if (file_name == "" || file_name == "std::none") {
        throw(exceptions::polygon_api_error("Expected a valid file name for " + tool_type +
                                            " but got \"" + file_name + "\""));
    }

    return file_name;
}

std::string get_problem_file(const std::string &file_name, const std::string &tool_type,
                             const types::polygon::Credentials &creds,
                             const std::string &problem_id) {
    httplib::Params params;
    params.emplace("problemId", problem_id);
    params.emplace("type", "source");
    params.emplace("name", file_name);

    std::string method = "problem.view";
    if (tool_type == "solution")
        method.append("Solution");
    else
        method.append("File");

    auto result = get(method, creds, params);

    return result->body;
}

std::vector<types::polygon::Solution>
get_problem_solutions(const types::polygon::Credentials &creds, const std::string problem_id) {
    httplib::Params params;
    params.emplace("problemId", problem_id);
    auto result = get("problem.solutions", creds, params);

    auto solutions_json = nlohmann::json::parse(result->body).at("result");
    auto solutions = solutions_json.get<types::polygon::SolutionsVector>();

    return solutions;
}

std::string generate_api_sig(const std::string &method_name, const httplib::Params &params,
                             const types::polygon::Credentials &creds) {
    std::string params_str;
    std::string delimiters{"?&"};

    auto rand_int = rand() % 1000000;

    std::string rand_str = std::to_string(rand_int);
    std::string zero_padding;
    zero_padding.append(6 - rand_str.size(), '0');
    zero_padding += rand_str;

    for (const auto &[key, val] : params) {
        auto delimiter = delimiters[params_str.size() > 0];
        auto val_encoded = httplib::detail::encode_query_param(val);
        params_str += delimiter + key + "=" + val_encoded;
    }

    auto to_hash = zero_padding + "/" + method_name + params_str + "#" + creds.secret;
    std::string hashed = util::sha_512(to_hash);

    return zero_padding + hashed;
}

} // namespace cptools::api::polygon