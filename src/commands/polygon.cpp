#include <iostream>
#include <getopt.h>

#include "commands/polygon.h"
#include "api/polygon.h"
#include "defs.h"
#include "error.h"
#include "message.h"
#include "fs.h"
#include "config.h"
#include "exceptions.h"


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

namespace cptools::commands::polygon {

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
        api::polygon::Credentials creds;
        string creds_file { fs::get_default_config_path() };
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
            nlohmann::json loaded_json;
            try
            {
                out << message::info("Getting credentials from " + creds_file + "\n");
                loaded_json = config::read(creds_file);
            }
            catch(const exceptions::inexistent_file& e)
            {
                err << message::failure(string(e.what()));
                return CP_TOOLS_EXCEPTION_INEXISTENT_FILE;
            }

            creds.key = config::get(loaded_json, "polygon|key", creds.key);
            creds.secret = config::get(loaded_json, "polygon|secret", creds.secret);

            creds_set_from_file = true;
        }

        if (not no_stdin and not creds_set_from_file) {
            // get from stdin
            // if dont get from here, throw error
        }

        creds.key = util::strip(creds.key);
        creds.secret = util::strip(creds.secret);

        auto connected = api::polygon::test_connection(creds);

        if(not connected) {
            err << message::failure("Could not connect with the given credentials.");
            return CP_TOOLS_ERROR_POLYGON_CANT_CONNECT;
        }

        out << message::success("The given credentials are valid.");

        return CP_TOOLS_OK;
    }
}
