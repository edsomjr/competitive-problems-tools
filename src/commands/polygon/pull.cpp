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

// Static variables
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

// Functions
void get_checker(const api::polygon::Credentials &creds,
                 const std::string &problem_id) {
  auto checker = api::polygon::get_problem_checker(creds, problem_id);
  auto config = config::read_config_file();
  auto checker_file_name = config::get_checker_file_name(config);
  fs::overwrite_file(checker_file_name, checker);
}

void get_validator(const api::polygon::Credentials &creds,
                   const std::string &problem_id) {
  auto validator = api::polygon::get_problem_validator(creds, problem_id);
  auto config = config::read_config_file();
  auto validator_file_name = config::get_validator_file_name(config);
  fs::overwrite_file(validator_file_name, validator);
}

// API
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
    get_checker(creds, problem_id);
    get_validator(creds, problem_id);
    // get_tests();
    // get_solutions();
  } catch (const exceptions::polygon_api_error &e) {
    err << message::failure(e.what());
    return CP_TOOLS_ERROR_POLYGON_API;
  }

  out << message::success("Pull completed");

  return CP_TOOLS_OK;
}
} // namespace cptools::commands::polygon::pull