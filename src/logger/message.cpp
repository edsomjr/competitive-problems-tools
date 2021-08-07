#include "logger/message.h"
#include "logger/format.h"

#include <iomanip>
#include <map>
#include <sstream>

namespace cptools::logger::message {

std::string debug(const std::string &text) {
    auto msg = format::apply(text, format::color::GRAY);
    return msg;
}

std::string info(const std::string &text) { return text; }

std::string header(const std::string &text) {
    auto msg = format::apply(text, format::emph::BOLD);
    return msg;
}

std::string success(const std::string &text) {
    auto ok = format::apply("Ok!", format::emph::BOLD + format::color::GREEN);
    auto msg = format::apply(text, format::color::GRAY);

    return ok + ' ' + msg;
}

std::string failure(const std::string &text) {
    auto msg = format::apply(text, format::color::RED);
    return msg;
}

std::string warning(const std::string &text) {
    auto msg = format::apply(text, format::color::YELLOW);
    return msg;
}

std::string trace(const std::string &errors) {
    std::istringstream iss(errors);
    std::ostringstream oss;

    oss << '\n';

    std::string line;

    while (getline(iss, line))
        oss << format::apply("\t" + line, format::emph::ITALIC + format::color::VIOLET) << "\n";

    return oss.str();
}

} // namespace cptools::logger::message
