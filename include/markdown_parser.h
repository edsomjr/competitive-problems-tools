#ifndef MARKDOWN_PARSER_H
#define MARKDOWN_PARSER_H

#include <string_view>

class Block;

class Document {
public:
    static Document from_file(std::string_view filepath);

private:
    Document() {}

    vector<Block *> _blocks;
};

#endif
