#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

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

Operacao _operacoes[5] = { soma, subtracao, multiplicacao, divisao, exponenciacao };
int _opcoes[4];


void linhaCol(int lin, int col) {
	COORD posicao;
	posicao.X = col - 1;
	posicao.Y = lin - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posicao);

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 100;
	cursor.bVisible = 0;
	SetConsoleCursorInfo(console, &cursor);
}




int calcularResposta(Expressao expressao) {
	switch (expressao.op) {
	case soma:
		return expressao.n1 + expressao.n2;
	case subtracao:
		return expressao.n1 - expressao.n2;
	case multiplicacao:
		return expressao.n1 * expressao.n2;
	case divisao:
		return expressao.n1 / expressao.n2;
	case exponenciacao:
		return pow(expressao.n1, expressao.n2);
	default:
		return NULL;
	}
}


int tamanhoExpressao(int n1) {
	int tamanho = 0;
	for (int i = 1000; i >= 10; i = i / 10) {
		if ((float)n1 / i >= 1.0)
			tamanho++;
	}
	return tamanho;
}

void gerarExpressao(Expressao *expressao) {
	int num1, num2;
	expressao->op = _operacoes[rand() % 5];
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
		num2 = rand() % 13 + 1;
		if (num2 > 3) num1 = 2;
		else if (num2 == 3) num1 = rand() % 11 + 1;
		else num1 = rand() % 50 + 1;
	}
	expressao->n1 = num1;
	expressao->n2 = num2;
	expressao->tamanho = tamanhoExpressao(num1);
}

int gerarOpcao(int resposta) {
	int tipoDeOpcao = rand() % 5;
	switch (tipoDeOpcao) {
	case 0:
		return resposta + (rand() % resposta);
	case 1:
		return resposta - (rand() % resposta);
	case 2:
		return resposta * (rand() % 2 + 2);
	case 3:
		return resposta + (rand() % 5 + 1);
	case 4:
		return resposta + (pow(10, rand() % 4));
	default:
		return -1;
	}
}

int opcaoJaGerada(int opcao, int opcoesGeradas[4]) {
	for (int i = 0; i < 4; i++) {
		if (opcao == opcoesGeradas[i]) {
			return 1;
		}
	}
	return 0;
}

void gerarOpcoes(int resposta, int opcoes[]) {
	int opcoesGeradas[4], posResposta = rand() % 4;
	opcoes[posResposta] = opcoesGeradas[posResposta] = resposta;
	for (int i = 0; i < 4; i++) {
		if (i != posResposta) {
			do {
				opcoes[i] = gerarOpcao(resposta);
			} while (opcaoJaGerada(opcoes[i], opcoesGeradas));
			opcoesGeradas[i] = opcoes[i];
		}
	}
}

void printarExpressao(Expressao expressao, int opcoes[]) {
	linhaCol(4, 19 - expressao.tamanho);
	printf("%d", expressao.n1);
	linhaCol(4, 21);
	printf("%c", expressao.op);
	linhaCol(4, 23);
	printf("%d", expressao.n2);

	for (int i = 0; i < 4; i++) {
		linhaCol(6 + i, 15);
		printf("Opcao %d: %d", i + 1, opcoes[i]);
	}
}


int main() {
	system("MODE con cols=41 lines=15 ");
	srand(time(NULL));
	int resposta;
	Expressao expressao;
	gerarExpressao(&expressao);
	resposta = calcularResposta(expressao);
	gerarOpcoes(resposta, _opcoes);
	printarExpressao(expressao, _opcoes);
}