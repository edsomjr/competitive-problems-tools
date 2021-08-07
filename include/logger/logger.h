#ifndef CP_TOOLS_LOGGER_H
#define CP_TOOLS_LOGGER_H

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

#include "logger/format.h"
#include "logger/message.h"

namespace cptools::logger {

typedef std::function<std::string(const std::string &)> formatter_t;

enum log_level { DEBUG, INFO, WARNING, TRACE, ERROR, FATAL };

const std::unordered_map<log_level, formatter_t> formatters = {
    {DEBUG, message::debug},   {INFO, message::info},     {WARNING, message::warning},
    {ERROR, message::failure}, {FATAL, message::failure}, {TRACE, message::trace}};

const std::unordered_map<log_level, std::string> level_names = {
    {DEBUG, "DEBUG"}, {INFO, "INFO"},   {WARNING, "WARNING"},
    {ERROR, "ERROR"}, {FATAL, "FATAL"}, {TRACE, "TRACE"}};

const std::unordered_map<log_level, long long> level_colors = {
    {DEBUG, format::color::GRAY}, {INFO, format::color::BLUE}, {WARNING, format::color::YELLOW},
    {ERROR, format::color::RED},  {FATAL, format::color::RED}, {TRACE, format::color::VIOLET}};

void set_log_level(log_level level);

void set_err_log_stream(std::ostream *stream);

void set_log_stream(std::ostream *stream);

std::string level_to_string(log_level level);

void log(log_level level, const std::string &message);

} // namespace cptools::logger

#endif