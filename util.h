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