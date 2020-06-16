#include "message.h"

#include <map>
#include <sstream>

using std::map;
using std::istringstream;
using std::ostringstream;

namespace cptools::message {

    enum Color { RED, GREEN, BLUE, GRAY, YELLOW, VIOLET, BOLD, ITALIC, UNDERLINE, END };

    static map<Color, string> colors {
        { RED, "\033[91m" },
        { GREEN, "\033[92m" },
        { BLUE, "\033[94m" },
        { GRAY, "\033[90m" },
        { YELLOW, "\033[93m" },
        { VIOLET, "\033[35m" },
        { BOLD, "\033[1m" },
        { ITALIC, "\033[3m" },
        { UNDERLINE, "\033[4m" },
        { END, "\033[0m" },
    };

    string success()
    {
        return colors[BOLD] + colors[GREEN] + "Ok!" + colors[END];
    }

    string failure()
    {
        return colors[BOLD] + colors[RED] + "Fail!" + colors[END];
    }

    string warning()
    {
        return colors[BOLD] + colors[YELLOW] + "Warning!" + colors[END];
    }

    string trace(const string& errors)
    {
        istringstream iss(errors);
        ostringstream oss;

        oss << '\n';
        oss << colors[BOLD] << colors[VIOLET] << "Errors:\n" << colors[END];
        string line;

        while (getline(iss, line))
            oss << colors[ITALIC] << colors[VIOLET] + '\t' + line + '\n';

        oss << colors[END];

        return oss.str();
    }
}
