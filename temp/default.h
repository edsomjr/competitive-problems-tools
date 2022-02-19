#ifndef CP_TOOLS_DEFAULT_H
#define CP_TOOLS_DEFAULT_H

#include <iostream>
#include "plugin.h"


class Default : public Plugin {
public:
    Default();
    ~Default() {};

    int execute(const Args& args) override;
};

#endif
