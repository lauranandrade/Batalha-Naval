#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

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

//Variáveis globais
int tabuleiro[10][10]; //Matriz para representar o tabuleiro do jogo
int tabuleiroOponente[10][10]; //Matriz para representar o tabuleiro do jogo

void criaOponente()
{
	srand(time(NULL));
	int linha, coluna, orientacao;
	//insere porta-aviões (ocupa 5 casas, logo só pode começar de 0-5 ou a-f)
	for (int i = 0; i < num_pa; ++i){
		orientacao = rand() % 2;
		if (orientacao == 0){ //horizontal
			linha = rand() % 10;
			coluna = rand() % 6;
			for (int j = 0; j < num_pa; ++j){
				tabuleiroOponente[linha][coluna] = portaAvioes;
				coluna++;
			}
		} else { //vertical
			linha = rand() % 6;
			coluna = rand() % 10;
			for (int j = 0; j < num_pa; ++j){
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
			} while (tabuleiroOponente[linha][coluna] != 0 && tabuleiroOponente[linha][coluna+1] != 0 && tabuleiroOponente[linha][coluna+2] != 0 && tabuleiroOponente[linha][coluna+3] != 0);
			for (int j = 0; j < num_nt; ++j){
				tabuleiroOponente[linha][coluna] = navioTanque;
				coluna++;
			}
		} else { //vertical
			do{
				linha = rand() % 7;
				coluna = rand() % 10;
			} while (tabuleiroOponente[linha][coluna] != 0 && tabuleiroOponente[linha+1][coluna] != 0 && tabuleiroOponente[linha+2][coluna] != 0 && tabuleiroOponente[linha+3][coluna] != 0);
			for (int j = 0; j < num_nt; ++j){
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
			} while (tabuleiroOponente[linha][coluna] != 0 && tabuleiroOponente[linha][coluna+1] != 0 && tabuleiroOponente[linha][coluna+2] != 0);
			for (int j = 0; j < num_con; ++j){
				tabuleiroOponente[linha][coluna] = contratorpedeiro;
				coluna++;
			}
		} else { //vertical
			do{
				linha = rand() % 8;
				coluna = rand() % 10;
			} while (tabuleiroOponente[linha][coluna] != 0 && tabuleiroOponente[linha+1][coluna] != 0 && tabuleiroOponente[linha+2][coluna] != 0);
			for (int j = 0; j < num_con; ++j){
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
			} while (tabuleiroOponente[linha][coluna] != 0 && tabuleiroOponente[linha][coluna+1] != 0);
			for (int j = 0; j < num_sub; ++j){
				tabuleiroOponente[linha][coluna] = submarino;
				coluna++;
			}
		} else { //vertical
			do{
				linha = rand() % 9;
				coluna = rand() % 10;
			} while (tabuleiroOponente[linha][coluna] != 0 && tabuleiroOponente[linha+1][coluna] != 0);
			for (int j = 0; j < num_sub; ++j){
				tabuleiroOponente[linha][coluna] = submarino;
				linha++;
			}
		}
	}
}

void imprimeTabuleiro()
{
	printf("Seu tabuleiro:\n");
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

int vazio()
{
	for (int i = 0; i < 10; ++i){
		for (int j = 0; j < 10; ++j){
			if (tabuleiro[i][j] != 0)
				return 0;
		}
	}
	return 1;
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

void posicionar_navio(char linha[10])
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

void teclaP()
{
	printf("\n  Seu tabuleiro:\n");
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

	printf("\n  Tabuleiro oponente:\n");
	printf("  0 1 2 3 4 5 6 7 8 9\n");
	coluna  = 'A';
	for (int i = 0; i < 10; ++i){
		printf("%c ", coluna);
		for (int j = 0; j < 10; ++j){
			if (tabuleiroOponente[i][j] != 1 && tabuleiroOponente[i][j] != 8){ //posicao não jogada ainda
				printf("- ");
			} else if (tabuleiroOponente[i][j] == 1){ //agua
				printf("0 ");
			} else if (tabuleiroOponente[i][j] == 8) { //barco
				printf("1 ");
			}
		}
		coluna = coluna + 1;
		printf("\n");
	}
}

void layout()
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
				posicionar_navio(linha);
				i = 0;
			}
		}
	} else {
		perror("\nNenhum arquivo de entrada encontrado\n");
		exit(EXIT_FAILURE);
	}
}

void limparBuffer(){
	char c;
	while((c = getchar()) != '\n' && c != EOF);
}

int main(int argc, char const *argv[])
{
	//Inicia o tabuleiro
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			tabuleiro[i][j] = 0;
			tabuleiroOponente[i][j] = 0;
		}
	}
	layout();
	imprimeTabuleiro();
	criaOponente();
	//imprimeTabuleiro();
	int fim = 0;
	int x, y = 0;
	char aux;
	while (fim != 1){
	inicio:
		printf("\nDigite a linha do tabuleiro que deseja atacar (a-j ou A-J) ou P para visualizar os tabuleiros: ");
		x = getchar();
		if (x == 'p' || x == 'P'){
			teclaP();
			limparBuffer();
			goto inicio;
		} else if ((x > 96 && x < 107) || (x > 64 && x < 74)){
			aux = x;
			x = numeroLinha(x);
			limparBuffer();
		} else {
			limparBuffer();
			printf("\nLinha inválida!\n");
			goto inicio;
		}
	repete:
		printf("\nDigite a coluna do tabuleiro que deseja atacar (0-9): ");
		y = getchar();
		if (y < 48 || y > 57) {
			printf("\nColuna inválida!\n");
			limparBuffer();
			goto repete;
		} else {
			y = numeroColuna(y);
			limparBuffer();
		}
		printf("\nPosição do tiro: linha '%c' coluna  '%d'.\n", aux, y);
		if (tabuleiroOponente[x][y] != 0 && tabuleiroOponente[x][y] != 1 && tabuleiro[x][y] != 8){ //acertou navio
			tabuleiroOponente[x][y] = 8;
			printf("\nBoom!\n");
		} else if (tabuleiroOponente[x][y] == 1 || tabuleiroOponente[x][y] == 8){ //acertou posição que já foi jogada
			printf("\nVocê já jogou essa posição! Jogue novamente!\n");
			goto repete;
		} else { //acertou água
			printf("\nSplash!\n");
			tabuleiroOponente[x][y] = 1;
		}
		fim = vazio();
	}
	printf("\nVocê venceu!\n");
	return 0;
}