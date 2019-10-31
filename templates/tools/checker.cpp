#include "testlib.h"

// Lê e valida a solução do participante e do juiz. 
// Se a resposta está errada e stream = ouf (participante), então veredict deve ser _wa
// Se a resposta está errada e stream = ans (juiz), então veredict deve ser _fail
int readAns(int x, int y, InStream& stream, TResult veredict)
{
    stream.readInt(x, x, "x");
    stream.readSpace();

    stream.readInt(y, y, "y");
    stream.readSpace();

    auto ans = stream.readInt(0, 2000, "ans");
    stream.readEoln();

    if (ans != x + y)
        quitf(veredict, "%d != %d", ans, x + y);

    return ans;
}

int main(int argc, char* argv[])
{
    registerTestlibCmd(argc, argv);

    // Leitura da entrada: não é necessário validar o input: o validator o fará
    auto x = inf.readInt();
    auto y = inf.readInt();

    readAns(x, y, ans, _fail);
    int pans = readAns(x, y, ouf, _wa);

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
