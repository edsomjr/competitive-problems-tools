#ifndef CPTOOLS_EXCEPTIONS_H
#define CPTOOLS_EXCEPTIONS_H

#include <exception>
#include <string>

namespace cptools::exceptions {
struct inexistent_file_error : std::exception {
  std::string path;

  inexistent_file_error(const std::string &file_path) : path(file_path){};
  const char *what() const noexcept;
};
} // namespace cptools::exceptions

#endif