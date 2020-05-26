#include <iostream>
#include <vector>
#include <map>

#include <sys/stat.h>

#include "sh.h"
#include "util.h"
#include "dirs.h"
#include "error.h"


namespace cptools::sh {

    long int last_modified(const std::string& filepath)
    {
        struct stat sb;

        if (lstat(filepath.c_str(), &sb) == -1)
            return 0;

        return sb.st_atime;
    }

    int copy_file(const std::string& dest, const std::string& src)
    {
        std::string command { "cp " + src + " " + dest };

        auto rc = std::system(command.c_str());

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_COPY_FILE;
    }


    int make_dir(const std::string& path)
    {
        std::string command { "mkdir -p " + path };

        auto rc = std::system(command.c_str());

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_CREATE_DIRECTORY;
    }

    int copy_dir(const std::string& dest, const std::string& src)
    {
        std::string command { "cp -r " + src + "/* " + dest };

        auto rc = std::system(command.c_str());

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_COPY_DIRECTORY;
    }

    int remove_dir(const std::string& path)
    {
        std::string command { "rm -rf " + path };

        auto rc = std::system(command.c_str());

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_REMOVE_DIRECTORY;
    }

    bool compare_dirs(const std::string& dirA, const std::string& dirB)
    {
        std::string command { "diff -r " + dirA + " " + dirB };

        auto rc = std::system(command.c_str());

        return rc == 0;
    }

    bool is_dir(const std::string& path)
    {
        std::string command { "test -d " + path };

        auto rc = std::system(command.c_str());

        return rc == 0;
    }

    bool is_file(const std::string& path)
    {
        std::string command { "test -f " + path };

        auto rc = std::system(command.c_str());

        return rc == 0;
    }


    int compile_cpp(const std::string& output, const std::string& src)
    {
        std::string command { "g++ -o " + output + " -O2 -std=c++17 -W -Wall " + src };

        auto rc = std::system(command.c_str());

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_CPP_COMPILATION_ERROR;
    }

    int build_tex(const std::string& output, const std::string& src)
    {
        std::string outdir { "." };

        if (output.find('/') != std::string::npos)
        {
            auto tokens = split(output, '/');
            outdir = tokens.front();
        }

        std::string command { std::string("export TEXINPUTS=\".:") + CP_TOOLS_CLASSES_DIR 
            + ":\" && pdflatex -output-directory=" + outdir + " " + src };

        auto rc = std::system(command.c_str());

        // Roda duas vezes para garantir que estilos que tenham referências sejam
        // renderizados corretamente
        if (rc == 0)
            rc = std::system(command.c_str());

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_PDFLATEX_ERROR;
 
    }

    std::map<std::string, int (*)(const std::string&, const std::string&)> fs {
        { "cpp", compile_cpp },
        { "tex", build_tex },
    };

    int build(const std::string& output, const std::string& src)
    {
        auto tokens = split(src, '.');
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

    int process(const std::string& input, const std::string& program, const std::string& output,
        int timeout)
    {
        std::string command { "timeout " + std::to_string(timeout) + "s " + program + " < " 
            + input + " > " + output };

        auto rc = std::system(command.c_str());

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_PROCESS_ERROR;
    }

    int exec(const std::string& program, const std::string& args, const std::string& output,
        int timeout)
    {
        std::string command { "timeout " + std::to_string(timeout) + "s " + program + "  " 
            + args + " > " + output };

        auto rc = std::system(command.c_str());

        return rc == 0 ? CP_TOOLS_OK : CP_TOOLS_ERROR_SH_EXEC_ERROR;
    }
}
