#ifndef CP_TOOLS_TABLE_H
#define CP_TOOLS_TABLE_H

#include <iostream>
#include <vector>

using std::pair;
using std::ostream;
using std::string;
using std::vector;

namespace cptools::table {

    struct Column {
        string label;
        size_t size;
        long long format;
    };

    class Table {
        friend ostream& operator<<(ostream& os, const Table& table);

    public:
        Table(const vector<Column>& cols) : header(cols) { };
        Table(vector<Column>&& cols) : header(cols) { };

        void add_row(const vector<pair<string, long long>>& row);
        
    private:
        vector<Column> header;
        vector<vector<pair<string,long long>>> rows;
    };
}

#endif
