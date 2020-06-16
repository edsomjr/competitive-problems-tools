#ifndef CP_TOOLS_MESSAGE_H
#define CP_TOOLS_MESSAGE_H

#include <string>

using std::string;

namespace cptools::message {

    string success();
    string failure();
    string warning();

    string trace(const string& errors);
}

#endif
