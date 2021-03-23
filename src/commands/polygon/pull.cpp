#include <getopt.h>
#include <string>

#include "commands/polygon/polygon.h"
#include "commands/polygon/pull.h"
#include "config.h"
#include "error.h"
#include "exceptions.h"
#include "fs.h"
#include "message.h"

namespace cptools::commands::polygon::pull {

static const string help_message{
    R"message(
Pulls a problem checker, validator and tests from the Polygon problem
set in config.json. It uses the credentials in ~/.cp-tools-config.json to
connect to Polygon's API.
The options are:

    Option          Description

    -h              Generates this help message.
    --help
)message"};

static struct option longopts[] = {{"help", no_argument, NULL, 'h'}};

int run(int argc, char *const argv[], std::ostream &out, std::ostream &err) {
  int option = -1;
  while ((option = getopt_long(argc, argv, "h", longopts, NULL)) != -1) {
    switch (option) {
    case 'h':
      out << help_message << "\n";
      return CP_TOOLS_OK;

    default:
      err << help_message << "\n";
      return CP_TOOLS_ERROR_POLYGON_INVALID_OPTION;
    }
  }

  std::string problem_id = "";

  try {
    auto config_json = config::read_config_file();
    problem_id = config::get_polygon_problem_id(config_json);
  } catch (const exceptions::inexistent_file_error &e) {
    err << message::failure(e.what());
    return CP_TOOLS_EXCEPTION_INEXISTENT_FILE;
  }

  if (problem_id == "") {
    err << message::failure(
        "Couldn't find the problem id in the configuration file.");
    return CP_TOOLS_ERROR_POLYGON_NO_PROBLEM_ID;
  }

  auto config_path = fs::get_default_config_path();
  auto creds = polygon::get_credentials_from_file(config_path);

  try {
    auto checker = api::polygon::get_problem_checker(creds, problem_id);
    out << message::success(checker);
  } catch (const exceptions::polygon_api_error &e) {
    err << message::failure(e.what());
    return CP_TOOLS_ERROR_POLYGON_API;
  }

  return CP_TOOLS_OK;
}
} // namespace cptools::commands::polygon::pull