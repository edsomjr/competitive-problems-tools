#ifndef CP_TOOLS_LOGGER_H
#define CP_TOOLS_LOGGER_H

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

#include "logger/message.h"

namespace cptools::logger {

typedef std::function<std::string(const std::string &)> formatter_t;

enum log_level { DEBUG, INFO, WARNING, ERROR, FATAL };

const std::unordered_map<log_level, formatter_t> formatters = {{DEBUG, message::trace},
                                                               {INFO, message::info},
                                                               {WARNING, message::warning},
                                                               {ERROR, message::failure},
                                                               {FATAL, message::failure}};

void set_log_level(log_level level);

void set_log_stream(std::ostream *stream);

void log(log_level level, const std::string &message);

} // namespace cptools::logger

#endif