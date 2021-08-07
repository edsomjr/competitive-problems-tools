#include "logger/message.h"
#include "logger/format.h"

#include <iomanip>
#include <map>
#include <sstream>

namespace cptools::logger::message {

std::string info(const std::string &text) {
    auto msg = format::apply(text, format::color::GRAY);

    return msg;
}

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
    auto fail = format::apply("Fail!", format::emph::BOLD + format::color::RED);
    auto msg = format::apply(text, format::emph::ITALIC + format::color::RED);

    return fail + ' ' + msg;
}

std::string warning(const std::string &text) {
    auto warning = format::apply("Warning!", format::emph::BOLD + format::color::YELLOW);
    auto msg = format::apply(text, format::emph::ITALIC + format::color::YELLOW);

    return warning + ' ' + msg;
}

std::string trace(const std::string &errors) {
    std::istringstream iss(errors);
    std::ostringstream oss;

    oss << '\n';
    oss << format::apply("Tracing:", format::emph::BOLD + format::color::VIOLET) << '\n';

    std::string line;

    while (getline(iss, line))
        oss << format::apply("\t" + line, format::emph::ITALIC + format::color::VIOLET) << '\n';

    return oss.str();
}

} // namespace cptools::logger::message
