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

namespace cptools::api::polygon
{

    /**
     * @brief Makes a GET HTTP request to Polygon's API.
     *
     * @param method Polygon's API method to call
     * @param creds Credentials to use for the request
     * @param params Parameters to send with the request
     * @return httplib::Result
     */
    httplib::Result
    get(std::string method, const types::polygon::Credentials &creds, httplib::Params &params)
    {
        std::time_t now{ std::time(nullptr) };
        std::string path{ "/api/" + method };

        params.emplace("time", std::to_string(now));
        params.emplace("apiKey", creds.key);
        params.emplace("apiSig", generate_api_sig(method, params, creds));

        httplib::Client client{ "https://polygon.codeforces.com" };
        auto result = client.Get(path.c_str(), params, httplib::Headers{});

        if (result->status != 200)
            {
                throw(exceptions::polygon_api_error(result));
            }

        return result;
    }

    /**
     * @brief Tests whether a Polygon connection is valid.
     *
     * @param creds Credentials to use for the request
     * @return true if the connection is valid, false otherwise
     */
    bool
    test_connection(const types::polygon::Credentials &creds)
    {
        httplib::Params params{};

        try
            {
                get("problems.list", creds, params);
            }
        catch (const exceptions::polygon_api_error &e)
            {
                return false;
            }

        return true;
    }

    /**
     * @brief Get the problem file name object
     *
     * @param tool_type Which tools should be pulled the information
     * @param creds Credentials to use for the request
     * @param problem_id The problem ID
     * @return std::string with the problem file name
     */
    std::string
    get_problem_file_name(const std::string &tool_type, const types::polygon::Credentials &creds,
                          const std::string &problem_id)
    {
        httplib::Params params;
        params.emplace("problemId", problem_id);
        auto result = get("problem." + tool_type, creds, params);

        auto request_json = nlohmann::json::parse(result->body);
        auto file_name = util::get_json_value<std::string>(request_json, "result", "");

        if (file_name == "" || file_name == "std::none")
            {
                throw(exceptions::polygon_api_error("Expected a valid file name for " + tool_type
                                                    + " but got \"" + file_name + "\""));
            }

        return file_name;
    }

    /**
     * @brief Get the problem file content
     *
     * @param file_name The problem file name
     * @param tool_type Which tool should be pulled the information
     * @param creds Credentials to use for the request
     * @param problem_id The problem ID
     * @return std::string with the problem file content
     */
    std::string
    get_problem_file(const std::string &file_name, const std::string &tool_type,
                     const types::polygon::Credentials &creds, const std::string &problem_id)
    {
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

    /**
     * @brief Get the problem solutions objects
     *
     * @param creds Credentials to use for the request
     * @param problem_id The problem ID
     * @return std::vector<types::polygon::Solution> with all Solutions found
     */
    std::vector<types::polygon::Solution>
    get_problem_solutions(const types::polygon::Credentials &creds, const std::string problem_id)
    {
        httplib::Params params;
        params.emplace("problemId", problem_id);
        auto result = get("problem.solutions", creds, params);

        auto solutions_json = nlohmann::json::parse(result->body).at("result");
        auto solutions = solutions_json.get<types::polygon::SolutionsVector>();

        return solutions;
    }

    /**
     * @brief Get the problem statement object
     *
     * @param creds Credentials to use for the request
     * @param problem_id The problem ID
     * @return types::polygon::StatementsVector with all Statements found
     */
    types::polygon::StatementsVector
    get_problem_statement(const types::polygon::Credentials &creds, const std::string &problem_id)
    {
        httplib::Params params;
        params.emplace("problemId", problem_id);
        auto result = get("problem.statements", creds, params);

        auto statements_json = nlohmann::json::parse(result->body).at("result");
        types::polygon::StatementsVector statements;
        for (auto &key_value : statements_json.items())
            {
                key_value.value()["language"] = key_value.key();
                statements.push_back(key_value.value().get<types::polygon::Statement>());
            }

        return statements;
    }

    /**
     * @brief Get the problem information object
     *
     * @param creds Credentials to use for the request
     * @param problem_id The problem ID
     * @return types::polygon::Information with all Information found
     */
    types::polygon::Information
    get_problem_information(const types::polygon::Credentials &creds, const std::string &problem_id)
    {
        httplib::Params params;
        params.emplace("problemId", problem_id);
        auto result = get("problem.info", creds, params);

        auto info_json = nlohmann::json::parse(result->body).at("result");
        auto info = info_json.get<types::polygon::Information>();

        return info;
    }

    /**
     * @brief Generates the parameters for authorization in a Polygon request
     *
     * @param method_name The name of the method to be called
     * @param params The parameters to be passed to the method
     * @param creds Credentials to use for the request
     * @return std::string with the API signature
     */
    std::string
    generate_api_sig(const std::string &method_name, const httplib::Params &params,
                     const types::polygon::Credentials &creds)
    {
        std::string params_str;
        std::string delimiters{ "?&" };

        auto rand_int = rand() % 1000000;

        std::string rand_str = std::to_string(rand_int);
        std::string zero_padding;
        zero_padding.append(6 - rand_str.size(), '0');
        zero_padding += rand_str;

        for (const auto &[key, val] : params)
            {
                auto delimiter = delimiters[params_str.size() > 0];
                params_str += delimiter + key + "=" + val;
            }

        auto to_hash = zero_padding + "/" + method_name + params_str + "#" + creds.secret;
        std::string hashed = util::sha_512(to_hash);

        return zero_padding + hashed;
    }

    /**
     * @brief Get the problem tags
     *
     * @param creds Credentials to use for the request
     * @param problem_id The problem ID
     * @return std::vector<std::string> with all tags found
     */
    std::vector<std::string>
    get_problem_tags(const types::polygon::Credentials &creds, const std::string &problem_id)
    {
        httplib::Params params;
        params.emplace("problemId", problem_id);
        auto result = get("problem.viewTags", creds, params);
        auto tags_json = nlohmann::json::parse(result->body).at("result");
        return tags_json.get<std::vector<std::string> >();
    }

    types::polygon::TestsVector
    get_problem_tests(const types::polygon::Credentials &creds, const std::string &problem_id)
    {
        httplib::Params params;
        params.emplace("problemId", problem_id);
        params.emplace("testset", "tests");
        auto result = get("problem.tests", creds, params);
        auto tests_json = nlohmann::json::parse(result->body).at("result");
        auto tests = tests_json.get<types::polygon::TestsVector>();
        return tests;
    }

} // namespace cptools::api::polygon