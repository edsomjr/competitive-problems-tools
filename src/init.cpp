#include <iostream>
#include <unistd.h>

#include "defs.h"
#include "init.h"


static std::string usage()
{
    return "Usage: " NAME " init [-h]";
}

static std::string help()
{
    const std::string message {
R"message(
Initialize an empty directory with template files. If a file already exists, it is not overridden.
)message" };

    return usage() + message;
}


int init(int argc, char * const argv[])
{
    int option = -1;

    while ((option = getopt(argc, argv, "h")) != -1)
    {
        switch (option) {
        case 'h':
            std::cout << help() << '\n';
            return 0;

        default:
            std::cout << help() << '\n';
            return -1;
        }
    }

    return 0;
}
