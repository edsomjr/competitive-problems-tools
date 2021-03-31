#ifndef CP_TOOLS_GENTEX_H
#define CP_TOOLS_GENTEX_H

#include <iostream>
#include <map>

using namespace std;

namespace cptools::commands::gentex {
// Auxiliary routines and definitions
namespace flag {
constexpr int INCLUDE_AUTHOR = 1;
constexpr int INCLUDE_CONTEST = 2;
} // namespace flag

bool validate_language(const string &lang);

int list_document_classes(ostream &out, ostream &err);

int generate_latex(const string &doc_class, const string &language, int flags, const string &label,
                   ostream &out, ostream &err);

int generate_tutorial_latex(const string &doc_class, const string &language, int flags,
                            const string &label, ostream &out, ostream &err);

// Main routine
int run(int argc, char *const argv[], ostream &out, ostream &err);
} // namespace cptools::commands::gentex

#endif
