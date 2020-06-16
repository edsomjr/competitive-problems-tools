#include "message.h"

#include <map>
#include <sstream>
#include <iomanip>

using std::map;
using std::setw;
using std::left;
using std::istringstream;
using std::ostringstream;

namespace cptools::message {

    namespace color {
        int BASE = 0;
        int END = BASE + 0;

        int RED = BASE + 1;
        int GREEN = BASE + 2;
        int BLUE = BASE + 3;
        int YELLOW = BASE + 4;
        int VIOLET = BASE + 5;
        int GRAY = BASE + 6;
        int BEIGE = BASE + 7;
    }

    namespace format {
        int BASE = 1000;

        int PLAIN = BASE + 0;
        int BOLD = BASE + 1;
        int ITALIC = BASE + 2;
        int UNDERLINE = BASE + 3;
    }

    namespace align {
        int BASE = 2000;

        int LEFT = BASE + 0;
        int RIGHT = BASE + 1;
    }

    namespace preset {
        int BASE = 3000;

        int PS_AC = BASE + 0;
        int PS_WA = BASE + 1;
        int PS_PE = BASE + 2;
        int PS_INT = BASE + 3;
        int PS_FLOAT = BASE + 4;
        int PS_COUNTER = BASE + 5;
    }

    static map<int, string> codes {
        { color::END, "\033[0m" },
        { color::RED, "\033[91m" },
        { color::GREEN, "\033[92m" },
        { color::BLUE, "\033[94m" },
        { color::GRAY, "\033[90m" },
        { color::YELLOW, "\033[93m" },
        { color::VIOLET, "\033[35m" },
        { color::BEIGE, "\033[36m" },
        { format::BOLD, "\033[1m" },
        { format::ITALIC, "\033[3m" },
        { format::UNDERLINE, "\033[4m" },
        // AC = BOLD + GREEN
        { preset::PS_AC, "\033[1m\033[92m" },
        // WA = BOLD + RED
        { preset::PS_WA, "\033[1m\033[91m" },
        // PE = VIOLET
        { preset::PS_PE, "\033[35m" },
        // INT = BLUE
        { preset::PS_INT, "\033[94m" },
        // FLOAT = BEIGE
        { preset::PS_FLOAT, "\033[36m" },
        // COUNTER = BOLD + GRAY
        { preset::PS_COUNTER, "\033[1m\033[90m" },
    };

    string success()
    {
        return codes[format::BOLD] + codes[color::GREEN] + "Ok!" + codes[color::END];
    }

    string failure()
    {
        return codes[format::BOLD] + codes[color::RED] + "Fail!" + codes[color::END];
    }

    string warning()
    {
        return codes[format::BOLD] + codes[color::YELLOW] + "Warning!" + codes[color::END];
    }

    string trace(const string& errors)
    {
        istringstream iss(errors);
        ostringstream oss;

        oss << '\n';
        oss << codes[format::BOLD] << codes[color::VIOLET] << "Errors:\n" << codes[color::END];
        string line;

        while (getline(iss, line))
            oss << codes[format::ITALIC] << codes[color::VIOLET] + '\t' + line + '\n';

        oss << codes[color::END];

        return oss.str();
    }

    string formatted(const string& text, size_t size, int pos, const vector<int>& spec)
    {
        ostringstream oss;

        for (auto x : spec)
            oss << codes[x];

        if (pos == align::LEFT)
            oss << left;

        oss << setw(size) << text << codes[color::END];

        return oss.str();
    }
}
