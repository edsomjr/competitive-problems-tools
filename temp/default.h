#ifndef CP_TOOLS_DEFAULT_H
#define CP_TOOLS_DEFAULT_H

#include <iostream>
#include "command.h"

class Default : public Command {
public:
    Default();
    ~Default() { std::cout << "Default Destrutor\n"; }
};

#endif
