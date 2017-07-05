# Compilation setup
include_directories(${ejproblem_SOURCE_DIR}/include)
add_compile_options(-W -Wall -pedantic -std=c++11)

# Project library and executable
add_library(ejproblemlib ini.cpp util.cpp)

add_executable(ejproblem2 main.cpp)
target_link_libraries(ejproblem2 ejproblemlib)

# Install files and directories
install(PROGRAMS ejproblem2 DESTINATION .)

# Packing
set(CPACK_PACKAGE_NAME "ejproblem")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "A problem formatter tool")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Edson Alves")
set(CPACK_PACKAGE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")

set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local/bin")
set(CPACK_GENERATOR "DEB")

include(CPack)
