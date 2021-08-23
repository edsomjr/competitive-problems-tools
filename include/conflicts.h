#ifndef CP_TOOLS_CONFLICTS_H
#define CP_TOOLS_CONFLICTS_H

#include <string>

namespace cptools::conflicts {

std::string solve_files(const std::string &local_file_name, const std::string &remote_file_name,
                        const std::string &remote_file_content, const bool forced);

} // namespace cptools::conflicts

#endif