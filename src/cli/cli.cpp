#include <iostream>
#include <sstream>

#include "cli/cli.h"
#include "cli/format.h"
#include "cli/styles.h"

namespace cptools::cli {

std::ostream *output_stream = &std::cout;
std::ostream *error_stream = &std::cerr;

/**
 * @brief Set the output stream object to be used for normal output.
 *
 * @param stream The stream object to be used
 */
void set_output_stream(std::ostream &stream) { output_stream = &stream; }

/**
 * @brief Set the error stream object to be used for error output.
 *
 * @param stream The stream object to be used
 */
void set_error_stream(std::ostream &stream) { error_stream = &stream; }

/**
 * @brief Get the prefix string formatted to a given message type.
 *
 * @param type The message type to get the prefix for
 * @return std::string The prefix string formatted given the message type
 */
std::string get_prefix(const fmt type) {
    const auto prefix_it = styles::message_type_prefixes.find(type);
    const auto prefix_styles_it = styles::prefix_style.find(type);
    std::string result = "";

    if (prefix_it != styles::message_type_prefixes.end()) {
        result = prefix_it->second;
        result += ' ';
    }

    if (prefix_styles_it != styles::prefix_style.end()) {
        result = format::apply(result, prefix_styles_it->second + format::emph::BOLD);
    }

    return result;
}

/**
 * @brief Writes the given message to the correct stream.
 *
 * @param type The type of the message
 * @param message The message to write
 * @param is_error If it should go to the error stream
 */
void write(const fmt type, const std::string &message, bool is_error) {
    const auto prefix = get_prefix(type);
    const auto style_it = styles::message_type_styles.find(type);
    std::string result = message;

    if (style_it != styles::message_type_styles.end() and type != fmt::none)
        result = format::apply(message, style_it->second);

    const auto final_message = prefix + result;
    const auto target_stream = (type >= fmt::trace or is_error) ? error_stream : output_stream;

    *target_stream << final_message << std::endl;
}

/**
 * @brief Writes a multi lines message formatted to be used in a trace.
 *
 * @param message The message to write
 */
void write_trace(const std::string &message) {
    std::istringstream iss(message);
    std::ostringstream oss;
    std::string line;

    oss << std::endl;
    while (std::getline(iss, line))
        oss << '\t' + line << std::endl;

    write(fmt::trace, oss.str());
}

} // namespace cptools::cli