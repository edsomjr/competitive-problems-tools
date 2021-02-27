#include <iostream>

#include "polygon.h"
#include "defs.h"
#include "error.h"
#include "message.h"


// Raw strings
static const string help_message {
R"message(
Connects to the Polygon API using the credentials in ~/.cp-tools-config.json. The options are:

    Option          Description

    -h              Generates this help message.
    --help

)message"
};

namespace cptools::polygon {
    // Auxiliary routines
    string usage()
    {
        return "Usage: " NAME " polygon [-h]";
    }

    string help()
    {
        return usage() + help_message;
    }

    // API functions
    int run(int argc, char* const argv[], ostream& out, ostream& err)
    {
        out << argc << '\n' << argv << '\n';
        err << message::failure("This method is not yet implemented.\n");
        return CP_TOOLS_ERROR_INVALID_COMMAND;
    }
}
