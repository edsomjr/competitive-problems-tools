#ifndef CP_TOOLS_TASK_H
#define CP_TOOLS_TASK_H

#include <iostream>
#include <string>
#include <vector>

using std::pair;
using std::string;
using std::vector;
using std::ostream;

// Problem preparation related tasks
namespace cptools::task {

    namespace tools {
        extern const int ALL;
        extern const int CHECKER;
        extern const int VALIDATOR;
        extern const int GENERATOR;
        extern const int INTERACTOR;
    }

    vector<pair<string, string>> generate_io_files(const string& testset, ostream& out, 
        ostream& err, bool gen_output = true);    

    int build_tools(string& error, int tools = tools::ALL, const string& where = ".");
    int gen_exe(string& error, const string& source, const string& dest, const string& where = ".");

}

#endif
