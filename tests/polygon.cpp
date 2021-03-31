#include <filesystem>
#include <fstream>
#include <getopt.h>
#include <string>

#include "catch.hpp"
#include "commands/polygon/polygon.h"
#include "error.h"
#include "fs.h"
#include "util.h"

using cptools::commands::polygon::run;

const std::filesystem::path config_path = cptools::fs::get_default_config_path();
const std::filesystem::path config_bkp = config_path.generic_string() + ".bkp";
const std::string default_json_invalid{"{\"polygon\":{\"key\": \"\", \"secret\": \"\"}}"};

SCENARIO("Command polygon", "[polygon]") {
    auto config_path_exists = cptools::fs::exists(config_path).ok;
    auto config_bkp_exists = cptools::fs::exists(config_bkp).ok;
    if (not config_bkp_exists and config_path_exists)
        copy_file(config_path, config_bkp);

    GIVEN("An execution of the command") {
        WHEN("There is no options") {
            AND_WHEN("There's no credentials file in user's home") {
                std::filesystem::remove(config_path);
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
                std::filesystem::remove(config_path);
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

        WHEN("There are valid credentials") {
            // Load valid credentials giving priotity to environtment variables
            auto polygon_key_env = getenv("POLYGON_KEY");
            auto polygon_secret_env = getenv("POLYGON_SECRET");
            bool use_env = polygon_key_env && polygon_secret_env;

            string polygon_key, polygon_secret;
            if (not use_env) {
                auto config = cptools::util::read_json_file(config_bkp);
                polygon_key = cptools::util::get_json_value<string>(config, "polygon|key", "");
                polygon_secret =
                    cptools::util::get_json_value<string>(config, "polygon|secret", "");
            } else {
                polygon_key = string(polygon_key_env);
                polygon_secret = string(polygon_secret_env);
            }

            AND_WHEN("They are in a valid credentials file in user's home") {
                // Insert credentials in the valid json config file
                auto valid_json = "{\"polygon\":{\"key\":\"" + polygon_key + "\",\"secret\":\"" +
                                  polygon_secret + "\"}}";

                if (cptools::fs::exists(config_path).ok)
                    std::filesystem::remove(config_path);
                std::ofstream valid_config(config_path);
                valid_config << valid_json;
                valid_config.close();

                THEN("The output must be a connection successful message") {
                    int argc = 2;
                    char *const argv[]{(char *)"cp-tools", (char *)"polygon"};
                    optind = 1;

                    std::ostringstream out, err;
                    auto rc = run(argc, argv, out, err);

                    REQUIRE(rc == CP_TOOLS_OK);
                }
            }

            AND_WHEN("Valid credentials are passed in the command line") {
                THEN("The output must be a connection successful message") {
                    int argc = 6;
                    char *const argv[]{(char *)"cp-tools", (char *)"polygon",
                                       (char *)"-s",       (char *)polygon_secret.c_str(),
                                       (char *)"-k",       (char *)polygon_key.c_str()};
                    optind = 1;

                    std::ostringstream out, err;
                    auto rc = run(argc, argv, out, err);

                    REQUIRE(rc == CP_TOOLS_OK);
                }
            }
        }
    }

    if (cptools::fs::exists(config_bkp).ok) {
        std::filesystem::remove(config_path);
        std::filesystem::copy_file(config_bkp, config_path);
        std::filesystem::remove(config_bkp);
    }
}
