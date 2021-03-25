#ifndef CP_TOOLS_API_POLYGON_H
#define CP_TOOLS_API_POLYGON_H

#include <string>
#include <vector>

#include "httplib.h"
#include "types/polygon.h"

using std::string;

namespace cptools::api::polygon {

bool test_connection(const types::polygon::Credentials &creds);

httplib::Result get(std::string method,
                    const types::polygon::Credentials &creds,
                    httplib::Params &params);

std::string get_problem_file_name(const std::string &tool_type,
                                  const types::polygon::Credentials &creds,
                                  const std::string &problem_id);

std::string get_problem_file(const std::string &file_name,
                             const std::string &tool_type,
                             const types::polygon::Credentials &creds,
                             const std::string &problem_id);

std::vector<types::polygon::Solution>
get_problem_solutions(const types::polygon::Credentials &creds,
                      const std::string problem_id);

string generate_api_sig(const string &method_name,
                        const httplib::Params &params,
                        const types::polygon::Credentials &creds);

} // namespace cptools::api::polygon

#endif
