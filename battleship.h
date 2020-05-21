#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PERDI "perdi"

//Navios e numero de casas que ocupa no tabuleiro
#define portaAvioes 5 //p
#define navioTanque 4 //n
#define contratorpedeiro 3 //c
#define submarino 2 //s

//Numero de navios de cada tipo
#define num_pa 1
#define num_nt 2
#define num_con 3
#define num_sub 4

void imprimeTabuleiro(int** tabuleiro);
int letra2num(char letra);
int numeroColuna(char numero);
int numeroLinha(char letra);


void criaOponente(int** tabuleiroOponente)
{
	srand(time(NULL));
	int linha, coluna, orientacao;
	//insere porta-aviões (ocupa 5 casas, logo só pode começar de 0-5 ou a-f)
	for (int i = 0; i < num_pa; ++i){
		orientacao = rand() % 2;
		if (orientacao == 0){ //horizontal
			linha = rand() % 10;
			coluna = rand() % 6;
			for (int j = 0; j < portaAvioes; ++j){
				tabuleiroOponente[linha][coluna] = portaAvioes;
				coluna++;
			}
		} else { //vertical
			linha = rand() % 6;
			coluna = rand() % 10;
			for (int j = 0; j < portaAvioes; ++j){
				tabuleiroOponente[linha][coluna] = portaAvioes;
				linha++;
			}
		}
	}

	//insere navios-tanque (ocupa 4 casas, logo só pode começar de 0-6 ou a-g)
	for (int i = 0; i < num_nt; ++i){
		orientacao = rand() % 2;
		if (orientacao == 0){ //horizontal
			do{
				linha = rand() % 10;
				coluna = rand() % 7;
			} while (tabuleiroOponente[linha][coluna] != 8 || tabuleiroOponente[linha][coluna+1] != 8 || tabuleiroOponente[linha][coluna+2] != 8 || tabuleiroOponente[linha][coluna+3] != 8);
			for (int j = 0; j < navioTanque; ++j){
				tabuleiroOponente[linha][coluna] = navioTanque;
				coluna++;
			}
		} else { //vertical
			do{
				linha = rand() % 7;
				coluna = rand() % 10;
			} while (tabuleiroOponente[linha][coluna] != 8 || tabuleiroOponente[linha+1][coluna] != 8 || tabuleiroOponente[linha+2][coluna] != 8 || tabuleiroOponente[linha+3][coluna] != 8);
			for (int j = 0; j < navioTanque; ++j){
				tabuleiroOponente[linha][coluna] = navioTanque;
				linha++;
			}
		}
	}

	//insere contratorpedeiros (ocupa 3 casas, logo só pode começar de 0-7 ou a-h)
	for (int i = 0; i < num_con; ++i){
		orientacao = rand() % 2;
		if (orientacao == 0){ //horizontal
			do{
				linha = rand() % 10;
				coluna = rand() % 8;
			} while (tabuleiroOponente[linha][coluna] != 8 || tabuleiroOponente[linha][coluna+1] != 8 || tabuleiroOponente[linha][coluna+2] != 8);
			for (int j = 0; j < contratorpedeiro; ++j){
				tabuleiroOponente[linha][coluna] = contratorpedeiro;
				coluna++;
			}
		} else { //vertical
			do{
				linha = rand() % 8;
				coluna = rand() % 10;
			} while (tabuleiroOponente[linha][coluna] != 8 || tabuleiroOponente[linha+1][coluna] != 8 || tabuleiroOponente[linha+2][coluna] != 8);
			for (int j = 0; j < contratorpedeiro; ++j){
				tabuleiroOponente[linha][coluna] = contratorpedeiro;
				linha++;
			}
		}
	}

	//insere submarinos (ocupa 2 casas, logo só pode começar de 0-8 ou a-i)
	for (int i = 0; i < num_sub; ++i){
		orientacao = rand() % 2;
		if (orientacao == 0){ //horizontal
			do{
				linha = rand() % 10;
				coluna = rand() % 9;
			} while (tabuleiroOponente[linha][coluna] != 8 || tabuleiroOponente[linha][coluna+1] != 8);
			for (int j = 0; j < submarino; ++j){
				tabuleiroOponente[linha][coluna] = submarino;
				coluna++;
			}
		} else { //vertical
			do{
				linha = rand() % 9;
				coluna = rand() % 10;
			} while (tabuleiroOponente[linha][coluna] != 8 || tabuleiroOponente[linha+1][coluna] != 8);
			for (int j = 0; j < submarino; ++j){
				tabuleiroOponente[linha][coluna] = submarino;
				linha++;
			}
		}
	}
}

void imprimeTabuleiro(int** tabuleiro)
{
	printf("Tabuleiro:\n");
	printf("  0 1 2 3 4 5 6 7 8 9\n");
	char coluna  = 'A';
	for (int i = 0; i < 10; ++i){
		printf("%c ", coluna);
		for (int j = 0; j < 10; ++j){
			printf("%d ", tabuleiro[i][j]);
		}
		coluna = coluna + 1;
		printf("\n");
	}
}

