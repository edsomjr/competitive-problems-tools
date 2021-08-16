#ifndef CP_TOOLS_CLI_STYLES_H
#define CP_TOOLS_CLI_STYLES_H

#include <unordered_map>

#include "cli/cli.h"
#include "cli/format.h"

namespace cptools::cli::styles {

const std::unordered_map<message_type, long long> message_type_styles = {
    {message_type::none, 0},
    {message_type::header, format::emph::BOLD},
    {message_type::info, format::color::GRAY},
    {message_type::ok, format::emph::ITALIC + format::color::GREEN},
    {message_type::warning, format::emph::ITALIC + format::color::YELLOW},
    {message_type::trace, format::emph::ITALIC + format::color::VIOLET},
    {message_type::error, format::emph::ITALIC + format::color::RED}};

const std::unordered_map<message_type, std::string> message_type_prefixes = {
    {message_type::ok, "Ok!"},
    {message_type::warning, "Warning!"},
    {message_type::trace, "Trace:"},
    {message_type::error, "Failed!"}};

const std::unordered_map<message_type, long long> prefix_style = {
    {message_type::ok, format::color::GREEN},
    {message_type::warning, format::color::YELLOW},
    {message_type::trace, format::color::VIOLET},
    {message_type::error, format::color::RED}};

} // namespace cptools::cli::styles

#endif