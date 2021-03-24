#ifndef CPTOOLS_EXCEPTIONS_H
#define CPTOOLS_EXCEPTIONS_H

#include <exception>
#include <string>

#include "httplib.h"

namespace cptools::exceptions {
struct inexistent_file_error : std::exception {
  std::string what_message;

public:
  inexistent_file_error(const std::string &file_path) {
    this->what_message =
        "File is inexistent or impossible to access: " + file_path;
  }

  const char *what() const noexcept override {
    return this->what_message.c_str();
  }
};

struct polygon_api_error : std::exception {
  std::string what_message;

public:
  polygon_api_error(const std::string &msg)
      : what_message("Polygon API: " + msg) {}

  polygon_api_error(const httplib::Result &result) {
    this->what_message = "Polygon API: " + std::to_string(result->status);
    this->what_message += ": " + result->body;
  }

  const char *what() const noexcept override {
    return this->what_message.c_str();
  }
};

struct invalid_config_error : std::exception {
  std::string what_message;

public:
  invalid_config_error(const std::string &msg)
      : what_message("Invalid config.json: " + msg) {}

  const char *what() const noexcept override {
    return this->what_message.c_str();
  }
};

} // namespace cptools::exceptions

#endif