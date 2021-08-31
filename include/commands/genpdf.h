#ifndef CP_TOOLS_GENPDF_H
#define CP_TOOLS_GENPDF_H

#include <iostream>

namespace cptools::commands::genpdf {

// Main routine
int run(int argc, char *const argv[]);

int generate_pdf(const std::string &doc_class, const std::string &language, int flags,
                 const std::string &label, const std::string &outfile, bool tutorial);

} // namespace cptools::commands::genpdf

#endif
