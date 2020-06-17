#include "format.h"

#include <map>
#include <sstream>
#include <iomanip>

using std::max;
using std::map;
using std::setw;
using std::left;
using std::istringstream;
using std::ostringstream;

namespace cptools::format {

    static long long END = 0;

    namespace color {
        long long BASE = (1 << 20);
        long long RED = (BASE << 1);
        long long GREEN = (BASE << 2);
        long long YELLOW = (BASE << 3);
        long long BLUE = (BASE << 4);
        long long MAGENTA = (BASE << 5);
        long long CYAN = (BASE << 6);
        long long WHITE = (BASE << 7);
        long long VIOLET = (BASE << 8);
        long long GRAY = (BASE << 9);
        long long BEIGE = (BASE << 10);
        long long BLACK = (BASE << 11);
    }

    namespace emph {
        long long BASE = (1 << 10);

        long long BOLD = (BASE << 1);
        long long ITALIC = (BASE << 2);
        long long UNDERLINE = (BASE << 3);
    }

    namespace align {
        long long BASE = 1;

        long long LEFT = (BASE << 1);
        long long RIGHT = (BASE << 2);
    }

    namespace style {
        long long AC = emph::BOLD + align::LEFT + color::GREEN;
        long long WA = emph::BOLD + align::LEFT + color::RED;
        long long PE = align::LEFT + color::VIOLET;
        long long INT = color::BLUE;
        long long FLOAT = color::BEIGE;
        long long COUNTER = color::CYAN;
    }

    static map<long long, string> codes {
        { END, "\033[0m" },
        { color::BLACK, "\033[30m" },
        { color::RED, "\033[91m" },
        { color::GREEN, "\033[92m" },
        { color::YELLOW, "\033[93m" },
        { color::BLUE, "\033[94m" },
        { color::MAGENTA, "\033[95m" },
        { color::CYAN, "\033[96m" },
        { color::WHITE, "\033[97m" },
        { color::GRAY, "\033[90m" },
        { color::VIOLET, "\033[35m" },
        { color::BEIGE, "\033[36m" },
        { emph::BOLD, "\033[1m" },
        { emph::ITALIC, "\033[3m" },
        { emph::UNDERLINE, "\033[4m" },
    };

    string apply(const string& text, long long specs, size_t size)
    {
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

}
