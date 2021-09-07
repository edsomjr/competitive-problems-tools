#include <getopt.h>
#include <limits>

#include "cli/cli.h"
#include "commands/genpdf.h"
#include "commands/gentex.h"
#include "config.h"
#include "dirs.h"
#include "error.h"
#include "fs.h"
#include "task.h"
#include "util.h"
#include "sh.h"


// Raw strings
static const std::string help_message{
    R"message(
Pack the marathon problem for the online judge BOCA (https://github.com/cassiopc/boca/).

    Option          Description

    -b              Defines the problem label. The default value is 'A'.
    --label

    -c              Document class that will be used. The default value
    --class         is 'cp_modern'.

    -g              Problem language. The default value is 'pt_BR'.
    --lang

    -h              Display this help message.
    --help

    -l              Lists all available document classes.
    --list

    -o              Output file. If omitted, the output will be the file 'problem.pdf'.
    --output

    -t              Generates the PDF for the problem's tutorial.
    --tutorial

    --no_author     Omits problem's author.

    --c-time-limit  Defines the maximum timelimit of the problem in the C language.
                    This time is used as a basis for calculating the time of other
                    programming languages.

    --no_contest    problem's contest.


)message"};

namespace cptools::commands::genboca {

typedef enum { NO_AUTHOR = 1000, NO_CONTEST } Value;

static struct option longopts[] = {{"help", no_argument, NULL, 'h'},

                                   // Same genpdf CLI interface
                                   {"label", required_argument, NULL, 'b'},
                                   {"lang", required_argument, NULL, 'g'},
                                   {"list", no_argument, NULL, 'l'},
                                   {"class", required_argument, NULL, 'c'},
                                   {"output", required_argument, NULL, 'o'},
                                   {"tutorial", no_argument, NULL, 't'},
                                   {"no_author", no_argument, NULL, NO_AUTHOR},
                                   {"no_contest", no_argument, NULL, NO_CONTEST},

                                   // Exclusive options for genboca0
                                   {"c-time-limit", required_argument, NULL, 0},
                                   {"cpp-time-limit", required_argument, NULL, 0},
                                   {"java-time-limit", required_argument, NULL, 0},
                                   {"kt-time-limit", required_argument, NULL, 0},
                                   {"py2-time-limit", required_argument, NULL, 0},
                                   {"py3-time-limit", required_argument, NULL, 0},
                                   {0, 0, 0, 0}};

// Auxiliary routines
std::string usage() {
    return "Usage: " NAME " genboca [-h] [-o outfile] [-b label] [-c doc_class] "
           "[-g lang] [-l] [-t] [--no-author] [--no-contest]";
}

std::string help() { return usage() + help_message; }

/**
 * @brief Function that initializes the Boca package build directory.
 *        This function will copy the boca template files to a build directory.
 *
 * @param out
 * @param err
 * @return int Returns CP_TOOLS_OK if everything goes as expected,
 *             or returns a specific error code if a problem occurs.
 */
int create_build_dirs() {
    std::string boca_build_dir{CP_TOOLS_BOCA_BUILD_DIR};

    auto res_remove = fs::remove(boca_build_dir);

    if (not res_remove.ok) {
        cli::write(cli::fmt::error, res_remove.error_message);
        return res_remove.rc;
    }

    auto res_create = fs::create_directory(boca_build_dir);

    if (not res_create.ok) {
        cli::write(cli::fmt::error, res_create.error_message);
        return res_create.rc;
    }

    auto res_copy = cptools::fs::copy(CP_TOOLS_BOCA_TEMPLATES_DIR, boca_build_dir, true);

    if (not res_copy.ok)
        cli::write(cli::fmt::error, res_copy.error_message);

    return res_copy.rc;
}

int execute_genpdf_command(int argc, char *const argv[]) {
    std::string document_class = util::get_from_argv(argc, argv, {"--class", "-c"}, "cp_modern");
    std::string outfile = util::get_from_argv(argc, argv, {"--output", "-o"}, "problem.pdf");
    std::string language = util::get_from_argv(argc, argv, {"--lang", "-g"}, "pt_BR");
    std::string label = util::get_from_argv(argc, argv, {"--label", "-b"}, "A");
    int flags = gentex::flag::INCLUDE_AUTHOR | gentex::flag::INCLUDE_CONTEST;

    // ~gentex::flag::INCLUDE_CONTEST
    // Qual é o char correto aqui?
    if (util::get_from_argv(argc, argv, {"--no_author"}, "") == std::string("")) {
        flags &= (~gentex::flag::INCLUDE_AUTHOR);
    }

    if (util::get_from_argv(argc, argv, {"--no_contest"}, "") == std::string("")) {
        flags &= (~gentex::flag::INCLUDE_CONTEST);
    }

    bool tutorial = false;

    for (int i = 0; i < argc; i++) {
        if (std::string(argv[i]) == std::string("--tutorial") or
            std::string(argv[i]) == std::string("-t")) {
            tutorial = true;
            outfile = "tutorial.pdf";
            break;
        }
    }

    return genpdf::generate_pdf(document_class, language, flags, label, outfile, tutorial);
}

int create_description_dir(int argc, char *const argv[]) {
    // optind is a built-in global variable.
    // It is used to control the current position of argv.

    int res_pdf = execute_genpdf_command(argc, argv);
    if (res_pdf != CP_TOOLS_OK) {
        return res_pdf;
    }

    // Generated pdf file name
    std::string pdf_file = util::get_from_argv(argc, argv, {"--output", "-o"}, "problem.pdf");

    std::string boca_desc_dir{CP_TOOLS_BOCA_BUILD_DIR + std::string("description/")};

    // Copy the pdf to boca's build directory
    auto res_cpy = fs::copy(pdf_file, boca_desc_dir, true);
    if (not res_cpy.ok) {
        cli::write(cli::fmt::error, res_cpy.error_message);
        return res_cpy.rc;
    }

    // Rename the copied file to <LABEL>.pdf
    std::string label = util::get_from_argv(argc, argv, {"--label", "-b"}, "A");
    auto res_rnm = fs::rename(boca_desc_dir + pdf_file, boca_desc_dir + label + ".pdf");
    if (not res_rnm.ok) {
        cli::write(cli::fmt::error, res_rnm.error_message);
        return res_rnm.rc;
    }

    std::string problem_info{boca_desc_dir + std::string("problem.info")};

    auto config = config::read_config_file();

    std::string problem_fullname{
        util::get_json_value(config, "problem|title|pt_BR", std::string("Título do problema"))};

    std::string content{std::string("basename=") + label + '\n' + std::string("fullname=") + '\"' +
                        problem_fullname + "\"\n" + std::string("descfile=") + label + ".pdf" +
                        '\n'};

    fs::overwrite_file(problem_info, content);

    return CP_TOOLS_OK;
}

int create_io_dir() {
    auto pairs = task::generate_io_files("all");

    for (const auto &dir : {std::string("input/"), std::string("output/")}) {

        auto res_cpy =
            fs::copy(CP_TOOLS_BUILD_DIR + std::string("/") + dir, CP_TOOLS_BOCA_BUILD_DIR + dir);

        if (not res_cpy.ok) {
            cli::write(cli::fmt::error, res_cpy.error_message);
            return res_cpy.rc;
        }
    }

    return CP_TOOLS_OK;
}

int find_best_divisor(int timelimit_ms, int t) {

    int l = 1, r = t;

    int local_best = -1;
    int smallest_diff = std::numeric_limits<int>::max();

    while (l <= r) {
        int mid = l + (r - l) / 2;

        int result = (t / mid);
        int diff = std::abs((t / mid) - timelimit_ms);

        if (diff < smallest_diff) {
            smallest_diff = diff;
            local_best = mid;
        }

        if (result == timelimit_ms) {
            break;
        }

        else if (result < timelimit_ms) {
            r = mid - 1;
        }

        else if (result > timelimit_ms) {
            l = mid + 1;
        }
    }

    return local_best;
}

/**
 * @brief Function that will find the best Ratio for the number of milliseconds divided
 * by the number of repetitions that generate a value close to the timelimit_ms
 * parameter. The second parameter is the search range for the numerator.
 *
 * @param timelimit_ms
 * @param numerator_range
 * @return std::pair<int, int> Returns a pair of integers, the first being the numerator
 * and the second being the denominator of the ratio found.
 */
std::pair<int, int> find_optimal_ratio(int timelimit_ms, std::pair<int, int> numerator_range) {
    // Smallest num, div possible
    int global_best_numerator = 1000;
    int global_best_divisor = 1;

    auto [lower_bound, upper_bound] = numerator_range;

    // The proportion that results in the closest to timelimit_ms is not always the best.
    // Proportion that have a very large numerator require the judge to have more
    // execution time. This way we penalize the large numerators to find the best ratio,
    // with the smallest possible numerator that the result approaches the timelimit_ms
    std::map<int, int> seconds_penality;

    int mid_point = lower_bound + (upper_bound - lower_bound) / 2;

    for (int i = lower_bound; i <= upper_bound; i += 1000) {
        if (i < mid_point)
            seconds_penality[i] = 0;
        else {
            seconds_penality[i] = 500;
        }
    }

    // t is in ms, and belongs to the range [1000, 5000]
    for (int t = lower_bound; t <= upper_bound; t += 1000) {
        int local_best_divisor = find_best_divisor(timelimit_ms, t);

        int global_diff = std::abs((global_best_numerator / global_best_divisor) - timelimit_ms);
        int local_diff = std::abs((t / local_best_divisor) - timelimit_ms);

        int approx_timelimit_ms = (t / local_best_divisor);

        // approx_timelimit_ms has to be greater than or equal to timelimit_ms
        if ((approx_timelimit_ms >= timelimit_ms) &&
            (seconds_penality[t] + local_diff < global_diff)) {
            global_best_numerator = t;
            global_best_divisor = local_best_divisor;
        }
    }

    // global_best_numerator is in ms, so divide by 1000 to convert to seconds
    auto [numerator, divisor] =
        util::reduce_fraction(global_best_numerator / 1000, global_best_divisor);

    return {numerator, divisor};
}

int create_limits_dir(int argc, char *const argv[]) {
    auto limit_path{CP_TOOLS_BOCA_BUILD_DIR + std::string("limits/")};

    auto config = config::read_config_file();

    // KBytes
    auto max_file_size = std::stoi({util::get_from_argv(argc, argv, {"--max-file-size"}, "1024")});

    // Mbytes
    auto max_memory_amount{
        util::get_json_value(config, "problem|memory_limit", std::string("1024"))};

    // First try to get the value from the command arguments.
    auto timelimit_ms_str{
        util::get_from_argv(argc, argv, {"--cpp-time-limit", "--c-time-limit"}, "")};

    int timelimit_ms;

    // If the argument is not passed, the value from config.json is used
    if (timelimit_ms_str == std::string("")) {
        // The value obtained here refers to the timelimit of c and c++
        timelimit_ms = util::get_json_value(config, "problem|timelimit", 1000);
    } else {
        timelimit_ms = std::stoi(timelimit_ms_str);
    }

    std::vector<std::pair<std::string, int>> languages{{"c", 1},  {"cpp", 1}, {"java", 3},
                                                       {"kt", 3}, {"py2", 5}, {"py3", 5}};

    for (const auto &[lang, time_ratio] : languages) {

        int lang_timelimit_ms = time_ratio * timelimit_ms;

        // The find_optimal_ratio function will look for the best ratio within that numerator range.
        std::pair<int, int> numerator_range{1000 * time_ratio, 5000 * time_ratio};

        auto [lang_timelimit_s, repetitions_number] =
            find_optimal_ratio(lang_timelimit_ms, numerator_range);

        std::string content{
            std::string("#!/bin/bash\n") +

            // time limit
            std::string("echo ") + std::to_string(lang_timelimit_s) + '\n' +

            // number of repetitions to be performed within the time limit
            std::string("echo ") + std::to_string(repetitions_number) + '\n' +

            // maximum amount of memory per repetition
            std::string("echo ") + max_memory_amount + '\n' +

            // maximum file size
            std::string("echo ") + std::to_string(max_file_size) + '\n' +

            // and shall return zero to indicate no failure
            std::string("exit 0\n")
        };

        fs::overwrite_file(limit_path + lang, content);
    }

    return CP_TOOLS_OK;
}

/**
 * @brief Modify a file that contains a checker.
 *
 *        The modification consists of overwriting the file's main function, in order to
 *        change the order of the parameters passed in argv. In addition to this
 *        modification, a function is added to the `on_exit` routine to change the
 *        returned code.
 *
 * @param checker_file
 * @return int
 */
int modify_checker_file(std::string checker_file)
{
    std::string sed_args = R"sed_cmd(\
-i \
's/'\
'int main(int argc'\
'/'\
'void convert_code(int rc, void*) {\n'\
'    exit(rc + 4);\n'\
'}\n\n'\
'int main2(int argc, char* argv[]);\n\n'\
'int main(int argc, char* argv[]) {\n'\
'    std::swap(argv[1], argv[2]);\n'\
'    std::swap(argv[2], argv[3]);\n'\
'    on_exit(convert_code, NULL);\n'\
'    main2(argc, argv);\n'\
'}\n\n'\
'int main2(int argc'\
'/g' )sed_cmd";

    sed_args += checker_file;

    std::string error;

    auto result = sh::execute("sed", sed_args, "", "");

    if (result.rc != CP_TOOLS_OK) {
        cli::write_trace(result.output);
        return CP_TOOLS_ERROR_GENBOCA_FAILURE_TO_RUN_SED_ON_CHECKER_FILE;
    }

    return CP_TOOLS_OK;
}


/**
 * @brief Function that performs the necessary modifications to adapt the cp-tools
 *        checker with the format expected by BOCA compare scripts.
 *
 * @return int
 */
int create_compare_dir()
{
    auto config = config::read_config_file();

    auto checker_file = util::get_json_value(
        config,
        "tools|checker",
        std::string("tools/checker.cpp")
    );

    std::string checker_cpy("tools/.checker.cpp");

    auto cpy_retn = fs::copy(checker_file, checker_cpy, true);

    if (not cpy_retn.ok) {
        cli::write(cli::fmt::error, cpy_retn.error_message);
        return CP_TOOLS_ERROR_GENBOCA_FAILURE_TO_CREATE_COPY_OF_CHECKER_FILE_FILE;
    }

    auto mdy_retn = modify_checker_file(checker_cpy);

    if (mdy_retn != CP_TOOLS_OK) {
        return mdy_retn;
    }

    auto binary_name = std::string(".base_binary");
    auto binary_path = std::string("tools/") + binary_name;

    auto bld_retn = sh::build(binary_path, checker_cpy);

    if (bld_retn.rc != CP_TOOLS_OK) {
        cli::write(cli::fmt::error, "Can't compile '" + checker_cpy + "'!");
        cli::write_trace(bld_retn.output);
        return bld_retn.rc;
    }

    auto rme_retn = fs::remove(checker_cpy);

    if (not rme_retn.ok) {
        cli::write(cli::fmt::error, rme_retn.error_message);
        return rme_retn.rc;
    }

    auto compare_path{ CP_TOOLS_BOCA_BUILD_DIR + std::string("compare/") };

    auto rnm_retn = fs::rename(binary_path, compare_path + binary_name);

    if (not rnm_retn.ok) {
        cli::write(cli::fmt::error, rnm_retn.error_message);
        return rnm_retn.rc;
    }

    std::vector<std::string> supported_languages{"c", "cpp", "java", "kt", "py2", "py3"};

    for(auto lang : supported_languages) {

        std::string lang_exec_file = compare_path + lang;

        auto bny_cpy_retn = fs::copy(
            compare_path + binary_name,
            lang_exec_file,
            true
        );

        if (not bny_cpy_retn.ok) {
            cli::write(cli::fmt::error, bny_cpy_retn.error_message);
            return {};
        }

        std::string error;
        std::string chmod_args = std::string("755 ") + lang_exec_file;

        auto chmod_retn = sh::execute("chmod", chmod_args, "", "");

        if (chmod_retn.rc != CP_TOOLS_OK) {
            cli::write_trace(chmod_retn.output);
            return CP_TOOLS_ERROR_GENBOCA_FAILURE_TO_ADD_EXECUTION_PERMISSION;
        }
    }

    auto rmv_retn = fs::remove(compare_path + binary_name);

    if (not rmv_retn.ok) {
        cli::write(cli::fmt::error, rmv_retn.error_message);
        return rmv_retn.rc;
    }

    return CP_TOOLS_OK;
}

int genboca(int argc, char *const argv[]) {
    auto rnt = create_build_dirs();
    if (rnt != CP_TOOLS_OK) {
        return CP_TOOLS_ERROR_GENBOCA_FAILURE_TO_CREATE_BUILD_DIRECTORY;
    }

    rnt = create_description_dir(argc, argv);
    if (rnt != CP_TOOLS_OK) {
        return CP_TOOLS_ERROR_GENBOCA_FAILURE_TO_CREATE_DESCRIPTION_DIRECTORY;
    }

    rnt = create_io_dir();
    if (rnt != CP_TOOLS_OK) {
        return CP_TOOLS_ERROR_GENBOCA_FAILURE_TO_CREATE_IO_DIRECTORY;
    }

    rnt = create_limits_dir(argc, argv);
    if (rnt != CP_TOOLS_OK) {
        cli::write(cli::fmt::error, std::to_string(rnt));
        return CP_TOOLS_ERROR_GENBOCA_FAILURE_TO_CREATE_IO_DIRECTORY;
    }

    rnt = create_compare_dir();
    if (rnt != CP_TOOLS_OK) {
        cli::write(cli::fmt::error, std::to_string(rnt));
        return CP_TOOLS_ERROR_GENBOCA_FAILURE_TO_MODIFY_CHECKER_FILE;
    }

    return CP_TOOLS_OK;
}

// API functions
int run(int argc, char *const argv[], std::ostream &, std::ostream &) {
    int option = -1;

    // This command has many options, but only the `help` option is dealt with here.
    while ((option = getopt_long(argc, argv, "ho:c:lg:b:tq:w:e:r:t:", longopts, NULL)) != -1) {
        switch (option) {
        case 'h':
            cli::write(cli::fmt::none, help());
            return 0;
        }
    }

    return genboca(argc, argv);
}

} // namespace cptools::commands::genboca