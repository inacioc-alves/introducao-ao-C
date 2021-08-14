#include <stdio.h>
#include "aluno.h"

int main(void)
{
  aluno_t *aluno = new_aluno();
  aluno->setNome(aluno, "Fulano de Tal");
  printf("Nome: %s\n", aluno->getNome(aluno));
}