#include "testlib.h"
#include <vector>
#include <set>

using namespace std;

int main(int argc, char* argv[])
{
    registerValidation(argc, argv);

    // Exemplo de leitura de uma entrada gerada pelo gerador do exemplo
    inf.readInt(1, 1000000, "Primeiro valor: inteiro entre [1, 10^6]");
    inf.readEoln();

    inf.readStrictDouble(0.0, 10.0, 1, 1, " Segundo valor: ponto flutuante entre [0.0, 10.0] com no mínimo e, no máximo, uma casa decimal");
    inf.readEoln();

    auto token = inf.readToken("Abacaxi|Banana|Caju", "Lê um dos três tokens possível, diferenciando maiúsculas e minúsculas");
    inf.readEoln();

    /**
     * Validação das entradas comentadas do generator

    auto s = inf.readString("[A-Za-z0-9]{1,20}", "Lê uma string dê acordo com a regex dada e lê também o caractere de fim de linha");

    // Encadeamentos de chamadas de rnd.next() num mesmo cout tem comportamento indefinido
    // e deve ser evitado
    vector<int> xs = inf.readInts(2, 1, 2, "Lê dois inteiros separados por vírgula no intervalo [1,2]");
    inf.readEoln();

    inf.readInt(1, 1000000, "Primeiro valor: inteiro entre [1, 10^6]");
    inf.readEoln();

    vector<int> ns { 1, 2, 3, 4, 5 };

    vector<int> ys = inf.readInts(5, 1, 5, "Lê dois inteiros separados por vírgula no intervalo [1,5]");
    set<int> zs { ys.begin(), ys.end() };

    ensuref(zs.size() == 5u, "Garante que ys é uma permutação de 1 a 5");
    inf.readEoln();

    // Escolhe um dentre os elementos do contêiner (vector ou string funcionam)
    auto symbol = inf.readString("[+/-*]{1,1}", "Lê um dos símbolos listados");

    inf.readEoln();

    *
    **/

    inf.readEof();

    return 0;
}
