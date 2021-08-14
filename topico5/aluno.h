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