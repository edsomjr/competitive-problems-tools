#include "exceptions.h"

namespace cptools::exceptions {
const char *inexistent_file_error::what() const noexcept {
  auto msg = "File is inexistent or impossible to access: " + this->path;
  return msg.c_str();
}
} // namespace cptools::exceptions