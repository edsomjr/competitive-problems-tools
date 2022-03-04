#include <vector>
#include <algorithm>

#include "utils.h"


namespace cptools::utils {

    size_t levenshtein_distance(const std::string& s, const std::string& t) {
        size_t n = s.length() + 1;
        size_t m = t.length() + 1;

        std::vector<size_t> d(m*n, 0);

        for(size_t i=1, im=0; i<m; ++i, ++im)
        {
            for(size_t j=1, jn=0; j<n; ++j, ++jn)
            {
                if(s[jn] == t[im])
                {
                    d[ (i*n)+j ] = d[((i-1)*n) + (j-1)];
                }
                else
                {
                    d[(i * n) + j] = std::min( { d[(i - 1) * n + j] + 1, // A deletion
                                                d[i * n + (j - 1)] + 1, // An insertion
                                                d[(i - 1) * n + (j - 1)] + 1} ); // A substitution
                }
            }
        }

        size_t result = d[n * m - 1];

        return result;
    }

    std::vector<std::string>
    split(const std::string &s, const std::string &delimiter) {
        size_t pos_start = 0;
        size_t pos_end;
        size_t delim_len = delimiter.length();

        std::string token;
        std::vector<std::string> res;

        while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
        {
            token = s.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back(token);
        }

        res.push_back (s.substr(pos_start));

        return res;
    }
}
