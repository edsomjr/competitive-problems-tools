#ifndef CPTOOLS_EXCEPTIONS_H
#define CPTOOLS_EXCEPTIONS_H

#include <exception>
#include <string>

namespace cptools::exceptions {
struct inexistent_file : std::exception {
  const char *what() const noexcept;
};
} // namespace cptools::exceptions

#endif