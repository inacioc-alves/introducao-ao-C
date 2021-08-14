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