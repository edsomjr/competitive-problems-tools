#ifndef CP_TOOLS_CLI_MESSAGE_H
#define CP_TOOLS_CLI_MESSAGE_H

#include <string>

namespace cptools::cli::message {

std::string info(const std::string &text = "");
std::string header(const std::string &text = "");
std::string success(const std::string &text = "");
std::string failure(const std::string &text = "");
std::string warning(const std::string &text = "");
std::string trace(const std::string &errors);

} // namespace cptools::cli::message

#endif
