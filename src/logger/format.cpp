#include "logger/format.h"

#include <iomanip>
#include <map>
#include <sstream>

namespace cptools::logger::format {

static constexpr long long END = 0;

static std::map<long long, std::string> codes{
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

std::string apply(const std::string &text, long long specs, size_t size) {
    std::ostringstream oss;

    for (long long mask = 1024; mask <= specs; mask <<= 1)
        if (mask & specs)
            oss << codes[mask];

    if (specs & align::LEFT)
        oss << std::left;

    size = std::max(size, text.size());

    oss << std::setw(size) << text << codes[END];

    return oss.str();
}

} // namespace cptools::logger::format
