#ifndef PRODUTO_IMPRESSAO_H
#define PRODUTO_IMPRESSAO_H

#include "produto.h"
#include "file.h"

// Defini��o da estrutura de n� da lista encadeada
struct noLista {
    int info;              // Informa��o armazenada no n�
    struct noLista* prox;  // Ponteiro para o pr�ximo n� na lista
};

// Defini��o de um alias para a estrutura de n� da lista encadeada
typedef struct noLista noL;

// Defini��o da estrutura de n� da fila, que utiliza a lista encadeada
typedef struct noFila {
    struct noLista* inicio;  // Ponteiro para o primeiro n� da fila
    struct noLista* fim;     // Ponteiro para o �ltimo n� da fila
} Fila;

// Fun��o para imprimir a lista de produtos armazenados na �rvore B-B
// Pr�-condi��es: Todos os arquivos e n�s devem estar acess�veis e corretamente inicializados.
//                A posi��o atual na �rvore deve ser v�lida.
// P�s-condi��es: A lista de produtos � impressa na sa�da padr�o, seguindo a estrutura definida.
void imprimir_lista_produtos(ARQUIVOS files, int pos_atual);

// Fun��o para imprimir as informa��es de um produto espec�fico na �rvore B
// Pr�-condi��es: Todos os arquivos e n�s devem estar acess�veis e corretamente inicializados.
//                O c�digo do produto a ser buscado deve ser v�lido e existir na �rvore.
// P�s-condi��es: As informa��es do produto s�o impressas na sa�da padr�o, seguindo a estrutura definida.
void imprimir_informacoes_produto(ARQUIVOS files, int codido);

// Fun��o para imprimir a lista de �ndices livres no arquivo de �ndices
// Pr�-condi��es: O arquivo de �ndices deve estar acess�vel e corretamente inicializado.
//                A posi��o atual na lista de �ndices livres deve ser v�lida.
// P�s-condi��es: A lista de posi��es livres no arquivo de �ndices � impressa na sa�da padr�o.
void imprimir_lista_indices_livres(FILE * file_indices, int pos_atual);

// Fun��o para imprimir a lista de registros livres no arquivo de dados
// Pr�-condi��es: O arquivo de dados deve estar acess�vel e corretamente inicializado.
//                A posi��o atual na lista de registros livres deve ser v�lida.
// P�s-condi��es: A lista de posi��es livres no arquivo de dados � impressa na sa�da padr�o.
void imprimir_lista_registros_livres(FILE * file_dados, int pos_atual);

// Fun��o para buscar o n�vel de um determinado n� na �rvore B-B
// Pr�-condi��es: O arquivo de �ndices deve estar acess�vel e corretamente inicializado.
//                O n� e a chave a serem buscados devem ser v�lidos.
// P�s-condi��es: O n�vel do n� na �rvore � retornado.
int buscaNivel(FILE* arq, ARVOREB* r, int chave, int count);

// Fun��o para buscar um n� espec�fico na �rvore B
// Pr�-condi��es: O arquivo de �ndices deve estar acess�vel e corretamente inicializado.
//                O n� e a chave a serem buscados devem ser v�lidos.
// P�s-condi��es: O n� contendo a chave � retornado, ou NULL se a chave n�o for encontrada.
ARVOREB * busca(FILE* arq, ARVOREB* r, int info, int * pos);

// Fun��o para verificar se a fila est� vazia
// Pr�-condi��es: A fila deve estar corretamente inicializada.
// P�s-condi��es: Retorna 1 se a fila estiver vazia, caso contr�rio retorna 0.
int vaziaFila(Fila* f);

// Fun��o para enfileirar um elemento na fila
// Pr�-condi��es: A fila deve estar corretamente inicializada.
// P�s-condi��es: O elemento � enfileirado na fila.
void enqueue(Fila* f, int x);

// Fun��o para enfileirar os filhos de um n� na fila
// Pr�-condi��es: O arquivo de �ndices e o n� devem estar acess�veis e corretamente inicializados.
//                A fila deve estar corretamente inicializada.
// P�s-condi��es: Os filhos do n� s�o enfileirados na fila.
void enfileiraFilhos(FILE* arq, ARVOREB* r, Fila* f);

// Fun��o para desenfileirar um elemento da fila
// Pr�-condi��es: A fila deve estar corretamente inicializada e n�o vazia.
// P�s-condi��es: O elemento � desenfileirado da fila e retornado.
int * dequeue(Fila* f);

// Fun��o para criar uma fila vazia
// Pr�-condi��es: Nenhuma.
// P�s-condi��es: Retorna a fila vazia.
Fila * cria_fila_vazia();

// Fun��o para imprimir a �rvore B-B por n�veis
// Pr�-condi��es: Todos os arquivos e n�s devem estar acess�veis e corretamente inicializados.
//                O n� raiz da �rvore deve ser v�lido.
// P�s-condi��es: A �rvore B-B � impressa por n�veis na sa�da padr�o.
void imprimir_por_niveis(ARQUIVOS files, ARVOREB* r);

// Fun��o para imprimir a �rvore B-B
// Pr�-condi��es: O arquivo de �ndices deve estar acess�vel e corretamente inicializado.
// P�s-condi��es: A �rvore B-B � impressa na sa�da padr�o.
void imprimir_arvore(ARQUIVOS files);

#endif //PRODUTO_IMPRESSAO_H