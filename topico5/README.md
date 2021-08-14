# Tópico 5

Este material é adaptado de [CS50 - Lecture 5](https://cs50.harvard.edu/x/2021/notes/5/)

- [Redimensionar Arrays](./README.md#redimensionar-arrays)
- [Estruturas de dados](./README.md#estruturas-de-dados)
- [Listas Ligadas](./README.md#listas-ligadas)
- [Simulando Orientação a objetos](./README.md#simulando-orientação-a-objetos)
- [Implementando Arrays](./README.md#implementando-arrays)
- [Implementando Listas Ligadas](./README.md#implementando-listas-ligadas)
- [Árvores](./README.md#arvores)
- [Mais Estruturas de Dados](./README.md#mais-estruturas-de-dados)

## Redimensionar Arrays
- Já aprendemos sobre a função `malloc` e o processo de alocar (reservar) espaço em memória.
- No tópico 2 aprendemos sobre arrays (vetores/matrizes) onde podemos armazenar valores de um mesmo tipo em uma espécie de lista contígua na memória. No entanto, se precisarmos adicionar mais elementos, será necessário aumentar o tamanho do vetor. Porém, pode acontecer de já existirem dados armazenados após o nosso vetor.

![Memória já ocupada](https://cs50.harvard.edu/x/2021/notes/5/array_of_length_3.png)

- Uma solução seria alocar espaço para um vetor maior (em alguma posição disponível na memória) e então copiar os dados do nosso vetor atual. Entretanto, se o nosso vetor for grande, isso pode tomar muito tempo.

![Aumentando espaço alocado](https://cs50.harvard.edu/x/2021/notes/5/array_of_length_4.png)

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

- Como você faria para:
  - generalizar a adição de novos nós na lista?
  - remover um nó da lista?
- No pior caso em um processo de busca, ou seja, quando o número que estamos procurando é o último da lista. Quantos elementos da lista devemos consultar, a partir do primeiro?


## Simulando Orientação a objetos

Nesta seção, faremos uma breve digressão sobre o conteúdo da disciplina para discutirmos um pouco sobre paradigmas de programação, em particular sobre o paradigma orientado a objetos.

O objetivo não é formalizar o que são, quais são e como diferem os paradigmas, em vez disso, apenas faremos um resumo bem grosseiro de orientação a objetos.

UM **paradigma de programação** é um "estilo" de programação que leva em conta as funcionalidades da linguagem usada. Por exemplo, `C` utiliza o paradigma procedural enquanto `Java` adota orientação a objetos e `Haskell` utiliza programação funcional.
- No paradigma **procedural**, tudo o que temos a disposição são os procedimentos (instruções). Isto é, quando escrevemos algo como `a = b + c;` estamos instruindo o computador a obter os valores de `b` e `c`, onde quer que eles estejam (memória, registradores ou _cache_  no processador etc), somar tais valores e armazenar o resultado em `a`, novamente, onde quer que a variável `a` esteja armazenada no momento.
- No paradigma **funcional**, tudo é feito por meio de funções 😮🤦🏽‍♂️ (Não diga!). Isto significa que não existem palavras reservadas para realizar laços. Também não é permitido alterar o valor de variáveis fora do escopo atual.
- Por fim, no paradigma **orientado a objetos** como em `Java` e `C++`, existe a noção de objeto. Grosso modo, um objeto é uma `struct` que contém atributos e funções. Na verdade, tais "structs" são chamadas de **classes** e as funções são denominadas de **métodos**.
  - Um exemplo básico de uma classe em `Java` é mostrado abaixo:
```Java
class Aluno{
  private String nome;
  private String endereco;

  void setNome(String n){
    nome = n
  }

  String getNome(){
    return nome;
  }

}
```  
- Uma classe funciona como um modelo (um novo tipo de dados, igual as `structs` de `C`). Para se definir uma variável do novo tipo, precisamos declarar e **instanciar** tal variável.
- O processo é semelhante a declarar uma variável ponteiro e em seguida alocar espaço em memória usando a função `malloc` ou `calloc`.
  - A diferença é que não precisamos tratar diretamente com ponteiros (variáveis criadas pelos tipos definidos pelo usuário, isto é, classes de usuários, são essencialmente ponteiros) nem chamar uma função como `malloc`. Em `Java`, isto é feito de forma "automática" como abaixo:
```Java
  Aluno a1 = new Aluno();
```
- Esta linha de código define a variável, `Aluno a1`, aloca ("`malloc`") espaço para ela e
- já faz a variável apontar para o endereço do elemento alocado.
  - Tudo isto é feito com `new Aluno();` sem que precisemos nos preocupar com os detalhes da alocação dinâmica.
- O preenchimento dos campos de um `Aluno` deve ser feito por meio dos métodos (funções) esistentes na classe. Isto está exemplificado abaixo:
```Java
  a1.setNome("Fulano de Tal");
```
- Na prática, removendo a palavra `private` da definição dos campos, isto poderia ser feito como abaixo
```Java
  a1.nome = "Fulano de Tal";
```
  - Entretanto, uma das ideias da programação orientada a objetos é que nenhuma classe (partes dos programas em POO) deveria ser capaz de mudar valores de outras classes. Mudança nos valores de atributos de uma classe `Aluno` so deveria ser realizada dentro da própria classe `Aluno`. Assim, uma classe externa "solicitaria" tal mudança por meio da chamada do método `setNome()` como no exemplo `a1.setNome("Fulano de Tal");` acima.

- Existem também o paradigmas de programação lógica, adotado pela linguagem `Prolog`. Vocês aprenderão mais sobre o assunto na disciplina de `Linguagens de programação`.

### Programação quase orientada a objetos em C
- Em linguagem `C`, uma `struct` só é permitida ter atributos, mas não funções.
- Entretanto, uma `struct` pode ter um ponteiro para função como atributo.
- O uso de ponteiros para função como um atributo de uma `struct` permite simular algumas propriedades de linguagens orientadas a objeto. 
  - Deste modo, podemos chamar uma função de modo bem parecido com o que ocorre em POO.
- O exemplo abaixo ilustra um trecho de código que utiliza essas ideias.
```c
// aluno.h
typedef struct aluno_t
{
  char nome[100];
  char endereco[100];

  void (*setNome)(struct aluno_t *a, char *nome);
  char *(*getNome)(struct aluno_t *a);
} aluno_t;

aluno_t *new_aluno();
void aluno_setNome(aluno_t *a, char *nome);
char *aluno_getNome(aluno_t *a);
```
- Definimos uma `struct` com dois campos, `nome` e `endereco`, para dados do aluno e mais dois campos que são ponteiros para função.
  - Um ponteiro para função é definido de maneira muito semelhante à definição de uma função comum, exceto pelo fato de que o nome da função (ponteiro) é precedida por um asterísco `*`, para indicar que a função é, ela própria, um ponteiro, e englobado por parênteses (incluindo o asterísco).
```c
// aluno.c
#include "aluno.h"
#include <string.h>
#include <stdlib.h>

// Inicializar as funções (métodos) do aluno. Poderíamos inicializar
// os dados do aluno também.
aluno_t *new_aluno()
{
  aluno_t *tmp = malloc(sizeof(aluno_t));
  tmp->setNome = aluno_setNome;
  tmp->getNome = aluno_getNome;
  return tmp;
}

void aluno_setNome(aluno_t *a, char *nome)
{
  strncpy(a->nome, nome, 100);
}

char *aluno_getNome(aluno_t *a)
{
  return a->nome;
}
```
- Perceba que diferente dos métodos `getNome` e `setNome` em `Java`. Na linguagem `C` somos obrigados a informar o endereço do ponteiro que queremos alterar ou consultar o nome (ou qualquer outro atributo). Isto ocorre porque o `Java` já tem conhecimento da referência (ponteiro) do objeto, mas `C` não tem esse conhecimento.
- Perceba, também, o que ocorre dentro da função `new_aluno`.
  - Estamos alocando (instanciando) espaço para um novo "objeto" do tipo aluno.
  - Definindo as referências de todos os métodos para as funções correspondentes (que estão fora da `struct`, ao contrário do `Java` onde os métodos estão dentro da classe ("struct")).
  - Retornando o endereço (referência) da variável recém-alocada.
```c
//main.c
#include <stdio.h>
#include "aluno.h"

int main(void)
{
  aluno_t *aluno = new_aluno();
  aluno->setNome(aluno, "Fulano de Tal");
  printf("Nome: %s\n", aluno->getNome(aluno));
}
```
- Observe ainda que a chamada dos "métodos" (funções) em `C` ocorrem com uso do operador seta (`->`), uma vez que nossa variável `aluno` é do tipo ponteiro. Além disso, passamos a referência (endereço) apontada pela variável `aluno` para ambos os métodos `setNome` e `getNome`.

## Implementando Arrays

## Implementando Listas Ligadas

## Árvores

## Mais Estruturas de Dados
