#ifndef CP_TOOLS_UTIL_H
#define CP_TOOLS_UTIL_H

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace cptools::util {
vector<string> split(const string &s, char delim = ' ');

string strip(const string &s);
string strip(const string &s, char c);
} // namespace cptools::util

#endif
