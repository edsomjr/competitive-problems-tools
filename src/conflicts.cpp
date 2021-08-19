#include <filesystem>

#include "cli/cli.h"
#include "conflicts.h"
#include "fs.h"
#include "util.h"

namespace cptools::conflicts {

/**
 * @brief Deals with conflicts between the current and the new file. Returns the file path of the
 * new file.
 *
 * @param local_file_name path to the local file
 * @param remote_file_name path to the remote file
 * @param remote_file_content content of the remote file
 * @param forced if it should be forced and overwrite the local file
 * @return std::string name of the new file
 */
std::string solve_files(const std::string &local_file_name, const std::string &remote_file_name,
                        const std::string &remote_file_content, const bool forced) {
    auto local_file_path = std::filesystem::path(local_file_name);
    auto remote_file_path = std::filesystem::path(remote_file_name);

    auto new_file_path = std::filesystem::path(local_file_name);
    new_file_path.replace_filename(remote_file_path.filename());

    auto local_file_hash = fs::sha_512_file(local_file_path);
    auto remote_file_hash = util::sha_512(remote_file_content);

    bool equal_paths = fs::equivalent(local_file_path, new_file_path).ok;
    bool different_hashes = local_file_hash != remote_file_hash;

    if (forced) {
        fs::remove(local_file_path);
    } else if (different_hashes and equal_paths) {
        std::filesystem::path target(local_file_path);
        target += ".old";
        cli::write(cli::message_type::warning, "The file '" + local_file_path.string() +
                                                   "' will be moved to '" + target.string() + "'");
        fs::copy(local_file_path, target);
    } else if (not equal_paths and not different_hashes) {
        cli::write(cli::message_type::info, "The file '" + local_file_path.string() +
                                                "' will be moved to '" + new_file_path.string() +
                                                "'");
        fs::remove(local_file_path);
    }
    fs::overwrite_file(new_file_path, remote_file_content);

    return new_file_path;
}

} // namespace cptools::conflicts