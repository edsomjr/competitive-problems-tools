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

bool test_connection(const Credentials &creds);

httplib::Result get(std::string method, const Credentials &creds,
                    httplib::Params &params);

std::string get_problem_file(const std::string &tool_name,
                             const Credentials &creds,
                             const std::string &problem_id);

string generate_api_sig(const string &method_name,
                        const httplib::Params &params,
                        const Credentials &creds);

} // namespace cptools::api::polygon

#endif
