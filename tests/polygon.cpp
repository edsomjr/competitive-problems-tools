#include <filesystem>
#include <fstream>
#include <getopt.h>
#include <string>

#include "catch.hpp"
#include "commands/polygon.h"
#include "error.h"
#include "fs.h"

using namespace std::filesystem;
using cptools::commands::polygon::run;

const path config_path = cptools::fs::get_default_config_path();
const path config_bkp = config_path.generic_string() + ".bkp";
const std::string default_json_invalid{
    "{\"polygon\":{\"key\": \"\", \"secret\": \"\"}}"};

SCENARIO("Command polygon", "[polygon]") {
  if (not exists(config_bkp) and exists(config_path))
    copy_file(config_path, config_bkp);

  GIVEN("An execution of the command") {
    WHEN("There is no options") {
      AND_WHEN("There's no credentials file in user's home") {
        remove(config_path);
        int argc = 2;
        char *const argv[]{(char *)"cp-tools", (char *)"polygon"};
        optind = 1;
        THEN("The output must be an error for inexistent file") {
          std::ostringstream out, err;
          auto rc = run(argc, argv, out, err);

          REQUIRE(rc == CP_TOOLS_EXCEPTION_INEXISTENT_FILE);
        }
      }

      AND_WHEN("There's an invalid credentials file in user's home") {
        remove(config_path);
        std::ofstream config_stream{config_path};
        config_stream << default_json_invalid;
        config_stream.close();

        int argc = 2;
        char *const argv[]{(char *)"cp-tools", (char *)"polygon"};
        optind = 1;

        THEN("The output must be an error because it wasn't able to connect") {
          std::ostringstream out, err;
          auto rc = run(argc, argv, out, err);

          REQUIRE(rc == CP_TOOLS_ERROR_POLYGON_CANT_CONNECT);
        }
      }

      AND_WHEN("There's a valid credentials file in user's home") {
        THEN("The output must be a connection successful message") {
          /* TODO: find a way to test valid credentials in the CI and here */
        }
      }
    }

    WHEN("The credentials file and command line credentials are specified") {
      int argc = 6;
      char *const argv[]{(char *)"cp-tools", (char *)"polygon", (char *)"-s",
                         (char *)"secret",   (char *)"-c",      (char *)"file"};
      optind = 1;

      THEN("The output must be an error because of the mutual choice") {
        std::ostringstream out, err;
        auto rc = run(argc, argv, out, err);

        REQUIRE(rc == CP_TOOLS_ERROR_POLYGON_MUTUAL_CHOICE_ERROR);
      }
    }

    WHEN("Invalid credentials are passed in the command line") {
      int argc = 6;
      char *const argv[]{(char *)"cp-tools", (char *)"polygon", (char *)"-s",
                         (char *)"secret",   (char *)"-k",      (char *)"key"};
      optind = 1;

      THEN("The output must be an error saying it wasn't able to connect") {
        std::ostringstream out, err;
        auto rc = run(argc, argv, out, err);

        REQUIRE(rc == CP_TOOLS_ERROR_POLYGON_CANT_CONNECT);
      }
    }

    WHEN("Valid credentials are passed in the command line") {
      THEN("The output must be a connection successful message") {
        /* TODO: find a way to test valid credentials in the CI and here */
      }
    }
  }

  if (exists(config_bkp)) {
    remove(config_path);
    copy_file(config_bkp, config_path);
    remove(config_bkp);
  }
}
