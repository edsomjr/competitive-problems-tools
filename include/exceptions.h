#ifndef CPTOOLS_EXCEPTIONS_H
#define CPTOOLS_EXCEPTIONS_H

#include <exception>
#include <string>

namespace cptools::exceptions {
    struct inexistent_file : std::exception {
        const std::string _path;

        inexistent_file(const std::string &path) : _path(path) {};

        const char* what() const noexcept;
    };
}

#endif