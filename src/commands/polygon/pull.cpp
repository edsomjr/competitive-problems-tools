#include <string>

#include "commands/polygon/pull.h"
#include "error.h"
#include "message.h"

namespace cptools::commands::polygon::pull {
int run(int argc, char *const argv[], std::ostream &out, std::ostream &err) {
  out << message::success("Running pull " + std::to_string(argc) +
                          std::string(argv[0]));
  err << " ";
  return CP_TOOLS_OK;
}
} // namespace cptools::commands::polygon::pull