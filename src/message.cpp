#include "message.h"
#include "format.h"

#include <map>
#include <sstream>
#include <iomanip>

using std::map;
using std::setw;
using std::left;
using std::istringstream;
using std::ostringstream;

namespace cptools::message {

    string info(const string& text)
    {
        auto msg = format::apply(text, format::color::GRAY);
        
        return msg;
    }

    string success(const string& text)
    {
        auto ok = format::apply("Ok!", format::emph::BOLD + format::color::GREEN);
        auto msg = format::apply(text, format::color::GRAY);
        
        return ok + ' ' + msg;
    }

    string failure(const string& text)
    {
        auto fail = format::apply("Fail!", format::emph::BOLD + format::color::RED);
        auto msg = format::apply(text, format::emph::ITALIC + format::color::RED);
        
        return fail + ' ' + msg;
    }

    string warning(const string& text)
    {
        auto warning = format::apply("Warning!", format::emph::BOLD + format::color::YELLOW);
        auto msg = format::apply(text, format::emph::ITALIC + format::color::YELLOW);
        
        return warning + ' ' + msg;
    }

    string trace(const string& errors)
    {
        istringstream iss(errors);
        ostringstream oss;

        oss << '\n';
        oss << format::apply("Errors:", format::emph::BOLD + format::color::VIOLET) << '\n';

        string line;

        while (getline(iss, line))
            oss << format::apply("\t" + line, format::emph::ITALIC + format::color::VIOLET) << '\n';

        return oss.str();
    }

}
