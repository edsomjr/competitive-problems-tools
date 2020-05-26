#include "testlib.h"

// Lê e valida a solução 
int readAns(int x, int y, InStream& in)
{
    in.readInt(x, x, "x");
    in.readSpace();

    in.readInt(y, y, "y");
    in.readSpace();

    auto ans = in.readInt(0, 2000, "ans");
    in.readEoln();

    if (ans != x + y)
        in.quitf("%d != %d", ans, x + y);

    return ans;
}

int main(int argc, char* argv[])
{
    registerTestlibCmd(argc, argv);

    // Leitura da entrada: não é necessário validar o input: o validator o fará
    auto x = inf.readInt();
    auto y = inf.readInt();

    readAns(x, y, ans);
    int pans = readAns(x, y, ouf);

    quitf(_ok, "answer = %d", pans);
    /**
     * Modelo para respostas de otimização, onde a resposta do participante pode ser
     * melhor do que a do juiz
    int jans = readAns(ans, _fail);
    int pans = readAns(ouf, _wa);

    if (jans > pans)
        quitf(_wa, "jury has the better answer: jans = %d, pans = %d", jans, pans);
    else if (jans == pans)
        quitf(_ok, "answer = %d", pans);
    else // (jans < pans)
        quitf(_fail, ":( participant has the better answer: jans = %d, pans = %d", jans, pans);
    **/

}
