#include "message.h"

#include <map>
#include <sstream>
#include <iostream>

namespace cptools::message
{
    enum ColorAndStyle { BLACK, BLUE, MAGENTA, CYAN, WHITE, VIOLET, BEIGE, UNDERLINE }; 

    // Fim da formatação
    constexpr char END[]    { "\033[0m" };

    // Cores
    constexpr char RED[]    { "\033[91m" };
    constexpr char GRAY[]   { "\033[90m" };
    constexpr char GREEN[]  { "\033[92m" };
    constexpr char YELLOW[] { "\033[93m" };

    // Estilos
    constexpr char BOLD[]   { "\033[1m" };
    constexpr char NORMAL[] { "\033[2m" };
    constexpr char ITALIC[] { "\033[3m" };

    static std::map<ColorAndStyle, std::string> code {
        { BLACK, "\033[30m" },
        { BLUE, "\033[94m" },
        { MAGENTA, "\033[95m" },
        { CYAN, "\033[96m" },
        { WHITE, "\033[97m" },
        { VIOLET, "\033[35m" },
        { BEIGE, "\033[36m" },
        { UNDERLINE, "\033[4m" },
    };

    std::string ok(const std::string& text)
    {
        std::ostringstream oss;

        oss << BOLD << GREEN << "Ok!";

        if (not text.empty())
            oss << NORMAL << ' ' << text;

        oss << END;

        return oss.str();
    }

    std::string info(const std::string& text)
    {
        std::ostringstream oss;

        oss << GRAY << text << END;

        return oss.str();
    }

    std::string warning(const std::string& text)
    {
        std::ostringstream oss;

        oss << BOLD << YELLOW << "Warning!";

        if (not text.empty())
            oss << NORMAL << ITALIC << ' ' << text;

        oss << END;

        return oss.str();
    }

    std::string error(const std::string& text)
    {
        std::ostringstream oss;

        oss << BOLD << RED << "Error!";

        if (not text.empty())
            oss << NORMAL << ' ' << text;

        oss << END;

        return oss.str();
    }
} 
