#ifndef CP_TOOLS_MESSAGE_H
#define CP_TOOLS_MESSAGE_H

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace cptools::message {

    namespace color {
        extern int RED;
        extern int GREEN;
        extern int BLUE;
        extern int YELLOW;
        extern int VIOLET;
        extern int GRAY;
        extern int BEIGE;
    };

    namespace format {
        extern int PLAIN;
        extern int BOLD;
        extern int ITALIC;
        extern int UNDERLINE;
    }

    namespace align {
        extern int LEFT;
        extern int RIGHT;
    }

    namespace preset {
        extern int PS_AC;
        extern int PS_WA;
        extern int PS_PE;
        extern int PS_INT;
        extern int PS_FLOAT;
        extern int PS_COUNTER;
    }

    string success();
    string failure();
    string warning();

    string trace(const string& errors);
    string formatted(const string& text, size_t size, int pos, const vector<int>& spec);

}

#endif
