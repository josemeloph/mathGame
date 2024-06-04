#define _CRT_SECURE_NO_WARNINGS

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

typedef struct no {
	char nome[20];
	int valor;
	struct no* prox;
}No;

const char* arqQuestoes = "rankingQuestoes.txt";
const char* arqTempo = "rankingTempo.txt";
No* _ranking;
Operacao _operacoes[5] = { soma, subtracao, multiplicacao, divisao, exponenciacao };
int _opcoes[4];
int _threadTerminou = 0;
int _tempo = 0;
int _modoDeJogo = 0;
char _nomeJogador[10];

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

void inserirNaListaDecrescente(No** lista, char* nome, int valor) {
	No* aux = *lista, * novo = malloc(sizeof(No));
	if (novo) {
		strcpy(novo->nome, nome);
		novo->valor = valor;
		if (!aux || valor > aux->valor) {
			novo->prox = *lista;
			*lista = novo;
		}
		else {
			while (aux->prox && valor <= aux->prox->valor) {
				aux = aux->prox;
			}
			novo->prox = aux->prox;
			aux->prox = novo;
		}
	}
}

void inserirNaListaCrescente(No** lista, char* nome, int valor) {
	No* aux = *lista, * novo = malloc(sizeof(No));
	if (novo) {
		strcpy(novo->nome, nome);
		novo->valor = valor;
		if (!aux || valor < aux->valor) {
			novo->prox = *lista;
			*lista = novo;
		}
		else {
			while (aux->prox && valor >= aux->prox->valor) {
				aux = aux->prox;
			}
			novo->prox = aux->prox;
			aux->prox = novo;
		}
	}
}

void liberarLista(No* lista) {
	No* aux;
	while (lista) {
		aux = lista;
		lista = lista->prox;
		free(aux);
	}
}

No* lerRanking(const char* nomeArq) {
	FILE* arq;
	No* ranking = NULL;
	char nome[20];
	int dado;

	arq = fopen(nomeArq, "r");
	if (arq) {
		while (fscanf(arq, " %19[^,],%d", nome, &dado) == 2) {
			if (_modoDeJogo == 0) {
				inserirNaListaDecrescente(&ranking, nome, dado);
			}
			else {
				inserirNaListaCrescente(&ranking, nome, dado);
			}
		}
		fclose(arq);
	}
	return ranking;
}


void rankingGlobal(const char* nomeArq) {
	No* ranking = lerRanking(nomeArq);
	No* aux = ranking;
	int min, seg, posicao = 0, lin = 3, col1 = 12, col2 = 28;
	linhaCol(lin, 14);
	printf("RANKING GLOBAL");
	while (aux && posicao < 5) {
		textColor(BRANCO, _PRETO);
		posicao++;
		lin += 2;
		linhaCol(lin, col1);
		if (strcmp(aux->nome, _nomeJogador) == 0) {
			textColor(VERDE, _PRETO);
		}
		printf("%d. %s", posicao, aux->nome);

		if (_modoDeJogo == 0) {
			linhaCol(lin, col2);
			printf("%d", aux->valor);
		}
		else {
			linhaCol(lin, col2 - 2);
			min = aux->valor / 60;
			seg = aux->valor - (min * 60);
			printf("%02d:%02d", min, seg);
		}
		aux = aux->prox;
	}
	if (posicao == 0) {
		linhaCol(lin + 2, 8);
		textColor(VERMELHO, _PRETO);
		printf("Nao tem ninguem no ranking\n");
	}
	getch();
	liberarLista(ranking);
	linhaCol(BRANCO, _PRETO);
}

void recordePessoal(const char* nomeArq) {
	No* ranking = lerRanking(nomeArq);
	No* aux = ranking;
	int min, seg, posicao = 0, lin = 3, col1 = 12, col2 = 28;
	linhaCol(lin, 14);
	printf("RECORDE PESSOAL");
	while (aux && posicao < 5) {
		if (strcmp(aux->nome, _nomeJogador) == 0) {
			posicao++;
			lin += 2;
			linhaCol(lin, col1);
			printf("%d. %s", posicao, aux->nome);
			linhaCol(lin, col2);
			if (_modoDeJogo == 0) {
				linhaCol(lin, col2);
				printf("%d", aux->valor);
			}
			else {
				linhaCol(lin, col2 - 2);
				min = aux->valor / 60;
				seg = aux->valor - (min * 60) - 1;
				printf("%02d:%02d", min, seg);
			}
		}
		aux = aux->prox;
	}
	if (posicao == 0) {
		linhaCol(lin + 2, 8);
		textColor(VERMELHO, _PRETO);
		printf("Voce nao tem nenhum recorde\n");
	}
	getch();
	linhaCol(BRANCO, _PRETO);
}


