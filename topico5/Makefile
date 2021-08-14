%.o: %.c
	clang -c $< -o $@

main: main.o aluno.o
	clang -o main main.o aluno.o