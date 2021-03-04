#include "format.h"

#include <iomanip>
#include <map>
#include <sstream>

using std::istringstream;
using std::left;
using std::map;
using std::max;
using std::ostringstream;
using std::setw;

namespace cptools::format {

static constexpr long long END = 0;

static map<long long, string> codes{
    {END, "\033[0m"},
    {color::BLACK, "\033[30m"},
    {color::RED, "\033[91m"},
    {color::GREEN, "\033[92m"},
    {color::YELLOW, "\033[93m"},
    {color::BLUE, "\033[94m"},
    {color::MAGENTA, "\033[95m"},
    {color::CYAN, "\033[96m"},
    {color::WHITE, "\033[97m"},
    {color::GRAY, "\033[90m"},
    {color::VIOLET, "\033[35m"},
    {color::BEIGE, "\033[36m"},
    {emph::BOLD, "\033[1m"},
    {emph::ITALIC, "\033[3m"},
    {emph::UNDERLINE, "\033[4m"},
};

string apply(const string &text, long long specs, size_t size) {
  ostringstream oss;

  for (long long mask = 1024; mask <= specs; mask <<= 1)
    if (mask & specs)
      oss << codes[mask];

  if (specs & align::LEFT)
    oss << left;

  size = max(size, text.size());

  oss << setw(size) << text << codes[END];

  return oss.str();
}

} // namespace cptools::format
