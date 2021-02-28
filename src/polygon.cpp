#include <iostream>
#include <getopt.h>

#include "polygon.h"
#include "defs.h"
#include "error.h"
#include "message.h"


// Raw strings
static const string help_message {
R"message(
Connects to the Polygon API using the credentials in ~/.cp-tools-config.json.
It is possible to define the credentials file or pass them in command line.
The options are:

    Option          Description

    -h              Generates this help message.
    --help

    -k              API key provided at Polygon website.
    --key

    -s              API secret provided at Polygon website.
    --secret

    -c              JSON file with the credentials (key and secret) for Polygon.
    --creds

    -n              Disable the input for credentials from the stdin. Useful for automated pipelines.
    --no-stdin

)message"
};

namespace cptools::polygon {

    // Global variables
    static struct option longopts[] = {
       { "help", no_argument, NULL, 'h' },
       { "key", required_argument, NULL, 'k' },
       { "secret", required_argument, NULL, 's' },
       { "creds", required_argument, NULL, 'c' },
       { "no-stdin", required_argument, NULL, 'n' },
       { 0, 0, 0, 0 }
    };

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
        int option = -1;
        Credentials creds;
        string creds_file { "~/.cp-tools-config.json" };
        bool creds_from_cmd = false;
        bool creds_from_file = false;
        bool creds_set_from_file = false;
        bool no_stdin = false;

        while ((option = getopt_long(argc, argv, "hk:s:c:n", longopts, NULL)) != -1)
        {
            switch (option) {
            case 'h':
                out << help() << '\n';
                return 0;

            case 'k':
                creds.key = string(optarg);
                creds_from_cmd = true;
                break;

            case 's':
                creds.secret = string(optarg);
                creds_from_cmd = true;
                break;

            case 'c':
                creds_file = string(optarg);
                creds_from_file = true;
                break;

            case 'n':
                no_stdin = true;
                break;

            default:
                err << help() << '\n';
                return CP_TOOLS_ERROR_POLYGON_INVALID_OPTION;
            }
        }

        if (creds_from_file and creds_from_cmd) {
            err << message::failure("A file for the credentials was specified along credentials in command line. Use only one method.\n");
            return CP_TOOLS_ERROR_POLYGON_MUTUAL_CHOICE_ERROR;
        }

        if (not creds_from_cmd) {
            // try to get from the file
            // set creds_set_from_file
        }

        if (not no_stdin and not creds_set_from_file) {
            // get from stdin
            // if dont get from here, throw error
        }

        // TODO: API call to test if the credentials work (Polygon API module + HTTPS module)

        err << message::failure("This method is not yet implemented.\n");

        return CP_TOOLS_ERROR_INVALID_COMMAND;
    }
}
