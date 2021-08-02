#ifndef CP_TOOLS_PACK4_BOCA_H
#define CP_TOOLS_PACK4_BOCA_H

#include <iostream>

// Devo colocar cada comando de empacotamento no mesmo namespace ou em namespaces separados?
// A função genboca deveria ser renomeada para `pack`?
namespace cptools::commands::pack4 {

int genboca(std::ostream &out, std::ostream &err);

} // namespace cptools::commands::pack4

#endif
