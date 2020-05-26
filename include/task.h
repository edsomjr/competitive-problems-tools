#ifndef CP_TOOLS_TASK_H
#define CP_TOOLS_TASK_H

#include <iostream>
#include <string>
#include <vector>

// Contest preparation related tasks
namespace cptools::task {

    std::vector<std::pair<std::string, std::string>> generate_io_files(const std::string& testset,
        std::ostream& out, std::ostream& err, bool gen_output = true);    
}

#endif
