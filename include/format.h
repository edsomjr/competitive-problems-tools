#ifndef CP_TOOLS_FORMAT_H
#define CP_TOOLS_FORMAT_H

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace cptools::format {

    namespace color {
        constexpr long long BASE = (1 << 20);
        constexpr long long RED = (BASE << 1);
        constexpr long long GREEN = (BASE << 2);
        constexpr long long YELLOW = (BASE << 3);
        constexpr long long BLUE = (BASE << 4);
        constexpr long long MAGENTA = (BASE << 5);
        constexpr long long CYAN = (BASE << 6);
        constexpr long long WHITE = (BASE << 7);
        constexpr long long VIOLET = (BASE << 8);
        constexpr long long GRAY = (BASE << 9);
        constexpr long long BEIGE = (BASE << 10);
        constexpr long long BLACK = (BASE << 11);
    }

    namespace emph {
        constexpr long long BASE = (1 << 10);

        constexpr long long BOLD = (BASE << 1);
        constexpr long long ITALIC = (BASE << 2);
        constexpr long long UNDERLINE = (BASE << 3);
    }

    namespace align {
        constexpr long long BASE = 1;

        constexpr long long LEFT = (BASE << 1);
        constexpr long long RIGHT = (BASE << 2);
    }

    namespace style {
        constexpr long long AC = align::LEFT + color::GREEN;
        constexpr long long WA = align::LEFT + color::RED;
        constexpr long long PE = align::LEFT + color::VIOLET;
        constexpr long long CE = align::LEFT + color::YELLOW + emph::ITALIC;
        constexpr long long TLE = align::LEFT + color::YELLOW;
        constexpr long long RTE = align::LEFT + color::VIOLET + emph::ITALIC;
        constexpr long long MLE = align::LEFT + color::BEIGE + emph::ITALIC;
        constexpr long long FAIL = align::LEFT + color::MAGENTA + emph::BOLD;
        constexpr long long UNDEF = align::LEFT + color::CYAN + emph::ITALIC;

        constexpr long long INT = color::BLUE;
        constexpr long long FLOAT = color::BEIGE;
        constexpr long long COUNTER = color::CYAN;
    }

    string apply(const string& text, long long specs, size_t size = 0);

}

#endif
