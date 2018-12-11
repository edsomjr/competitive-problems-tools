## Título do problema

Texto da descrição do problema. Para o modo de matemática, colocar a expressão entre dois símbolos 
de dólar, ou dois pares, em caso de uma equação centralizada. Por exemplo: 

Dada um orçamento de $M$ reais e uma lista de $N$
itens que podem ser adquiridos, determinar o número máximo de itens $Q$ que podem ser adquiridos
gastanto, no máximo, $M$ reais.

Se houvesse uma equação, a notação seria

$$
    \int_a^b \frac{1}{x} dx
$$

### Entrada

Descrição das entradas do problema. Cada linha deve ser completamente descrita e cada variável apresentada deve ser seguida de seu domínio. Por exemplo:

A primeira linha contém os valores dos inteiros $N$ e $M$ $(1\leq N\leq 100, 0.0\leq M\leq 20.0)$, separados por um espaço em branco, onde $M$ é dado em reais, com exatamente uma casa decimal.

A segunda linha contém $N$ valores $x_i$ $(0.0\leq x_i\leq 10.0)$, separados por um espaço em branco, representado o custo do $i$-ésimo item.

### Saída

Descrição das saídas do problema. Deve ser destacado quais espaços ou quebras de linha que fazem parte da formatação da saída. Por exemplo:

Imprima, em uma linha, o número máximo $Q$ de itens que podem ser adquiridos com $M$ reais. 

Em seguida imprima, em uma linha, $Q$ inteiros $q_i$, separados por um espaço em branco, indicando
o índice de um item a ser adquirido (os itens são numerados sequencialmente de 1 a $N$, na ordem
em que foram dados na entrada). Cada item pode ser adquirido uma única vez.

### Exemplos

Listagem com os exemplos de entrada que estarão disponíveis no problema. Um exemplo de sintaxe é (linhas como esta, não iniciadas com o símbolo `>>`,  são consideradas comentários nesta seção) dado abaixo, onde `input1`, `2` e `test3` são arquivos contidos no diretório `tests`:

>> input1
>> 2
>> test3

Caso seja necessário formatar o output de alguma das entradas anteriores (por conta de caracteres especiais, por exemplo. Na maioria dos casos o ideal é deixar a formatação automática), basta indicar o arquivo a ser usada usndo o operador seta `->`, conforme mostrado no exemplo abaixo:

>> 1 -> out1

### Notas

Comentários explicativos sobre os exemplos de entrada. Em geral cada comentário ocupa uma única linha, e todos os exemplos do caderno são comentados. Por exemplo,

No primeiro caso, acontece isso...

No segundo caso, acontece aquilo...

No terceiro caso, temos que...

O quarto caso é um exemplo de saída formatada e...


### Tutorial

Comentários sobre a solução do problema. Se possível incluir um pequeno trecho do código (ou
pseudo-código) da solução. Importante indicar a complexidade da solução proposta (tempo e memória).
