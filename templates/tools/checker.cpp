#include "testlib.h"

#include <vector>

using namespace std;

vector<double> xs;
int N;
double M;

// Lê e valida a solução do participante e do juiz. 
// Se a resposta está errada e stream = ouf (participante), então veredict deve ser _wa
// Se a resposta está errada e stream = ans (juiz), então veredict deve ser _fail
int readAns(InStream& stream, TResult veredict)
{
    auto Q = stream.readInt(0, 100, "Q must be in range [1, N]");
    auto total = 0.0;

    set<int> ys;

    for (int i = 0; i < Q; ++i)
    {
        int y = stream.readInt(1, N, "y must be in range [1, N]");

        if (ys.count(y))
            quitf(veredict, "The indexes can not have duplicates (%d)", y);

        ys.insert(y);

        total += xs[y];
    }
    
    if (total > M)
        quitf(veredict, "The sum of the chosen items (%.2f) is greater than M (%.2f)", total, M);

    return Q;
}

int main(int argc, char* argv[])
{
    registerTestlibCmd(argc, argv);

    // Leitura da entrada: não é necessário validar o input: o validator o fará
    N = inf.readInt(1, 100);
    M = inf.readDouble(0.0, 20.0);

    for (int i = 0; i < N; i++)
        xs.push_back(inf.readDouble(0.0, 10.0));

    int jans = readAns(ans, _fail);
    int pans = readAns(ouf, _wa);

    if (jans > pans)
        quitf(_wa, "jury has the better answer: jans = %d, pans = %d", jans, pans);
    else if (jans == pans)
        quitf(_ok, "answer = %d", pans);
    else // (jans < pans)
        quitf(_fail, ":( participant has the better answer: jans = %d, pans = %d", jans, pans);
}
