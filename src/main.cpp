#include <iostream>

#include "defs.h"

std::string usage()
{
    return "Usage: " NAME " [-h] [-v] action";
}

int main(int argc, const char* [])
{
    if (argc < 2)
    {
        std::cout << usage() << '\n';
        return -1;
    }

    return 0;
}

