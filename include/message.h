#ifndef CP_TOOLS_MESSAGE_H
#define CP_TOOLS_MESSAGE_H

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace cptools::message {

string info(const string &text = "");
string header(const string &text = "");
string success(const string &text = "");
string failure(const string &text = "");
string warning(const string &text = "");
string trace(const string &errors);

} // namespace cptools::message

#endif