void inserirNoRanking(const char* nomeArq, char* nome, int dado) {
	FILE* arq;
	arq = fopen(nomeArq, "a");
	if (arq) {
		fprintf(arq, "%s,%d\n", nome, dado);
	}
	fclose(arq);
}

int buscaRecorde(const char* nomeArq) {
	No* ranking = lerRanking(nomeArq);
	No* aux = ranking;
	int busca = 0;
	while (aux) {
		if (strcmp(aux->nome, _nomeJogador) == 0) {
			busca = aux->valor;
			break;
		}
		aux = aux->prox;
	}
	liberarLista(ranking);
	return busca;
}

int buscaPosRanking(const char* nomeArq) {
	No* ranking = lerRanking(nomeArq);
	No* aux = ranking;
	int posicao = 0;
	while (aux) {
		posicao++;
		if (strcmp(aux->nome, _nomeJogador) == 0) {
			break;
		}
		aux = aux->prox;
	}
	liberarLista(ranking);
	return posicao;
}


int menu(int lin1, int col1, int qtd, char lista[3][40], int escolherModo) {
	int i, lin2, linOpcao = 0, opcao = 0, teclaPressionada;

	while (1) {
		lin2 = lin1;
		system("cls");
		linhaCol(14, 9);
		textColor(AZUL_CLARO, _PRETO);
		printf("%s", _nomeJogador);
		for (i = 0; i < qtd; i++) {
			if (i == opcao) {
				textColor(PRETO, _BRANCO);
				linOpcao = lin2;
			}
			else {
				textColor(BRANCO, _PRETO);
			}
			linhaCol(lin2, col1 + 2);
			printf("%s", lista[i]);
			lin2 += 2 + escolherModo;
			textColor(BRANCO, _PRETO);
		}
		if (escolherModo) {
			linhaCol(linOpcao + 1, col1 + 2);
			textColor(VERDE_CLARO, _PRETO);
			if (opcao < 1) {
				printf("Max questoes em 1 min");
			}
			else if (opcao < 2) {
				printf("20 questoes no menor tempo");
			}
			else {
				printf("Tempo e questoes infinitas");
			}
		}
		teclaPressionada = getch();

		if (teclaPressionada == teclaEnter) {
			break;
		}
		else if (teclaPressionada == teclaSetaCima) {
			if (opcao > 0)opcao--;

		}
		else if (teclaPressionada == teclaSetaBaixo) {
			if (opcao < qtd - 1)opcao++; ;
		}
		else if (teclaPressionada == teclaEsc) {
			return -1;
		}

	}
	textColor(BRANCO, _PRETO);
	system("cls");
	return opcao;
}


int customGetch() {
	while (!_threadTerminou) {
		if (kbhit()) {
			return getch();
		}
	}
	return -1;
}

