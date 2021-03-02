#include <iostream>
#include "commands/cptools.h"


int main(int argc, char* const argv[])
{
    return cptools::commands::run(argc, argv, std::cout, std::cerr);
}
