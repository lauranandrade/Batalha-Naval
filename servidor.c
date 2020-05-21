#include <time.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

#include "battleship.h"

int server_fd, new_socket, valread; 
struct sockaddr_in address; 
int addrlen = sizeof(address); 
int PORT = 8000;
int opt = 1;

void init_socket()
{
	// criando socket ipv4, tcp, ip
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("Socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	puts("Socket criado");
	puts("");
	
	// setando o socket na porta PORT
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	{ 
		perror("Setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	puts("Socket setado");
	puts("");
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// setando o socket na porta PORT
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("Bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	puts("Bind feito");
	puts("");

	if (listen(server_fd, 3) < 0) 
	{ 
		perror("Listen"); 
		exit(EXIT_FAILURE); 
	} 

	puts("Aguardando conexão");
	puts("");
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
	{ 
		perror("Accept"); 
		exit(EXIT_FAILURE); 
	} 
	puts("Conectado");
	puts("");
}

int main(int argc, char *argv[]) 
{ 
	//cria tabuleiro do servidor
	int** tabuleiroOponente = criaMatriz(10,10);

	//inicializa o tabuleiro com 8 em todas as posições
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			tabuleiroOponente[i][j] = 8;
		}
	}

	char ataque[3];
	int x, y = 0;
	int x_tiro, y_tiro;
	int c;
	int acertei_cliente;
	int filax[8] = {-1};
	int filay[8] = {-1};
	char aux, auy; //guarda a linha do tabuleiro escolhida
	char buffer[1024] = {0}; 

	srand(time(NULL));
	criaOponente(tabuleiroOponente);

	while((c = getopt(argc,argv,"p:")) != -1) {
		switch(c) {
			case 'p':
				PORT = str2int(optarg);
				break;
			case '?':
				if(optopt == 'p') {
					puts("\nPorta não específicada\n Uso: servidor -p <porta>\n");
					puts("");
					return 1;
				}
				break;
			default:
					puts("\nParametros não específicados\n Usando porta padrão.\nUso: servidor -p <porta>\n");
					puts("");
		}
	}

	init_socket();

	valread = read(new_socket, buffer, 1024); 
	if ( ! valread) {
		printf("\nErro de comunicacao no socket\n");
		exit(EXIT_FAILURE);
	}

	while(strcmp(buffer, PERDI)) { //compara duas strings
		//cliente ataca servidor
		aux = buffer[0];
		x = numeroLinha(buffer[0]);
		auy = buffer[2];
		y = numeroColuna(buffer[2]);
		acertei_cliente = numeroColuna(buffer[4]);
		
		//se o servidor acertou o cliente
		//insere os pontos vizinhos a uma fila
		if (acertei_cliente == 1) {
			int i = 0;
			for(;i<8 && filax[i] == -1;++i);
			if (i<7) { 
				filax[i] = x+1;
				filax[i+1] = x-1;
			}else if(i<8) {
				filax[i] = x+1;
			}
			i=0;
			for(;i<8 && filay[i] == -1;++i);
			if (i<7) { 
				filay[i] = y+1;
				filay[i+1] = y-1;
			}else if(i<8) {
				filay[i] = y+1;
			}
		}

		char at_cli[256];
		snprintf(at_cli, 256, "\nCliente atacou a posição %c%c", aux, auy);
		puts(at_cli);

		int tiro = jogada(x, y, tabuleiroOponente);
		if (tiro){
			printf("\nCliente acertou!\n");
		}
		else if (!tiro){
			printf("\nCliente errou!\n");
		}

		if(vazio(tabuleiroOponente)){
			write(new_socket , PERDI , strlen(PERDI)); 
			printf("\nCliente Ganhou!\n");
			exit(0);
		}

		//pega um ponto aleatorio ou escolhe um ponto da fila
		if (filax[0] != -1) {
			x_tiro = filax[0];
			//anda a fila
			for(int i=0; i<7 && filax[i+1] != -1; i++) {
				filax[i] = filax[i+1];
				filax[i+1] = -1;
			}

		}else{
			x_tiro = rand() % 10;
		}
		if (filay[0] != -1) {
			y_tiro = filay[0];
			//anda a fila
			for(int i=0; i<7 && filay[i+1] != -1; i++) {
				filay[i] = filay[i+1];
				filay[i+1] = -1;
			}
		}else{
			y_tiro = rand() % 10;
		}

		//cria uma string a ser enviada pelo socket
		//ultimo caractere indica se o jogador me acertou na ultima jogada
		snprintf(ataque, 8, "%d %d %d", x_tiro, y_tiro, tiro);

		//servidor ataca cliente
		write(new_socket, ataque, strlen(ataque));

		//cliente ataca servidor
		valread = read(new_socket, buffer, 1024);
		if ( ! valread) {
			printf("\nErro de comunicacao no socket\n");
			exit(EXIT_FAILURE);
		}
	}
	return 0; 
} 