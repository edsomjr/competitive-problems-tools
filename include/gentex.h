#ifndef CP_TOOLS_GENTEX_H
#define CP_TOOLS_GENTEX_H

#include <iostream>
#include <map>

namespace cptools::gentex {

    // Auxiliary routines and definitions
    typedef enum { INCLUDE_AUTHOR = 1, INCLUDE_CONTEST = 2 } Flags;

    bool validate_language(const std::string& lang);

    int list_document_classes(std::ostream& out, std::ostream& err);

    int generate_latex(const std::string& doc_class, const std::string& language, 
        int flags, const std::string& label, std::ostream& out, std::ostream& err);

    int generate_tutorial_latex(const std::string& doc_class, const std::string& language, 
        int flags, const std::string& label, std::ostream& out, std::ostream& err);
    // Main routine
    int run(int argc, char * const argv[], std::ostream& out, std::ostream& err);

}

#endif
