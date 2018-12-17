#ifndef CP_TOOLS_GENTEX_H
#define CP_TOOLS_GENTEX_H

#include <string>
//#include <variant>

//#include "json.hpp"


//struct GentexError {
//    int error_code;
//    std::string message;

//    GentexError(int ec, std::string& msg) : error_code(ec), message(std::move(msg)) {}
//};

extern int gentex(int argc, char * const argv[]);

//extern std::variant<std::string, GentexError>
//generate_latex(const string& problem, const json& config);

#endif
