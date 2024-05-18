#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>


//cores da letra
enum {
	PRETO = 0,                 
	AZUL = 1,                  
	VERDE = 2,                                   
	VERMELHO = 4,                                
	CINZA_CLARO = 7,           
	CINZA_ESCURO = 8,              
	AZUL_CLARO = 9,             
	VERDE_CLARO = 10,                    
	AMARELO = 14,                
	BRANCO = 15                  
};

//cores do fundo
enum {
	_PRETO = 0,                  
	_AZUL = 16,                 
	_VERDE = 32,                                
	_VERMELHO = 64,                                                   
	_BRANCO = 240       
};

//teclas
enum {
	teclaEnter = 13,
	teclaEsc = 27,
	teclaSetaCima = 72,
	teclaSetaBaixo = 80,
	teclaSetaEsquerda = 75,
	teclaSetaDireita = 77,
};

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
int _tempoAcabou = 0;
int _tempo = 0;

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

void textColor(int letras, int fundo) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), letras + fundo);
}

int menu(int lin1, int col1, int qtd, char lista[3][40]) {
	int i, lin2, opcao = 0, teclaPressionada;

	while (1) {
		lin2 = lin1;
		for (i = 0; i < qtd; i++) {
			if (i == opcao) {
				textColor(PRETO, _BRANCO);
			}
			else {
				textColor(BRANCO, _PRETO);
			}
			linhaCol(lin2, col1 + 2);
			printf("%s", lista[i]);
			lin2 += 2;
		}

		teclaPressionada = getch();

		if (teclaPressionada == teclaEsc) { 
			opcao = -1; 
			break;
		}
		else if (teclaPressionada == teclaEnter) {
			break;
		}
		else if (teclaPressionada == teclaSetaCima) {
			if (opcao > 0)opcao--;   

		}
		else if (teclaPressionada == teclaSetaBaixo) {
			if (opcao < qtd - 1)opcao++;  ;     
		}
	}
	return opcao;
}


int customGetch() {
	while (!_tempoAcabou) {
		if (kbhit()) {
			return getch();
		}
	}
	return -1;
}

int menuOpcoes(int lin, int col, int opcoes[]) {
	int i, col2, opcao = 0, teclaPressionada, tamOpcoes[2];
	tamOpcoes[0] = snprintf(NULL, 0, "%d", opcoes[0]), tamOpcoes[1] = snprintf(NULL, 0, "%d", opcoes[1]);
	while (1) {
		for (i = 0; i < 4; i++) {
			col2 = col;
			if (i >= 2) {
				col2 += 15;
			}
			else {
				col2 += 6 - tamOpcoes[i];
			}
			if (i == opcao) {
				textColor(PRETO, _BRANCO);
			}
			else {
				textColor(BRANCO, _PRETO);
			}
			linhaCol(lin + (i % 2) * 2, col2);
			printf("%d", opcoes[i]);
		}		
		if (_tempoAcabou) {
			return -1;
		}
		else {
			teclaPressionada = customGetch();
		}

		if (teclaPressionada == teclaEnter) {
			break;
		}
		else if (teclaPressionada == teclaSetaCima && opcao % 2 != 0) {
			opcao--;
		}
		else if (teclaPressionada == teclaSetaBaixo && opcao % 2 == 0) {
			opcao++;
		}
		else if (teclaPressionada == teclaSetaEsquerda && opcao >= 2) {
			opcao = opcao - 2;
		}
		else if (teclaPressionada == teclaSetaDireita && opcao < 2) {
			opcao = opcao + 2;
		}
	}
	textColor(BRANCO, _PRETO);
	return opcao;
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

int printarExpressao(Expressao expressao, int opcoes[]) {
	linhaCol(4, 19 - expressao.tamanho);
	printf("%d", expressao.n1);
	linhaCol(4, 21);
	printf("%c", expressao.op);
	linhaCol(4, 23);
	printf("%d", expressao.n2);
	return menuOpcoes(6, 11, opcoes);
}

int gerarQuestao(int opcoes[], int* certas) {
	Expressao expressao;
	int resposta, opcaoSelecionada;

	gerarExpressao(&expressao);
	resposta = calcularResposta(expressao);
	gerarOpcoes(resposta, opcoes);
	opcaoSelecionada = printarExpressao(expressao, opcoes);

	if (resposta == opcoes[opcaoSelecionada]) {
		(*certas)++;
		return 1;
	}
	return 0;
}

DWORD WINAPI threadQuestoes(LPVOID parametro) {
	int acertou, maxQuestoes = parametro, certas = 0;
	srand(time(NULL));
	if (maxQuestoes == 0) {
		while (!_tempoAcabou) {

			for (int i = 4; i < 15; i++) {
				linhaCol(i, 1);
				printf("                                                                   ");
			}
			linhaCol(14, 10);
			printf("Certas: %d", certas);
			acertou = gerarQuestao(_opcoes, &certas);
			if (!acertou) {
				_tempo -= 5;
			}
		}
	}
	return certas;
}

DWORD WINAPI threadCronometro() {	
	if (_tempo != 0) {
		while (_tempo >= 0) {
			Sleep(1000);
			_tempo--;
			linhaCol(2, 10);
			textColor(BRANCO, _PRETO);
			printf("00:%02d", _tempo);
		}
	}
	else {

	}
	_tempoAcabou = 1;
	return _tempo;
}

int inicializarJogo(int modoDeJogo) {
	int maxQuestoes;
	HANDLE threads[2];
	DWORD idThreads[2];


	system("cls");
	if (modoDeJogo == 0) {
		_tempo = 60, maxQuestoes = 0;
		threads[0] = CreateThread(NULL, 0, threadCronometro, NULL, 0, &idThreads[0]);
		threads[1] = CreateThread(NULL, 0, threadQuestoes, maxQuestoes, 0, &idThreads[1]);

		WaitForMultipleObjects(2, threads, 1, INFINITE);

		int certas;
		GetExitCodeThread(threads[1], (LPDWORD)&certas);
		system("cls");
		printf("\nQuestoes corretas: %d\n", certas);
		CloseHandle(threads[0]);
		CloseHandle(threads[1]);
	}
}

int main() {
	system("MODE con cols=41 lines=15 ");
	int opcaoSelecionada, modoDeJogo = 0, tamanhoLista = 4;
	char lista[4][40] = { " JOGAR ", " MODOS DE JOGO ", " RANKING ", " CREDITOS " };
	opcaoSelecionada = menu(4, 6, tamanhoLista, lista);

	if (opcaoSelecionada == 0) {
		inicializarJogo(modoDeJogo);
	}
	else if (opcaoSelecionada == 1) {
	}

	getch();
	system("cls");
}
