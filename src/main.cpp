#include <iostream>
#include <unistd.h>

#include "defs.h"

std::string usage()
{
    return "Usage: " NAME " [-h] [-v] action";
}

std::string version()
{
    std::string header { NAME " " VERSION "\n" };
    std::string body { 
R"body(License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

Written by Edson Alves.)body" };

    return header + body;
}

int main(int argc, char* const argv[])
{
    if (argc < 2)
    {
        std::cout << usage() << '\n';
        return -1;
    }

    int option = -1;

    while ((option = getopt(argc, argv, "hv")) != -1)
    {
        switch (option) {
        case 'v':
            std::cout << version() << '\n';
            return 0;
        
        default:
            break;
        }
    }

    return 0;
}

