#ifndef CP_TOOLS_FS_H
#define CP_TOOLS_FS_H

#include <filesystem>
#include <string>

#include "types/common.h"

namespace cptools::fs {

using Result = types::common::Result;
using types::common::make_result;

const Result create_directory(const std::string &path);
const Result exists(const std::string &path);
const Result copy(const std::string &src, const std::string &dst, bool overwrite = false);
const Result remove(const std::string &path);
const Result equivalent(const std::string &p1, const std::string &p2);
const Result is_directory(const std::string &path);
const Result is_file(const std::string &path);
const Result rename(const std::string old_path, const std::string new_path);

void overwrite_file(const std::string dst, const std::string content);

std::string get_home_dir();
std::string get_default_config_path();
std::string read_file(std::string filename);
std::string sha_512_file(std::string filename);

} // namespace cptools::fs

#endif