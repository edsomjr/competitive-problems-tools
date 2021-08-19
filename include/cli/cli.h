#ifndef CP_TOOLS_CLI_H
#define CP_TOOLS_CLI_H

#include <string>

namespace cptools::cli {

enum fmt { none, header, info, ok, warning, trace, error };

void set_output_stream(std::ostream &stream);

void set_error_stream(std::ostream &stream);

void write(const fmt type, const std::string &message = "", bool is_error = false);

void write_trace(const std::string &message);

} // namespace cptools::cli

#endif