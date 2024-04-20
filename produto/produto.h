#ifndef PRODUTO_H
#define PRODUTO_H

#define ORDEM 3
#define MAX_NOME 50
#define MAX_MARCA 30
#define MAX_CATEGORIA 50

#include "../file/file.h"

// Estrutura para n� da �rvore B*
typedef struct indice_node {
    int chave[ORDEM];   // Armazena as chaves (c�digos) da �rvore, al�m disso a posi��o 0 das chaves ser� utilizada para encadear os n�s livres
    int pt_dados[ORDEM]; // Posi��es dos registros de dados no arquivo
    int filho[ORDEM+1]; // Ponteiros para os filhos
    int num_chaves;     // N�mero de chaves presentes no n�
    int pos_file; // posi��o da chave no arquivo, a fim de facilitar a busca do mesmo
    int pos_pai; // posi��o do pai do n� no arquivo, afim de facilitar opera��es de inser��o, remo��o e balanceamento da �rvore B
} ARVOREB;

// Estrutura para os dados do produto
typedef struct produto_data {
    int codigo;
    char nome[MAX_NOME + 1];      // Nome do produto
    char marca[MAX_MARCA + 1];    // Marca do produto
    char categoria[MAX_CATEGORIA + 1]; // Categoria do produto
    int estoque;                  // Quantidade em estoque
    float preco;                  // Pre�o do produto
} PRODUTO_REGISTRO;

#endif //PRODUTO_H
