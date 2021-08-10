#include <iostream>

#include "cli/writer.h"
#include "commands/cptools.h"

int main(int argc, char *const argv[]) {
    {
        using namespace cptools::cli::writer;
        write(writer_type::none, "Test with none");
        write(writer_type::header, "Test with header");
        write(writer_type::info, "Test with info");
        write(writer_type::ok, "Test with ok");
        write(writer_type::warning, "Test with warning");
        write(writer_type::trace, "Trace1\nTrace2");
        write(writer_type::failure, "Test with failure");
    }
    return cptools::commands::run(argc, argv, std::cout, std::cerr);
}
