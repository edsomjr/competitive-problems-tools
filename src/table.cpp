#include "table.h"
#include "format.h"

#include <iostream>

using namespace std;

namespace cptools::table {

    ostream& operator<<(ostream& os, const Table& t)
    {
        int total = 0, N = t.header.size();

        for (auto col : t.header)
            total += (3 + col.size);

        string hline(total + 1, '-');

        os << hline << '\n';

        for (auto h : t.header)
        {
            os << "| ";

//            os << format::formatted(h.label, h.size, h.pos, h.format) << " ";
            os << format::apply(h.label, h.format, h.size) << " ";
        }

        os << "|\n" << hline << '\n';

        for (auto row : t.rows)
        {
            int M = row.size();

            for (int i = 0; i < N; ++i) 
            {
                auto data = i < M ? row[i].first : "";
                auto spec = i < M ? row[i].second : 0;

                os << "| ";
                //os << format::formatted(data, t.header[i].size, t.header[i].pos, { spec }) << " ";
                os << format::apply(data, spec, t.header[i].size) << " ";
            }

            os << "|\n" << hline << '\n';
        }

        return os;
    }

    void Table::add_row(const vector<pair<string, long long>>& row)
    {
        rows.push_back(row);
    }

}
