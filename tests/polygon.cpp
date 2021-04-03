#include <filesystem>
#include <getopt.h>
#include <string>

#include "catch.hpp"
#include "commands/polygon/polygon.h"
#include "error.h"
#include "fs.h"
#include "util.h"

using cptools::commands::polygon::run;

const std::string default_json_invalid{"{\"polygon\":{\"key\": \"\", \"secret\": \"\"}}"};
const std::string user_config_path = cptools::fs::get_default_config_path();

SCENARIO("Command polygon", "[polygon]") {

    GIVEN("An execution of the command") {
        std::filesystem::remove(user_config_path + ".bkp");
        std::filesystem::copy_file(user_config_path, user_config_path + ".bkp");

        WHEN("There is no options") {
            AND_WHEN("There's no credentials file in user's home") {
                std::filesystem::remove(user_config_path);

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
                cptools::fs::overwrite_file(user_config_path, default_json_invalid);

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
            const auto env_key = getenv("POLYGON_KEY");
            const auto env_secret = getenv("POLYGON_SECRET");

            REQUIRE(env_key != NULL);
            REQUIRE(env_secret != NULL);

            const std::string polygon_key(env_key);
            const std::string polygon_secret(env_secret);

            AND_WHEN("They are in a valid credentials file in user's home") {
                auto valid_json = "{\"polygon\":{\"key\":\"" + polygon_key + "\",\"secret\":\"" +
                                  polygon_secret + "\"}}";

                cptools::fs::overwrite_file(user_config_path, valid_json);

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

        std::filesystem::remove(user_config_path);
        std::filesystem::copy_file(user_config_path + ".bkp", user_config_path);
        std::filesystem::remove(user_config_path + ".bkp");
    }
}
