#include <iostream>
#include "pmh_parser.h"

#include <locale>
#include <codecvt>
#include <string>


int main()
{
    std::string markdown, line;

    while (getline(std::cin, line))
        markdown += line + "\n";

    char buffer[16000];
    sprintf(buffer, "%s", markdown.c_str());


    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide = converter.from_bytes(markdown);

    std::cout << "narrow = [" << markdown.size() << "]\n";
    std::cout << "wide = [" << wide.size() << "]\n";

    pmh_element **results;
    pmh_markdown_to_elements(buffer, pmh_EXT_NOTES, &results);

    for (int i = 0; i <= pmh_ALL; i++) {
        switch (i) {
            case pmh_EMPH: std::cout << "Italico\n"; break;
            case pmh_STRONG: std::cout << "Bold\n"; break;
            case pmh_H1: std::cout << "Header\n"; break;
            default: break;
        }

        pmh_element *element_cursor = results[i];

        while (element_cursor != NULL)
        {
            auto a = element_cursor->pos, b = element_cursor->end;

            std::cout << "Type: " << pmh_element_name_from_type(element_cursor->type) << '\n';
            std::cout << "Elemento: [" << a << ", " << b << "]\n";
            std::string text = converter.to_bytes(wide.substr(a, b - a));
            std::cout << "Text = [" << text << "]\n";

            element_cursor = element_cursor->next;
        }
    }

    pmh_free_elements(results);

    return 0;
}
