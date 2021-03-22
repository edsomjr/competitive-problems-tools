#ifndef CPTOOLS_EXCEPTIONS_H
#define CPTOOLS_EXCEPTIONS_H

#include <exception>
#include <string>

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
} // namespace cptools::exceptions

#endif