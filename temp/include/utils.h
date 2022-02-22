#ifndef CP_TOOLS_UTILS_H
#define CP_TOOLS_UTILS_H

#include <vector>
#include <algorithm>

size_t levenshtein_distance(const std::string& s, const std::string& t)
{
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

#endif
