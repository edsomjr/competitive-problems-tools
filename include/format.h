#ifndef CP_TOOLS_FORMAT_H
#define CP_TOOLS_FORMAT_H

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace cptools::format {

    namespace color {
        extern long long RED;
        extern long long GREEN;
        extern long long BLUE;
        extern long long YELLOW;
        extern long long VIOLET;
        extern long long GRAY;
        extern long long BEIGE;
    };

    namespace emph {
        extern long long PLAIN;
        extern long long BOLD;
        extern long long ITALIC;
        extern long long UNDERLINE;
    }

    namespace align {
        extern long long LEFT;
        extern long long RIGHT;
    }

    namespace style {
        extern long long AC;
        extern long long WA;
        extern long long PE;
        extern long long INT;
        extern long long FLOAT;
        extern long long COUNTER;
    }

//    string formatted(const string& text, size_t size, long long pos, const vector<long long>& spec);

    string apply(const string& text, long long specs, size_t size = 0);
}

#endif
