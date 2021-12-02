#ifndef CP_TOOLS_TABLE_H
#define CP_TOOLS_TABLE_H

#include <iostream>
#include <vector>

namespace cptools::table
{

    struct Column
    {
        std::string label;
        size_t size;
        long long format;
    };

    class Table
    {
        friend std::ostream &operator<<(std::ostream &os, const Table &table);

      public:
        Table(const std::vector<Column> &cols) : header(cols){};
        Table(std::vector<Column> &&cols) : header(cols){};

        void add_row(const std::vector<std::pair<std::string, long long> > &row);

      private:
        std::vector<Column> header;
        std::vector<std::vector<std::pair<std::string, long long> > > rows;
    };
} // namespace cptools::table

#endif
