# Tópico 5

Este material é adaptado de [CS50 - Lecture 5](https://cs50.harvard.edu/x/2021/notes/5/)

- [Redimensionar Arrays](./README.md#redimensionar-arrays)
- [Estruturas de dados](./README.md#estruturas-de-dados)
- [Listas Ligadas](./README.md#listas-ligadas)
- [Implementando Arrays](./README.md#implementando-arrays)
- [Implementando Listas Ligadas](./README.md#implementando-listas-ligadas)
- [Árvores](./README.md#arvores)
- [Mais Estruturas de Dados](./README.md#mais-estruturas-de-dados)

## Redimensionar Arrays
- Já aprendemos sobre a função `malloc` e o processo de alocar (reservar) espaço em memória.
- No tópico 2 aprendemos sobre arrays (vetores/matrizes) onde podemos armazenar valores de um mesmo tipo em uma espécie de lista contígua na memória. No entanto, se precisarmos adicionar mais elementos, será necessário aumentar o tamanho do vetor. Porém, pode acontecer de já existirem dados armazenados após o nosso vetor.
![Memória já ocupada](https://cs50.harvard.edu/x/2021/notes/5/array_of_length_3.png)
- Uma solução seria alocar espaço para um vetor maior (em alguma posição disponível na memória) e então copiar os dados do nosso vetor atual. Entretanto, se o nosso vetor for grande, isso pode tomar muito tempo.
- ![Aumentando espaço alocado](https://cs50.harvard.edu/x/2021/notes/5/array_of_length_4.png)

## Estruturas de Dados
- **Estruturas de dados** nos fornecem modos mais complexos de armazenar dados na memória, permitindo-nos armazenar dados em diferentes formas de organização (_layout_).
- Para construir uma estrutura de dados, precisaremos das seguintes ferramentas:
  -  `struct`, para construir tipos de dados personalizados.
  -  `.`, para acessar atributos em uma estrutura.
  -  `*`, para criar variáveis do tipo ponteiro, bem como acessar os dados apontados por um ponteiro.
  -  `->`, para acessar atributos de uma estrutura apontada por um ponteiro.

## Listas Ligadas
- Com uma **lista ligada**, podemos armazenar uma lista de valores que pode crescer facilmente armazenando dados em diferentes partes da memória, sem a necessidade de reservar espaço contíguo.
![Dados esparsos](https://cs50.harvard.edu/x/2021/notes/5/linked_list.png)
  - Nós temos os valores `1`, `2` e `3`, cada um em uma posição de memória como `0123`, `0x456` e `0x789`.
  - Note que esta não é a estrutura de um vetor, pois os dados não estão um ao lado do outro. Porém, sempre podemos alocar espaço para um elemento, desde que exista espaço suficiente na memória. Alocar espaço para um vetor pode ser mais complicado pela falta de espaço contíguo.
- Para manter um registro (_track_) dos valores de modo que possamos acessar todos eles, podemos construir uma estrutura que faça cada elemento apontar para o próximo.
![Ponteiro para o próximo](https://cs50.harvard.edu/x/2021/notes/5/linked_list_with_addresses.png)
  - Perceba que junto ao valor `1`, temos um ponteiro apontando para `0x456` onde está armazenado o número `2` e junto ao número `2` temos um ponteiro apontando para `0x789` onde se encontra armazenado o número `3`. Essa construção, `valor`+`ponteiro` é, muitas vezes, chamada de **nó**. Em C, isto pode ser implementado usando uma `struct`.
  - Para o último nó, quando não temos um próximo elemento, o ponteiro deve apontar para um endereço **nulo** a fim de indicar o final da lista.
- Observações:
  1. Primeiro, precisamos de mais espaço para cada elemento. Um espaço para o valor e outro para o ponteiro.
  2. Com esta construção, não temos mais como usar uma busca binária, pois não temos acesso ao endereço do "elemento do meio" da nossa lista.
  3. Portanto, só podemos realizar busca linear seguindo um pontiro por vez. 
- Em C, podemos criar nossa própria estrutura chamada `no_t`(ou `node_t`) para armazenar um `int` e um ponteiro para o próximo `node`.
```c
typedef struct node
{
    int valor;
    struct node *prox;
}node_t;
```
  - Temos um campo `valor` para armazenar o valor e outro campo `prox` que armazena um ponteiro para o próximo `no` da lista.
  - Note que o campo `prox` precisou ser definido como `struct node*` em vez de simplesmente `node_t*`. Isto ocorre porque o operador `typedef` só passa a fazer sentido depois de toda a `struct` ter sido definida. No ponto onde estamos adicionando o atributo `prox` ainda estamos no meio da definição da `struct`.
  - Note ainda que na definição de `prox` poderia ter usado `struct node_t` para manter uma padronização, mas a `struct` deveria ser alterada em seu início para `typedef struct node_t`.
  - Observe o prefixo `_t` na definição da nossa estrutura. Ele não é requerido, mas é uma **boa prática de programação** adicioná-lo para indicar que `node_t` é um tipo, `_t`, definido pelo programador.
- Podemos, então, criar um elemento chamado `primeiro` que aponta para o primeiro elemento da nossa lista pondo `node_t *primeiro = NULL;`
- Para adicionar um elemento `n`, precisamos alocar espaço em memória para este elemento, configurar o campo `prox` de `n` para `NULL` e fazer o `primeiro` apontar para `n`. O código abaixo nos mostra como fazer:
```c
node_t *n = malloc(sizeof(node_t));
if (n != NULL)
{
  n->valor = 1;
  n->prox = NULL;
  primeiro = n;
}
```
- Nossa lista agora será como apresentada abaixo:

![Lista com 1 elemento](https://cs50.harvard.edu/x/2021/notes/5/list_with_one_node.png)

- Para adicionar mais um elemento na lista, devemos proceder como abaixo:
```c
n = malloc(sizeof(node_t));
if (n != NULL)
{
  n->valor = 2;
  n->prox = NULL;
  primeiro->prox = n;
}
```
- Note que usamos a mesma variável `n` para alocar o novo `node_t`, uma vez que o `node_t` atual já possui um ponteiro, `primeiro`, apontando para ele.
  - Você deve tomar bastante cuidado com este tipo de operação, pois se você sobrescrever algum ponteiro, toda a lista a partir deste nó será perdida e ainda continuará ocupando espaço na memória.
  - Pior que isto, elementos alocados (reservados) sem um ponteiro apontando para eles podem, potencialmente, permanecer na memória após o fim do programa, "vazando" dados para que aplicativos maliciosos façam uso indevido dos dados.
- Para adicionar um terceiro elemento, podemos fazer como abaixo:
```c
n = malloc(sizeof(node_t));
if (n != NULL)
{
  n->valor = 3;
  n->prox = NULL;
  primeiro->prox->prox = n;
}

```
- Neste momento, a memória se parecerá com a imagem abaixo:
![Lista com 1 elemento](https://cs50.harvard.edu/x/2021/notes/5/list_with_three_nodes.png)
- 
