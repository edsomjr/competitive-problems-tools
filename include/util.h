#ifndef CP_TOOLS_UTIL_H
#define CP_TOOLS_UTIL_H

#include <string>
#include <vector>

std::vector<std::string> split(const std::string& s, char delim = ' ');
std::string strip(const std::string& s);
std::string strip(const std::string& s, char c);

#endif
