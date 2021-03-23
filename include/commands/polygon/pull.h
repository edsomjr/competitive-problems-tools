#ifndef CP_TOOLS_POLYGON_PULL_H
#define CP_TOOLS_POLYGON_PULL_H

#include <ostream>

namespace cptools::commands::polygon::pull {
int run(int argc, char *const argv[], std::ostream &out, std::ostream &err);
void get_checker(const api::polygon::Credentials &creds,
                 const std::string &problem_id);
void get_validator(const api::polygon::Credentials &creds,
                   const std::string &problem_id);
} // namespace cptools::commands::polygon::pull

#endif