int menuOpcoes(int lin, int col, int opcoes[]) {
	int i, col2, opcao = 0, aux, teclaPressionada, tamOpcoes[2];
	tamOpcoes[0] = opcoes[0] < 0 ? snprintf(NULL, 0, "%d", -1 * opcoes[0]) : snprintf(NULL, 0, "%d", opcoes[0]);
	tamOpcoes[1] = opcoes[1] < 0 ? snprintf(NULL, 0, "%d", -1 * opcoes[1]) : snprintf(NULL, 0, "%d", opcoes[1]);
	while (1) {
		for (i = 0; i < 4; i++) {
			col2 = col;
			if (i >= 2) {
				col2 += 15;
			}
			else {
				col2 += 6 - tamOpcoes[i];
			}
			if (opcoes[i] < 0) {
				if (i == opcao) {
					textColor(VERMELHO, _BRANCO);
				}
				else {
					textColor(VERMELHO, _PRETO);
				}
				linhaCol(lin + (i % 2) * 2, col2);
				opcoes[i] *= -1;
				printf("%d", opcoes[i]);
				opcoes[i] *= -1;
			}
			else {
				if (i == opcao) {
					textColor(PRETO, _BRANCO);
				}
				else {
					textColor(BRANCO, _PRETO);
				}
				linhaCol(lin + (i % 2) * 2, col2);
				printf("%d", opcoes[i]);
			}
		}
		if (!_threadTerminou) {
			teclaPressionada = customGetch();
		}
		else {
			break;
		}

		if (teclaPressionada == teclaEnter) {
			if (opcoes[opcao] >= 0) {
				break;
			}
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
		else if (teclaPressionada == teclaEsc) {
			return -1;
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

void gerarExpressao(Expressao* expressao) {

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

int printarExpressao(Expressao expressao, int opcoes[], int resposta) {
	int opcaoSelecionada;
	linhaCol(4, 19 - expressao.tamanho);
	printf("%d", expressao.n1);
	linhaCol(4, 21);
	printf("%c", expressao.op);
	linhaCol(4, 23);
	printf("%d", expressao.n2);
	opcaoSelecionada = menuOpcoes(6, 11, opcoes);
	if (opcaoSelecionada == -1) {
		return 1;
	}
	else {
		while (opcoes[opcaoSelecionada] != resposta) {
			if (_threadTerminou) {
				break;
			}
			if (_modoDeJogo == 0) {
				_tempo -= 5;
			}
			opcoes[opcaoSelecionada] *= -1;
			opcaoSelecionada = menuOpcoes(6, 11, opcoes);
			if (opcaoSelecionada == -1) {
				return 1;
			}
		}
	}
	return 0;
}

int gerarQuestao(int opcoes[], int* certas) {
	Expressao expressao;
	int resposta, opcaoSelecionada, voltarAoInicio;

	gerarExpressao(&expressao);
	resposta = calcularResposta(expressao);
	gerarOpcoes(resposta, opcoes);
	voltarAoInicio = printarExpressao(expressao, opcoes, resposta);
	if (voltarAoInicio) {
		return 1;
	}
	(*certas)++;
	return 0;
}

DWORD WINAPI threadQuestoes(LPVOID parametro) {
	int acertou = 0, certas = 0, maxQuestoes = parametro, voltarAoInicio;
	srand(time(NULL));
	if (maxQuestoes == 0) {
		while (!_threadTerminou) {
			for (int i = 4; i < 10; i++) {
				linhaCol(i, 1);
				printf("                                                                   ");
			}
			linhaCol(12, 8);
			printf("Questoes: %d", certas);
			voltarAoInicio = gerarQuestao(_opcoes, &certas);
			if (voltarAoInicio) {
				certas = -1;
				_tempo = 0;
				break;
			}
		}
	}
	else {
		while (certas < maxQuestoes) {
			for (int i = 4; i < 10; i++) {
				linhaCol(i, 1);
				printf("                                                                   ");
			}
			linhaCol(12, 8);
			printf("Questoes: %d/20", certas);
			voltarAoInicio = gerarQuestao(_opcoes, &certas);
			if (voltarAoInicio) {
				certas = -1;
				break;
			}
		}
	}

	_threadTerminou = 1;
	return certas;
}

DWORD WINAPI threadCronometro() {
	if (_tempo != 0) {
		while (_tempo > 0) {
			Sleep(1000);
			_tempo--;
			linhaCol(12, 24);
			textColor(BRANCO, _PRETO);
			printf("Tempo: 00:%02d", _tempo);
		}
	}
	else {
		while (!_threadTerminou) {
			Sleep(1000);
			_tempo++;
			linhaCol(12, 24);
			textColor(BRANCO, _PRETO);
			printf("Tempo: 00:%02d", _tempo);
		}
	}
	_threadTerminou = 1;
}

void fimDoJogo(int questoes) {
	int tempo, min, seg, lin, col1, col2;
	if (questoes == -1) return;
	lin = 3, col1 = 12, col2 = 26;
	system("cls");
	linhaCol(lin, 14);
	textColor(VERMELHO, _PRETO);
	printf("JOGO ENCERRADO");
	textColor(BRANCO, _PRETO);
	linhaCol(lin + 2, col1);
	printf("Apelido: ");
	linhaCol(lin + 2, col2);
	printf("%s", _nomeJogador);
	linhaCol(lin + 6, col1);
	printf("Recorde: ");
	linhaCol(lin + 8, col1);
	printf("Ranking: ");
	if (_modoDeJogo == 0) {
		inserirNoRanking(arqQuestoes, _nomeJogador, questoes - 1);
		linhaCol(lin + 4, col1);
		printf("Questoes: ");
		linhaCol(lin + 4, col2);
		printf("%d", questoes - 1);
		linhaCol(lin + 6, col2);
		printf("%d", buscaRecorde(arqQuestoes));
		linhaCol(lin + 8, col2);
		printf("%d%c", buscaPosRanking(arqQuestoes), 248);
	}
	else {
		min = _tempo / 60;
		seg = _tempo - (min * 60) - 1;
		inserirNoRanking(arqTempo, _nomeJogador, _tempo - 1);
		linhaCol(lin + 4, col1);
		printf("Tempo: ");
		linhaCol(lin + 4, col2);
		printf("%02d:%02d", min, seg);
		linhaCol(lin + 6, col2);
		tempo = buscaRecorde(arqTempo);
		min = tempo / 60;
		seg = tempo - (min * 60);
		printf("%02d:%02d", min, seg);
		linhaCol(lin + 8, col2);
		printf("%d%c", buscaPosRanking(arqTempo), 248);
	}

	getchar();
	return;
}



void inicializarJogo() {
	int maxQuestoes, opcaoSelecionada, certas, tempo, tamanhoLista;
	HANDLE threads[2];
	DWORD idThreads[2];

	system("cls");
	tamanhoLista = 3;
	char lista[3][40] = { " QUESTOES ", " TEMPO ", " SEM LIMITES " };
	opcaoSelecionada = menu(4, 6, tamanhoLista, lista, 1);
	if (opcaoSelecionada == -1) return;
	_modoDeJogo = opcaoSelecionada;
	_threadTerminou = 0;
	if (_modoDeJogo == 0) {
		_tempo = 60, maxQuestoes = 0;
	}
	else if (_modoDeJogo == 1) {
		_tempo = 0, maxQuestoes = 20;
	}
	else {
		_tempo = 0, maxQuestoes = 0;
	}

	threads[0] = CreateThread(NULL, 0, threadCronometro, NULL, 0, &idThreads[0]);
	threads[1] = CreateThread(NULL, 0, threadQuestoes, maxQuestoes, 0, &idThreads[1]);
	WaitForMultipleObjects(2, threads, 1, INFINITE);
	GetExitCodeThread(threads[1], (LPDWORD)&certas);
	CloseHandle(threads[0]);
	CloseHandle(threads[1]);
	if (certas != -1) {
		fimDoJogo(certas);
	}
}

void receberNomeJogador() {
	while (1) {
		linhaCol(6, 7);
		textColor(BRANCO, _PRETO);
		printf("Digite seu apelido: ");
		fgets(_nomeJogador, 10, stdin);

		size_t len = strlen(_nomeJogador);
		if (len > 0 && _nomeJogador[len - 1] == '\n') {
			_nomeJogador[len - 1] = '\0';
			len--;
		}

		if (len == 0) {
			linhaCol(7, 7);
			textColor(VERMELHO, _PRETO);
			printf("Nome nao pode estar vazio.");
		}
		else {
			break;
		}
	}
}

int main() {
	system("MODE con cols=41 lines=15 ");
	int opcaoSelecionada, tamanhoLista;
	while (1) {
		system("cls");
		linhaCol(3, 13);
		textColor(VERDE, _PRETO);
		printf("SEJA BEM VINDO!");
		receberNomeJogador();;
		while (1) {
			system("cls");
			textColor(BRANCO, _PRETO);

			char lista[4][40] = { " JOGAR ", " RECORDE PESSOAL ", " RANKING GLOBAL ", " SAIR " };
			tamanhoLista = 4;
			opcaoSelecionada = menu(4, 6, tamanhoLista, lista, 0);
			if (opcaoSelecionada == 0) {
				inicializarJogo();
			}
			else if (opcaoSelecionada == 1 || opcaoSelecionada == 2) {
				tamanhoLista = 2;
				char lista[2][40] = { " QUESTOES ", " TEMPO " };
				 _modoDeJogo= menu(5, 6, tamanhoLista, lista, 1);
				 if (opcaoSelecionada == 1) {
					 if (_modoDeJogo == 0) {
						 recordePessoal(arqQuestoes);
					 }
					 else if (_modoDeJogo == 1) {
						 recordePessoal(arqTempo);
					 }
				 }
				 else {
					 if (_modoDeJogo == 0) {
						 rankingGlobal(arqQuestoes);
					 }
					 else if (_modoDeJogo == 1) {
						 rankingGlobal(arqTempo);
					 }

				 }
			}
			else if (opcaoSelecionada == 3) {
				break;
			}
		}
	}
}
