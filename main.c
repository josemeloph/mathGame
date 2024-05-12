#include <stdio.h>
#include <stdlib.h>

typedef enum {
	soma = '+',
	subtracao = '-',
	multiplicacao = '*',
	divisao = '/',
	exponenciacao = '^',
}Operacao;

typedef struct {
	int n1;
	int n2;
	Operacao op;
	int tamanho;
}Expressao;

Operacao operacoes[5] = { soma, subtracao, multiplicacao, divisao, exponenciacao };


void gerarExpressao(Expressao *expressao) {
	int num1, num2;
	expressao->op = operacoes[rand() % 5];
	switch (expressao->op) {
	case soma:
		num1 = rand() % 1990 + 10;
		num2 = rand() % 1990 + 10;
		break;
	case subtracao:
		do {
			num1 = rand() % 3990 + 10;
			num2 = rand() % 3980 + 10;
		} while (num1 < num2);
		break;
	case multiplicacao:
		num1 = rand() % 98 + 2;
		num2 = rand() % 98 + 2;
		break;
	case divisao:
		do {
			num1 = rand() % 996 + 4;
			num2 = rand() % 998 + 2;
		} while (num1 < num2 || num1 % num2 != 0);
		break;
	case exponenciacao:
		num2 = rand() % 15 + 1;
		if (num2 > 3) num1 = 2;
		else if (num2 == 3) num1 = rand() % 11 + 1;
		else num1 = rand() % 50 + 1;
	}
	expressao->n1 = num1;
	expressao->n2 = num2;
}


int main() {
	srand(time(NULL));
	Expressao expressao;
	gerarExpressao(&expressao);
	printf("%d %c %d", expressao.n1, expressao.op, expressao.n2);
}