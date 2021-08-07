#include "logger/logger.h"
#include "logger/format.h"

namespace cptools::logger {

u_int8_t current_log_level = INFO;
std::ostream *current_log_stream = &std::cout;
std::ostream *current_err_log_stream = &std::cerr;

void set_log_level(log_level level) { current_log_level = level; }

void set_log_stream(std::ostream *stream) { current_log_stream = stream; }

void set_err_log_stream(std::ostream *stream) { current_log_stream = stream; }

std::string level_to_string(log_level level) {
    auto name = level_names.at(level);
    return name;
}

void log(log_level level, const std::string &message) {
    if (level < current_log_level)
        return;

    std::string level_name = "[" + level_to_string(level) + "]";
    auto level_formatted = format::apply(level_name, format::emph::BOLD + level_colors.at(level));

    auto formatter = formatters.at(level);
    auto formatted_msg = level_formatted + " " + formatter(message);

    if (level >= ERROR) {
        *current_err_log_stream << formatted_msg << std::endl;
    } else {
        *current_log_stream << formatted_msg << std::endl;
    }
}

} // namespace cptools::logger