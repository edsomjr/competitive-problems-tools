#include "table.h"
#include "cli/format.h"

#include <iostream>

namespace cptools::table
{

    std::ostream &
    operator<<(std::ostream &os, const Table &t)
    {
        int total = 0, N = t.header.size();

        for (auto col : t.header)
            total += (3 + col.size);

        std::string hline(total + 1, '-');

        os << hline << '\n';

        for (auto h : t.header)
            {
                os << "| " << cli::format::apply(h.label, h.format, h.size) << " ";
            }

        os << "|\n" << hline << '\n';

        for (auto row : t.rows)
            {
                int M = row.size();

                for (int i = 0; i < N; ++i)
                    {
                        auto data = i < M ? row[i].first : "";
                        auto spec = i < M ? row[i].second : 0;

                        os << "| " << cli::format::apply(data, spec, t.header[i].size) << " ";
                    }

                os << "|\n" << hline << '\n';
            }

        return os;
    }

    void
    Table::add_row(const std::vector<std::pair<std::string, long long> > &row)
    {
        rows.push_back(row);
    }

} // namespace cptools::table
