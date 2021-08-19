#ifndef CP_TOOLS_CLI_STYLES_H
#define CP_TOOLS_CLI_STYLES_H

#include <unordered_map>

#include "cli/cli.h"
#include "cli/format.h"

namespace cptools::cli::styles {

const std::unordered_map<fmt, long long> message_type_styles = {
    {fmt::none, 0},
    {fmt::header, format::emph::BOLD},
    {fmt::info, format::color::GRAY},
    {fmt::ok, format::emph::ITALIC + format::color::GREEN},
    {fmt::warning, format::emph::ITALIC + format::color::YELLOW},
    {fmt::trace, format::emph::ITALIC + format::color::VIOLET},
    {fmt::error, format::emph::ITALIC + format::color::RED}};

const std::unordered_map<fmt, std::string> message_type_prefixes = {
    {fmt::ok, "Ok!"}, {fmt::warning, "Warning!"}, {fmt::trace, "Trace:"}, {fmt::error, "Failed!"}};

const std::unordered_map<fmt, long long> prefix_style = {{fmt::ok, format::color::GREEN},
                                                         {fmt::warning, format::color::YELLOW},
                                                         {fmt::trace, format::color::VIOLET},
                                                         {fmt::error, format::color::RED}};

} // namespace cptools::cli::styles

#endif