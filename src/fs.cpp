#include <filesystem>
#include <fstream>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "cli/cli.h"
#include "dirs.h"
#include "error.h"
#include "fs.h"
#include "util.h"

namespace cptools::fs {

const Result make_result(bool res) { return Result{res, CP_TOOLS_OK, ""}; }

const Result make_result(bool res, int rc, const std::filesystem::filesystem_error &e) {
    Result result{res, rc, ""};
    result.error_message = e.what();
    return result;
}

const Result make_result(bool res, int rc, const std::string err_msg) {
    return Result{res, rc, err_msg};
}

const Result create_directory(const std::string &path) {
    if (exists(path).ok)
        return make_result(true);

    bool created = false;
    try {
        created = std::filesystem::create_directories(path);
    } catch (const std::filesystem::filesystem_error &err) {
        return make_result(false, CP_TOOLS_ERROR_CPP_FILESYSTEM_CREATE_DIRECTORY, err);
    }

    if (created)
        return make_result(created);
    else
        return make_result(created, CP_TOOLS_ERROR_CPP_FILESYSTEM_CREATE_DIRECTORY,
                           "Failed to create directory " + path);
}

const Result exists(const std::string &path) {
    bool ok = false;
    try {
        ok = std::filesystem::exists(path);
    } catch (const std::filesystem::filesystem_error &err) {
        return make_result(false, CP_TOOLS_ERROR_CPP_FILESYSTEM_EXISTS, err);
    }

    return make_result(ok);
}

const Result copy(const std::string &src, const std::string &dst, bool overwrite) {
    std::filesystem::copy_options options = std::filesystem::copy_options::recursive;
    if (overwrite)
        options |= std::filesystem::copy_options::overwrite_existing;

    try {
        std::filesystem::copy(src, dst, options);
    } catch (const std::filesystem::filesystem_error &err) {
        return make_result(false, CP_TOOLS_ERROR_CPP_FILESYSTEM_COPY, err);
    }

    return make_result(true);
}

const Result remove(const std::string &path) {
    if (not fs::exists(path).ok)
        return make_result(true);

    bool removed = false;
    try {
        removed = std::filesystem::remove_all(path);
    } catch (const std::filesystem::filesystem_error &err) {
        return make_result(false, CP_TOOLS_ERROR_CPP_FILESYSTEM_REMOVE, err);
    }

    if (removed)
        return make_result(removed);
    else
        return make_result(removed, CP_TOOLS_ERROR_CPP_FILESYSTEM_REMOVE,
                           "Impossible to remove " + path);
}

const Result equivalent(const std::string &p1, const std::string &p2) {
    bool same = false;
    try {
        same = std::filesystem::equivalent(p1, p2);
    } catch (const std::filesystem::filesystem_error &err) {
        return make_result(false, CP_TOOLS_ERROR_CPP_FILESYSTEM_EQUIVALENT, err);
    }

    if (same)
        return make_result(same);
    else
        return make_result(same, CP_TOOLS_ERROR_CPP_FILESYSTEM_EQUIVALENT,
                           "Impossible to compare " + p1 + " and " + p2);
}

const Result is_directory(const std::string &path) {
    bool is_dir = false;
    try {
        is_dir = std::filesystem::is_directory(path);
    } catch (const std::filesystem::filesystem_error &err) {
        make_result(false, CP_TOOLS_ERROR_CPP_FILESYSTEM_IS_DIRECTORY, err);
    }

    return make_result(is_dir);
}

const Result is_file(const std::string &path) {
    bool is_file = false;
    try {
        is_file = std::filesystem::is_regular_file(path);
    } catch (const std::filesystem::filesystem_error &err) {
        make_result(false, CP_TOOLS_ERROR_CPP_FILESYSTEM_IS_FILE, err);
    }

    return make_result(is_file);
}

const Result rename(const std::string old_path, const std::string new_path) {
    try {
        std::filesystem::rename(old_path, new_path);
    } catch (const std::filesystem::filesystem_error &err) {
        return make_result(false, CP_TOOLS_ERROR_CPP_FILESYSTEM_RENAME, err);
    }
    return make_result(true);
}

void overwrite_file(const std::string dst, const std::string content) {
    std::ofstream file;
    file.open(dst);
    file << content;
    file.close();
}

std::string get_home_dir() {
    char *homedir = getenv("HOME");
    if (homedir == NULL) {
        auto pw_struct = getpwuid(getuid());
        homedir = pw_struct->pw_dir;
    }
    return std::string(homedir);
}

std::string get_default_config_path() {
    auto homedir = get_home_dir();
    auto config_path = homedir + "/" + CP_TOOLS_CONFIG_FILE;
    return config_path;
}

/**
 * @brief Reads a file and returns the SHA512 hash of its content.
 *
 * @param filename path to the file to be hashed
 * @return std::string
 */
std::string sha_512_file(std::string filename) {
    auto content = read_file(filename);
    return util::sha_512(content);
}

/**
 * @brief Reads a file and returns its content as a string.
 *
 * @param filename path of the file to be read
 * @return std::string
 */
std::string read_file(std::string filename) {
    std::ifstream file;
    file.open(filename);
    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    file.close();
    return content;
}

} // namespace cptools::fs