#include <iostream>
#include <sstream>

#include "cli/cli.h"
#include "cli/format.h"
#include "cli/styles.h"

namespace cptools::cli {

std::ostream *output_stream = &std::cout;
std::ostream *error_stream = &std::cerr;

void set_output_stream(std::ostream &stream) { output_stream = &stream; }

void set_error_stream(std::ostream &stream) { error_stream = &stream; }

void write(const message_type type, const std::string &message, bool is_error) {
    const auto prefix_it = styles::message_type_prefixes.find(type);
    const auto prefix_styles_it = styles::prefix_style.find(type);

    const auto prefix_styles =
        prefix_styles_it == styles::prefix_style.end() ? 0 : prefix_styles_it->second;

    std::string prefix = "";
    if (prefix_it != styles::message_type_prefixes.end()) {
        prefix = format::apply(prefix_it->second, prefix_styles + format::emph::BOLD);
        prefix += ' ';
    }

    const auto style = styles::message_type_styles.at(type);
    const auto formatted = format::apply(message, style);

    const auto final_message = prefix + formatted;
    const auto target_stream =
        (type >= message_type::trace or is_error) ? error_stream : output_stream;

    *target_stream << final_message << std::endl;
}

void write_trace(const std::string &message) {
    std::istringstream iss(message);
    std::ostringstream oss;
    std::string line;

    oss << std::endl;
    while (std::getline(iss, line))
        oss << '\t' + line << std::endl;

    write(message_type::trace, oss.str());
}

} // namespace cptools::cli