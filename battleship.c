#include <stdlib.h>
#include <stdio.h>

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

void imprimeTabuleiro(){
	for (int i = 0; i < 10; ++i){
		for (int j = 0; j < 10; ++j){
			printf("%d ", tabuleiro[i][j]);
		}
		printf("\n");
	}
}

int vazio(){
	for (int i = 0; i < 10; ++i){
		for (int j = 0; j < 10; ++j){
			if (tabuleiro[i][j] != 0){
				return 0;
			}
		}
	}
	return 1;
}

//transforma o numero da linha
int linhas(char letra){
	if (letra > 96 && letra < 107){
		return letra-97;
	} else if (letra > 64 && letra < 75){
		return letra-75;
	} else {
		printf("Caracter diferente de a-j!\n");
	}
}

//transforma o numero da coluna
int colunas(char numero){
	if (numero < 48 || numero > 57){ //se for um caracter diferente de {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
		printf("Caracter diferente de 0-9!\n");
	} else {
		return numero-48;
	}
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
	} else if (linha[0] == 's'){ //submarino
		navio = submarino;
	} else {
		printf("Navio inválido!\n");
	}

	int x, y = 0;
	//coordenadas da posicao inicial do navio
	x = linhas(linha[2]);
	y = colunas(linha[4]);

	//orientacao do navio
	if (linha[6] == 'h'){ //horizontal
		for (int i = y; i < y+navio; ++i){
			tabuleiro[x][i] = navio;
		}
	} else if (linha[6] == 'v'){ //vertical
		for (int i = x; i < x+navio; ++i){
			tabuleiro[i][y] = navio;
		}
	} else {
		printf("Não é vertical e nem horizontal!\n");
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
		}
	}
	layout();
	imprimeTabuleiro();
	int fim = 0;
	int x, y = 0;
	while (fim != 1){
		printf("\nDigite a linha do tabuleiro que deseja atacar (a-j):\n");
		x = getchar();
		x = linhas(x);
		limparBuffer();
		printf("Digite a coluna do tabuleiro que deseja atacar (0-9):\n");
		y = getchar();
		y = colunas(y);
		limparBuffer();
		if (tabuleiro[x][y] != 0){
			tabuleiro[x][y] = 0;
			printf("\nBoom!\n");
			//imprimeTabuleiro();
		} else {
			printf("\nSplash!\n");
		}
		fim = vazio();
	}
	printf("\nVocê venceu!\n");
	return 0;
}