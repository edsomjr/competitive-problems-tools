#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include <sys/stat.h>

#include "cli/cli.h"
#include "dirs.h"
#include "error.h"
#include "fs.h"
#include "sh.h"
#include "util.h"

using timer = std::chrono::high_resolution_clock;

namespace cptools::sh
{

    static double
    parse_time_output(const std::string &out)
    {
        std::ifstream in(out);
        std::string line;

        while (getline(in, line), line.find("Maximum resident set size") == std::string::npos)
            ;

        long long kbs = 0;

        for (auto c : line)
            {
                if (isdigit(c))
                    {
                        kbs *= 10;
                        kbs += (c - '0');
                    }
            }

        double mbs = kbs / 1024.0;

        return mbs;
    }

    const Result
    execute_command(const std::string &command, int on_error)
    {
        auto fp = popen(command.c_str(), "r");

        if (fp == NULL)
            return make_result(false, CP_TOOLS_ERROR_SH_POPEN_FAILED);

        std::ostringstream oss;
        char buffer[1024 * 1024];
        int amount;

        while ((amount = fread(buffer, sizeof(char), 1024 * 1024, fp)) > 0)
            {
                oss << std::string(buffer, amount);
            }

        auto status = pclose(fp);
        cli::Trace(oss.str());

        if (status == -1)
        {
            cli::Error("Falha na execução do comando: " + command);
            cli::Trace(oss.str());

            return make_result(false, on_error, "", oss.str());
        }

        return make_result(true, status, oss.str());
    }

    long int
    last_modified(const std::string &filepath)
    {
        struct stat sb;

        if (lstat(filepath.c_str(), &sb) == -1)
            return 0;

        return sb.st_atime;
    }

    const Result
    diff_dirs(const std::string &dirA, const std::string &dirB)
    {
        std::string command{ "diff -r " + dirA + " " + dirB + " 2>&1" };
        return execute_command(command);
    }

    const Result
    compile_cpp(const std::string &output, const std::string &src)
    {
        //std::string command{ "g++ -o " + output + " -O2 -std=c++17 -W -Wall " + src + " 2>&1" };
        std::string command{ "g++ -o " + output + " -O2 -std=c++17 -W -Wall " + src };

        return execute_command(command, CP_TOOLS_ERROR_SH_CPP_COMPILATION_ERROR);
    }

    const Result
    build_py(const std::string &output, const std::string &src)
    {
        std::vector<std::string> commands{
            "echo '#!/usr/bin/python3' > " + output,
            "cat " + src + " >> " + output,
            "chmod 755 " + output,
        };

        for (auto command : commands)
            {
                auto result = execute_command(command, CP_TOOLS_ERROR_SH_PY_BUILD_ERROR);

                if (!result.ok)
                    return result;
            }

        return make_result(true, CP_TOOLS_OK);
    }

    const Result
    compile_java(const std::string &output, const std::string &src)
    {
        auto dir = util::split(src, '/').front();
        auto filename = util::split(src, '.').front();
        auto name = util::split(filename, '/').back();

        std::vector<std::string> commands{
            "javac " + src,
            "echo '#!/bin/bash' > " + output,
            "echo 'java -cp " + dir + "/ " + name + "' > " + output,
            "chmod 755 " + output,
        };

        for (auto command : commands)
            {
                std::string error;
                auto res = execute_command(command);

                if (!res.ok)
                    return res;
            }

        return make_result(true, CP_TOOLS_OK);
    }

    const Result
    build_tex(const std::string &output, const std::string &src)
    {
        std::string outdir{ "." };

        if (output.find('/') != std::string::npos)
            {
                auto tokens = util::split(output, '/');
                outdir = tokens.front();
            }

        std::string command{ std::string("export TEXINPUTS=\".:") + CP_TOOLS_CLASSES_DIR
                             + ":\" && pdflatex -halt-on-error -output-directory=" + outdir + " "
                             + src };
cli::Trace(command);
        auto res = execute_command(command);

        if (!res.ok)
            res = execute_command(command);

        return res;
    }

    std::map<std::string, const Result (*)(const std::string &, const std::string &)> fs{
        { "cpp", compile_cpp },
        { "java", compile_java },
        { "tex", build_tex },
        { "py", build_py },
    };

    const Result
    build(const std::string &output, const std::string &src)
    {
        auto tokens = util::split(src, '.');
        auto ext = tokens.back();
        auto it = fs.find(ext);

        auto x = last_modified(src);
        auto y = last_modified(output);

        if (x <= y)
            {
                return make_result(true);
            }

        if (it == fs.end())
            return make_result(false, CP_TOOLS_ERROR_SH_BUILD_EXT_NOT_FOUND, "",
                               "Extension not found");

        return it->second(output, src);
    }

    const Result
    execute_program(const std::string &program, const std::string &args, const std::string &infile,
                    const std::string &outfile, int timeout)
    {
        std::string command{ program + " " + args };

        if (not infile.empty())
            command += " < " + infile;

        if (not outfile.empty())
            command += " > " + outfile;

        command += " 2>&1";

        if (timeout > 0)
            command = " timeout " + std::to_string(timeout) + "s " + command;
        auto result = execute_command(command);
        result.rc = WEXITSTATUS(result.rc);
        return result;
    }

    Info
    profile(const std::string &program, const std::string &args, int timeout,
            const std::string &infile, const std::string &outfile)
    {
        Info info;

        // Prepares the file which will have the output of the command /usr/bin/time
        auto res = fs::create_directory(CP_TOOLS_TEMP_DIR);
        if (not res.ok)
            {
                info.rc = res.rc;
                return info;
            }

        std::string out{ std::string(CP_TOOLS_TEMP_DIR) + "/.time_output" };

        // Prepares the command to the terminal
        std::string command{ "/usr/bin/time -v -o " + out };

        if (timeout > 0)
            command += " timeout " + std::to_string(timeout) + "s ";

        command += " " + program + "  " + args;

        if (not infile.empty())
            command += " < " + infile;

        if (not outfile.empty())
            command += " > " + outfile;

        // Executes the command
        auto start = timer::now();

        auto fp = popen(command.c_str(), "r");

        auto rc = pclose(fp);

        auto end = timer::now();

        if (fp == NULL)
            {
                info.rc = CP_TOOLS_ERROR_SH_POPEN_FAILED;
                return info;
            }

        // Prepares to the return
        auto t = std::chrono::duration_cast<std::chrono::duration<double> >(end - start);

        info.rc = rc;
        info.elapsed = t.count();
        info.memory = parse_time_output(out);

        return info;
    }
} // namespace cptools::sh
