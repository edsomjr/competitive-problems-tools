#ifndef CP_TOOLS_DEFAULT_H
#define CP_TOOLS_DEFAULT_H

#include <iostream>
#include "plugin.h"


class Default : public Plugin {
public:
    Default();
    ~Default() { }

    std::string help() const override;
    int execute(const Args& args) override;

    std::string strip_plugin_name_from_message(std::string message) const;
};

#endif
