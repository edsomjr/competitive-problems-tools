#include "exceptions.h"

namespace cptools::exceptions {
const char *inexistent_file::what() const noexcept {
  auto msg = "File is inexistent or impossible to access.\n";
  return msg;
}
} // namespace cptools::exceptions