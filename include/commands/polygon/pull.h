#ifndef CP_TOOLS_POLYGON_PULL_H
#define CP_TOOLS_POLYGON_PULL_H

#include <ostream>

#include "types/polygon.h"

namespace cptools::commands::polygon::pull {

int run(int argc, char *const argv[], std::ostream &out, std::ostream &err);

void pull_tool_file(const std::string tool_name, const types::polygon::Credentials &creds,
                    const std::string &problem_id, bool forced);

void pull_solutions(const types::polygon::Credentials &creds, const std::string &problem_id,
                    bool forced);

} // namespace cptools::commands::polygon::pull

#endif
