#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#include <sys/stat.h>

#include "sh.h"
#include "util.h"
#include "dirs.h"
#include "error.h"

using std::ostringstream;
using std::to_string;
using std::ostream;
using std::vector;
using std::map;

namespace cptools::sh {

    static int execute_command(const string& command, ostream& out)
    {
        auto fp = popen(command.c_str(), "r");

        if (fp == NULL)
            return CP_TOOLS_ERROR_SH_POPEN_FAILED;

        char buffer[4096];

        while (fread(buffer, sizeof(char), 4096, fp) > 0)
        {
            out << buffer;
        }

        return pclose(fp);
    }

    int make_dir(const string& path)
    {
        string command { "mkdir -p " + path };
        ostringstream out;

        auto rc = execute_command(command, out); 

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_CREATE_DIRECTORY;
    }

    int copy_dir(const string& dest, const string& src)
    {
        string command { "cp -r -n " + src + "/* 2>&1 " + dest };
        ostringstream out;

        auto rc = execute_command(command, out);

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_COPY_DIRECTORY;
    }

    long int last_modified(const string& filepath)
    {
        struct stat sb;

        if (lstat(filepath.c_str(), &sb) == -1)
            return 0;

        return sb.st_atime;
    }

    int copy_file(const string& dest, const string& src)
    {
        string command { "cp " + src + " " + dest };

        auto rc = system(command.c_str());

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_COPY_FILE;
    }

    int remove_file(const string& path)
    {
        string command { "rm -f " + path };

        auto rc = system(command.c_str());

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_REMOVE_FILE;

    }


    int remove_dir(const string& path)
    {
        string command { "rm -rf " + path };

        auto rc = system(command.c_str());

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_REMOVE_DIRECTORY;
    }

    bool compare_dirs(const string& dirA, const string& dirB)
    {
        string command { "diff -r " + dirA + " " + dirB };

        auto rc = system(command.c_str());

        return rc == 0;
    }

    bool is_dir(const string& path)
    {
        string command { "test -d " + path };

        auto rc = system(command.c_str());

        return rc == 0;
    }

    bool is_file(const string& path)
    {
        string command { "test -f " + path };

        auto rc = system(command.c_str());

        return rc == 0;
    }


    int compile_cpp(const string& output, const string& src)
    {
        string command { "g++ -o " + output + " -O2 -std=c++17 -W -Wall " + src };

        auto rc = system(command.c_str());

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_CPP_COMPILATION_ERROR;
    }

    int build_py(const string& output, const string& src)
    {
        vector<string> commands {
            "echo '#!/usr/bin/python' > " + output,
            "cat " + src + " >> " + output,
            "chmod 755 " + output,
         };

        for (auto command : commands)
        {
            auto rc = system(command.c_str());

            if (rc != CP_TOOLS_OK)
                return CP_TOOLS_ERROR_SH_PY_BUILD_ERROR;
        }

        return CP_TOOLS_OK;
    }


    int build_tex(const string& output, const string& src)
    {
        string outdir { "." };

        if (output.find('/') != string::npos)
        {
            auto tokens = util::split(output, '/');
            outdir = tokens.front();
        }

        string command { string("export TEXINPUTS=\".:") + CP_TOOLS_CLASSES_DIR 
            + ":\" && pdflatex -output-directory=" + outdir + " " + src };

        auto rc = system(command.c_str());

        // Roda duas vezes para garantir que estilos que tenham referências sejam
        // renderizados corretamente
        if (rc == 0)
            rc = system(command.c_str());

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_PDFLATEX_ERROR;
 
    }

    map<string, int (*)(const string&, const string&)> fs {
        { "cpp", compile_cpp },
        { "tex", build_tex },
        { "py", build_py },
    };

    int build(const string& output, const string& src)
    {
        auto tokens = util::split(src, '.');
        auto ext = tokens.back();
        auto it = fs.find(ext);

        auto x = last_modified(src);
        auto y = last_modified(output);
    
        if (x <= y)
        {
            return CP_TOOLS_OK;
        }

        if (it == fs.end())
            return CP_TOOLS_ERROR_SH_BUILD_EXT_NOT_FOUND;

        return it->second(output, src); 
    }

    int process(const string& input, const string& program, const string& output,
        int timeout)
    {
        string command { "timeout " + to_string(timeout) + "s " + program + " < " 
            + input + " > " + output };

        auto rc = system(command.c_str());

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_PROCESS_ERROR;
    }

    int exec(const string& program, const string& args, const string& output,
        int timeout)
    {
        string command;

        if (timeout > 0)
        {
            command = "timeout " + to_string(timeout) + "s " + program + "  " 
                + args + " > " + output;
        } else
        {
            command = program + "  " + args + " > " + output;;
        }

        int rc = system(command.c_str());

        return WEXITSTATUS(rc);
    }
}
