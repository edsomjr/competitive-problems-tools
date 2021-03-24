#ifndef CP_TOOLS_POLYGON_PULL_H
#define CP_TOOLS_POLYGON_PULL_H

#include <ostream>

namespace cptools::commands::polygon::pull {

int run(int argc, char *const argv[], std::ostream &out, std::ostream &err);

void get_tool_file(const std::string tool_name,
                   const api::polygon::Credentials &creds,
                   const std::string &problem_id);

} // namespace cptools::commands::polygon::pull

#endif