int vazio(int** tabuleiro)
{
	for (int i = 0; i < 10; ++i){
		for (int j = 0; j < 10; ++j){
			if (tabuleiro[i][j] != 0 && tabuleiro[i][j] != 1 && tabuleiro[i][j] != 8)
				return 0;
		}
	}
	return 1;
}

void posicionar_navio(char linha[10], int** tabuleiro)
{
	int navio; //tipo do navio

	//tipo do navio
	if (linha[0] == 'p'){ //porta aviao
		navio = portaAvioes;
	} else if (linha[0] == 'n'){ //navio tanque
		navio = navioTanque;
	} else if (linha[0] == 'c'){ //contratorpedeiro
		navio = contratorpedeiro;
	} else { //submarino
		navio = submarino;
	}

	int x, y = 0;
	//coordenadas da posicao inicial do navio
	x = numeroLinha(linha[2]);
	y = numeroColuna(linha[4]);

	//orientacao do navio
	if (linha[6] == 'h'){ //horizontal
		for (int i = y; i < y+navio; ++i){
			tabuleiro[x][i] = navio;
		}
	} else { //vertical
		for (int i = x; i < x+navio; ++i){
			tabuleiro[i][y] = navio;
		}
	}
}

void teclaP(int** tabuleiro, int** meusTiros)
{
	printf("\n  Seu tabuleiro:\n");
	printf("  0 1 2 3 4 5 6 7 8 9\n");
	char coluna  = 'A';
	for (int i = 0; i < 10; ++i){
		printf("%c ", coluna);
		for (int j = 0; j < 10; ++j){
			if (tabuleiro[i][j] > 1 && tabuleiro[i][j] < 8){ //posições de barco ainda não atacadas
				printf("1 ");
			}
			else if (tabuleiro[i][j] == 8 || tabuleiro[i][j] == 0){ //posições de água não jogadas ainda
				printf("- ");
			} else if (tabuleiro[i][j] == 1){//posições de barco já atacadas
				printf("x ");
			}
		}
		coluna = coluna + 1;
		printf("\n");
	}

	printf("\n  Seus tiros:\n");
	printf("  0 1 2 3 4 5 6 7 8 9\n");
	coluna  = 'A';
	for (int i = 0; i < 10; ++i){
		printf("%c ", coluna);
		for (int j = 0; j < 10; ++j){
			if (meusTiros[i][j] == 8){ //posicao não jogada ainda
				printf("- ");
			} else if (meusTiros[i][j] == 0){ //agua
				printf("0 ");
			} else if (meusTiros[i][j] == 1) { //barco
				printf("x ");
			}
		}
		coluna = coluna + 1;
		printf("\n");
	}
}

void layout(int** tabuleiro, char *arq)
{
	int c;
	int i = 0;
	char linha[10] ;
	FILE *file = fopen("entrada.txt","r");
	if (file) {
		while((c = getc(file)) != EOF) {
			if (c != '\n') {
				linha[i] = c;
				i++;
			} else {
				posicionar_navio(linha, tabuleiro);
				i = 0;
			}
		}
	} else {
		perror("\nNenhum arquivo de entrada encontrado\n");
		exit(EXIT_FAILURE);
	}
}

int jogada(int x, int y, int** tabuleiro){
	if (tabuleiro[x][y] > 1 && tabuleiro[x][y] < 6){ //acertou navio
			return 1;
		} else { //acertou água
			return 0;
		}
}

/*Cria matrizes de números inteiros
  Parâmetros:
  1. i: número de linhas da matriz
  2. j: número de colunas da matriz
*/
int** criaMatriz(int i, int j) 
{
	int** retorno = (int**)malloc(sizeof(int*)*i);
	for(int x = 0; x < i; x++) {
		retorno[x] = (int*)malloc(sizeof(int*)*j);
	}
	return retorno;
}

//transforma o numero da linha
int numeroLinha(char letra){
	if (letra > 96 && letra < 107){
		return letra-97;
	} else if (letra > 64 && letra < 75){
		return letra-75;
	}
}

//transforma o numero da coluna
int numeroColuna(char numero){
	return numero-48;
}

void limparBuffer(){
	char c;
	while((c = getchar()) != '\n' && c != EOF);
}

int str2int(char *str)
{
	int t = strlen(str);
	int res = 0;
	for (int i=0; i< t; ++i) {
		res += letra2num(str[i]) * pow(10,t-i-1);
	}
	return res;
}

int letra2num(char letra)
{
	int num = 0;
	if (letra < 58)
		num = letra-48;
	else if (letra < 91)
		num = letra-65;
	else
		num = letra-97;
	return num;
}