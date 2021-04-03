#include "commands/cptools.h"
#include <iostream>

int main(int argc, char *const argv[]) {
    return cptools::commands::run(argc, argv, std::cout, std::cerr);
}
