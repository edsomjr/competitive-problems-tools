#include "logger/logger.h"

namespace cptools::logger {

u_int8_t current_log_level = INFO;
std::ostream *current_log_stream = &std::cout;
std::ostream *current_err_log_stream = &std::cerr;

void set_log_level(log_level level) { current_log_level = level; }

void set_log_stream(std::ostream *stream) { current_log_stream = stream; }

void set_err_log_stream(std::ostream *stream) { current_log_stream = stream; }

void log(log_level level, const std::string &message) {
    if (level < current_log_level)
        return;

    auto formatter = formatters.find(level)->second;
    if (level >= ERROR) {
        *current_err_log_stream << formatter(message) << std::endl;
    } else {
        *current_log_stream << formatter(message) << std::endl;
    }
}

} // namespace cptools::logger