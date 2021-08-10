#ifndef CP_TOOLS_CLI_WRITER_H
#define CP_TOOLS_CLI_WRITER_H

#include <functional>
#include <string>
#include <unordered_map>

#include "cli/message.h"

namespace cptools::cli::writer {

typedef std::function<std::string(std::string)> type_formatter;

enum writer_type { none, header, info, ok, warning, trace, failure };

const std::unordered_map<writer_type, type_formatter> formatters = {
    {writer_type::none, [](std::string s) { return s; }},
    {writer_type::header, message::header},
    {writer_type::info, message::info},
    {writer_type::ok, message::success},
    {writer_type::warning, message::warning},
    {writer_type::trace, message::trace},
    {writer_type::failure, message::failure}};

void set_output_stream(std::ostream &stream);

void set_error_stream(std::ostream &stream);

void write(const writer_type type, const std::string &message);

} // namespace cptools::cli::writer

#endif