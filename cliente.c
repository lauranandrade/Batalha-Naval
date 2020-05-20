#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

#include "battleship.h"

int sock = 0, valread; 
struct sockaddr_in serv_addr; 
char ataque[10] = "0 0 0";
char buffer[1024]; 
int opt;
char *ip = "127.0.0.1";
char *arq = "entrada.txt";
int PORT = 8000;

void init_socket()
{
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\nSocket creation error \n"); 
		exit(0);
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convertendo endereco ip de texto para binario
	if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		exit(0);
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		exit(0); 
	} 
}

int main(int argc, char *argv[]) 
{ 
	int** tabuleiro = criaMatriz(10,10);
	int** meusTiros = criaMatriz(10,10);

	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			tabuleiro[i][j] = 8;
			meusTiros[i][j] = 8;
		}
	}

	int x, y = 0; //coordenadas do tiro
	//int x_tiro, y_tiro;
	int c;
	//int me_acertou = 0;
	int acertei;
	char aux, auy;

	while((c = getopt(argc,argv,"i:p:a:")) != -1) {
		switch(c) {
			case 'i': 
				ip = optarg;
				break;
			case 'p':
				//PORT = str2int(optarg);
				break;
			case 'a':
				arq = optarg;
				break;
			case '?':
				if (optopt == 'i') {
					perror("\nIP não especificado\n uso: cliente -i <ip> -p <porta> [-a <arquivo>]\n");
					return 1;
				}
				else if(optopt == 'p') {
					perror("\nPorta não especificada\n uso: cliente -i <ip> -p <porta> [-a <arquivo>]\n");
					return 1;
				}
				else if(optopt == 'a') {
					perror("\nArquivo não especificado\n uso: cliente -i <ip> -p <porta> [-a <arquivo>]\n");
					return 1;
				}
			default :
				printf("\nParametros não especificados, usando padrão\nuso: cliente -i <ip> -p <porta> [-a <arquivo>]\n");
		}
	}

	init_socket();
	
	//inicia o tabuleiro do cliente
	layout(tabuleiro, arq);

inicio:
	//jogada do cliente
	printf("\nDigite a linha do tabuleiro que deseja atacar (a-j ou A-J) ou P para visualizar os tabuleiros: ");
	x = getchar();
	if (x == 'p' || x == 'P'){
		teclaP(tabuleiro, meusTiros);
		limparBuffer();
		goto inicio;
	} else if ((x > 96 && x < 107) || (x > 64 && x < 74)){
		aux = x; //guarda a linha como char (a-j)
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
	auy = y;  //guarda a linha como char ('0'-'9')
	if (y < 48 || y > 57) {
		printf("\nColuna inválida!\n");
		limparBuffer();
		goto repete;
	} else {
		y = numeroColuna(y);
		limparBuffer();
	}	

	//cria uma string a ser enviada pelo socket
	snprintf(ataque, 8, "%c %c 0", aux, auy);

	//cliente ataca servidor
	write(sock, ataque, 3);
	
	valread = read( sock, buffer, 1024);
	if ( ! valread) {
		printf("\nErro de comunicacao no socket\n");
		exit(EXIT_FAILURE);
	}

	while(strcmp(buffer,PERDI)) {
		//servidor ataca cliente
		x = numeroColuna(buffer[0]);
		y = numeroColuna(buffer[2]);
		acertei = numeroColuna(buffer[4]);
		//printf("\nAcertei no servidor: %d\n", acertei);

		//marca na matriz de ataque onde atacou 1 para navio 0 para agua
		if(acertei == 1)
			meusTiros[numeroLinha(aux)][numeroColuna(auy)] = 1;
		else
			meusTiros[numeroLinha(aux)][numeroColuna(auy)] = 0;

		printf("\nServidor atacou a posição %c%d\n", buffer[0]+49, y);
		//limparBuffer();

		//Jogada do servidor
		int tiro = jogada(x, y, tabuleiro);
		if (tiro) {
			printf("\nServidor acertou!\n");
		}
		else if (!tiro){
			printf("\nServidor errou!\n");
		}
		if(vazio(tabuleiro)) {
			write(sock, PERDI, strlen(PERDI)); 
			printf("\nServidor Ganhou!\n");
			exit(0);
		}

		printf("\nDigite a linha do tabuleiro que deseja atacar (a-j ou A-J) ou P para visualizar os tabuleiros: ");
		x = getchar();
		aux = x;
		while (x == 80 || x == 112) {
			teclaP(tabuleiro, meusTiros);
			limparBuffer();
			printf("\nDigite a linha do tabuleiro que deseja atacar (a-j ou A-J) ou P para visualizar os tabuleiros: ");
			x = getchar();
		}
		limparBuffer();
		printf("\nDigite a coluna do tabuleiro que deseja atacar (0-9): ");
		y = getchar();
		auy = y;
		limparBuffer();

		//x_tiro = x;
		//y_tiro = y;

		//cria uma string a ser enviada pelo socket
		snprintf(ataque, 8, "%c %c %c", x, y, tiro);

		//cliente ataca servidor
		write(sock, ataque, 8);

		//servidor ataca cliente
		valread = read( sock, buffer, 1024);
		if ( ! valread) {
			printf("\nErro de comunicacao no socket\n");
			exit(EXIT_FAILURE);
		}
	}
	return 0; 
} 