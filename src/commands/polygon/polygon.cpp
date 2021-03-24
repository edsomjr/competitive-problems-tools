#include <getopt.h>
#include <iostream>
#include <unordered_map>

#include "api/polygon.h"
#include "commands/polygon/polygon.h"
#include "commands/polygon/pull.h"
#include "defs.h"
#include "error.h"
#include "exceptions.h"
#include "fs.h"
#include "message.h"
#include "util.h"

// Raw strings
static const string help_message{
    R"message(
Validates the connection to the Polygon API using the credentials in ~/.cp-tools-config.json.
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

)message"};

namespace cptools::commands::polygon {

// Global variables
std::unordered_map<string, int (*)(int, char *const[], ostream &, ostream &)>
    commands{{"pull", pull::run}};

static struct option longopts[] = {{"help", no_argument, NULL, 'h'},
                                   {"key", required_argument, NULL, 'k'},
                                   {"secret", required_argument, NULL, 's'},
                                   {"creds", required_argument, NULL, 'c'},
                                   {0, 0, 0, 0}};

// Auxiliary routines
string usage() { return "Usage: " NAME " polygon [-h]"; }

string help() { return usage() + help_message; }

types::polygon::Credentials get_credentials_from_file(const string &filepath) {
  nlohmann::json loaded_json;
  loaded_json = util::read_json_file(filepath);

  types::polygon::Credentials creds;
  creds.key = util::get_json_value(loaded_json, "polygon|key", creds.key);
  creds.secret =
      util::get_json_value(loaded_json, "polygon|secret", creds.secret);

  creds.key = util::strip(creds.key);
  creds.secret = util::strip(creds.secret);

  return creds;
}

// API functions
int run(int argc, char *const argv[], ostream &out, ostream &err) {
  int option = -1;
  types::polygon::Credentials creds;
  string creds_file{fs::get_default_config_path()};
  bool creds_from_cmd{false};
  bool creds_from_file{false};

  if (argc >= 3) {
    std::string command{argv[2]};
    auto it = commands.find(command);
    if (it != commands.end())
      return commands[command](argc, argv, out, err);
  }

  while ((option = getopt_long(argc, argv, "hk:s:c:", longopts, NULL)) != -1) {
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

    default:
      err << help() << '\n';
      return CP_TOOLS_ERROR_POLYGON_INVALID_OPTION;
    }
  }

  if (creds_from_file and creds_from_cmd) {
    err << message::failure(
        "A file for the credentials was specified along credentials in command "
        "line. Use only one method.\n");
    return CP_TOOLS_ERROR_POLYGON_MUTUAL_CHOICE_ERROR;
  }

  if (not creds_from_cmd) {
    try {
      creds = get_credentials_from_file(creds_file);
    } catch (const exceptions::inexistent_file_error &e) {
      err << message::failure(string(e.what()));
      return CP_TOOLS_EXCEPTION_INEXISTENT_FILE;
    }
  }

  creds.key = util::strip(creds.key);
  creds.secret = util::strip(creds.secret);

  bool connected = false;
  try {
    connected = api::polygon::test_connection(creds);
  } catch (const exceptions::polygon_api_error &e) {
    err << message::trace(e.what());
    connected = false;
  }

  if (not connected) {
    err << message::failure("Could not connect with the given credentials.");
    return CP_TOOLS_ERROR_POLYGON_CANT_CONNECT;
  }

  out << message::success("The given credentials are valid.");

  return CP_TOOLS_OK;
}
} // namespace cptools::commands::polygon
