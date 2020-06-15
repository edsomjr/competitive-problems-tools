#include "message.h"

#include <map>

using std::map;

namespace cptools::message {

    enum Color { RED, GREEN, BLUE, YELLOW, BOLD, END };

    static map<Color, string> colors {
        { RED, "\033[91m" },
        { GREEN, "\033[92m" },
        { YELLOW, "\033[93m" },
        { BLUE, "\033[94m" },
        { BOLD, "\033[1m" },
        { END, "\033[0m" },
    };

    string sucess()
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

}
