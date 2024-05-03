#ifndef PRODUTO_REMOCAO_H
#define PRODUTO_REMOCAO_H

#include "produto.h"
#include "produto_insercao.h"
#include "produto_impressao.h"

// Fun��o para remover um produto da �rvore B
// Pr�-condi��es: ARQUIVOS files cont�m os arquivos necess�rios para opera��es de leitura e escrita,
//                 incluindo os arquivos de �ndices e de dados.
// P�s-condi��es: O produto com o c�digo especificado � removido da �rvore B, se presente. Caso contr�rio,
//                 mensagens apropriadas s�o exibidas.
void remover_produto(ARQUIVOS files);

// Fun��o para atualizar a lista de posi��es livres no arquivo de dados ap�s a remo��o de um registro
// Pr�-condi��es: O arquivo de dados deve estar aberto e a posi��o do registro removido deve ser v�lida.
// P�s-condi��es: A lista de posi��es livres no arquivo de dados � atualizada.
void atualizar_pos_livres_dados(FILE * file_dados, int pos);

// Fun��o para atualizar a lista de posi��es livres no arquivo de �ndices ap�s a remo��o de um n�
// Pr�-condi��es: O arquivo de �ndices deve estar aberto e a posi��o do n� removido deve ser v�lida.
// P�s-condi��es: A lista de posi��es livres no arquivo de �ndices � atualizada.
void atualizar_pos_livres_indices(FILE * file_indices, int pos);

// Fun��o para atualizar um n� ap�s a remo��o de um registro em um n� folha
// Pr�-condi��es: O arquivo de �ndices deve estar aberto e o n� folha, bem como as posi��es de remo��o e c�digo, devem ser v�lidos.
// P�s-condi��es: O n� � atualizado com as informa��es da remo��o.
void atualiza_no_remocao_folha(ARQUIVOS files, ARVOREB * folha, int pos_remocao, int pos_codigo);

// Fun��o para tratar a remo��o quando esta � feita em um n� folha com n�mero de chaves maior que o m�nimo (ORDEM/2 + ORDEM%2 - 1)
// Pr�-condi��es: O arquivo de �ndices deve estar aberto e o n� a ser removido, bem como o c�digo e a posi��o de remo��o, devem ser v�lidos.
// P�s-condi��es: A remo��o � tratada de acordo com o caso espec�fico.
void remover_caso1(ARQUIVOS files, ARVOREB * no_a_remover, int codigo, int pos_remocao);

// Fun��o para buscar a chave sucessora em um n� folha
// Pr�-condi��es: O arquivo de �ndices deve estar aberto e o n� a ser removido, bem como a posi��o da chave e a posi��o do n� sucessor, devem ser v�lidos.
// P�s-condi��es: A chave sucessora � retornada e a posi��o do n� sucessor � atualizada.
int buscar_chave_sucessora_folha(ARQUIVOS files, ARVOREB * no_a_remover, int pos_codigo, int * pos_no_sucessor);

// Fun��o para tratar a remo��o quando esta � feita em um n� interno
// Pr�-condi��es: O arquivo de �ndices deve estar aberto e o n� a ser removido, bem como o c�digo e a posi��o de remo��o, devem ser v�lidos.
// P�s-condi��es: A remo��o � tratada de acordo com o caso espec�fico.
int remover_caso2(ARQUIVOS files, ARVOREB * no_a_remover, int codigo, int pos_remocao);

// Fun��o para verificar se � poss�vel realizar a redistribui��o de chaves entre irm�os
// Pr�-condi��es: O arquivo de �ndices deve estar aberto, a posi��o do pai, o �ndice do filho, bem como os ponteiros para as posi��es dos filhos esquerdo e direito, devem ser v�lidos.
// P�s-condi��es: Retorna 1 se a redistribui��o for poss�vel, caso contr�rio, retorna 0.
int pode_redistribuir(ARQUIVOS files, int pos_pai, int indice_filho, int * pegar_esq, int * pegar_dir);

// Fun��o para redistribuir chaves a partir do irm�o direito
// Pr�-condi��es: O arquivo de �ndices deve estar aberto e as posi��es do pai, remo��o, filho da remo��o e irm�o direito devem ser v�lidos.
// P�s-condi��es: As chaves s�o redistribu�das entre o n� e seu irm�o direito.
void redistribuir_partir_da_direita(ARQUIVOS files, int pos_pai, int pos_remocao, int pos_filho_remocao, int pos_dir);

// Fun��o para redistribuir chaves a partir do irm�o esquerdo
// Pr�-condi��es: O arquivo de �ndices deve estar aberto e as posi��es do pai, remo��o, filho da remo��o e irm�o esquerdo devem ser v�lidos.
// P�s-condi��es: As chaves s�o redistribu�das entre o n� e seu irm�o esquerdo.
void redistribuir_partir_da_esquerda(ARQUIVOS files, int pos_pai, int pos_remocao, int pos_filho_remocao, int pos_esq);

// Fun��o para realizar a redistribui��o de chaves entre irm�os
// Pr�-condi��es: O arquivo de �ndices deve estar aberto e as posi��es do pai, remo��o, filho da remo��o e irm�os devem ser v�lidos.
// P�s-condi��es: As chaves s�o redistribu�das entre os irm�os conforme necess�rio.
void redistribuir(ARQUIVOS files, int pos_pai, int pos_remocao, int pos_filho_remocao, int pos_esq, int pos_dir);

// Fun��o para buscar os filhos esquerdo e direito de um n� pai
// Pr�-condi��es: O arquivo de �ndices deve estar aberto e acess�vel. A posi��o do n� pai deve ser v�lida.
// P�s-condi��es: As posi��es dos filhos esquerdo e direito s�o retornadas por meio dos par�metros pos_esq e pos_dir, respectivamente.
void buscar_filhos_esq_dir(ARQUIVOS files, int pos_pai, int indice_remocao, int * pos_esq, int * pos_dir);

// Fun��o para concatenar o n� da esquerda com o n� a ser removido
// Pr�-condi��es: Todos os n�s e arquivos devem estar acess�veis e corretamente inicializados.
//                O n� a ser removido n�o deve ser a raiz e deve conter as chaves a serem concatenadas.
// P�s-condi��es: O n� da esquerda � concatenado com o n� a ser removido, mantendo a estrutura da �rvore B.
void concatenar_esquerda(ARQUIVOS files, ARVOREB * pai, ARVOREB * esq, ARVOREB * no_a_remover, int indice_filho);

// Fun��o para concatenar o n� da direita com o n� a ser removido
// Pr�-condi��es: Todos os n�s e arquivos devem estar acess�veis e corretamente inicializados.
//                O n� a ser removido n�o deve ser a raiz e deve conter as chaves a serem concatenadas.
// P�s-condi��es: O n� da direita � concatenado com o n� a ser removido, mantendo a estrutura da �rvore B.
void concatenar_direita(ARQUIVOS files, ARVOREB * pai, ARVOREB * dir, ARVOREB * no_a_remover, int pos_filho_remocao);

// Fun��o para realizar a concatena��o dos n�s
// Pr�-condi��es: Todos os n�s e arquivos devem estar acess�veis e corretamente inicializados.
//                As posi��es dos n�s pai, remo��o, filhos esquerdo e direito devem ser v�lidas.
// P�s-condi��es: Os n�s s�o concatenados conforme necess�rio, mantendo a estrutura da �rvore B.
void concatenar(ARQUIVOS files, int pos_pai, int pos_remocao, int indice_filho, int pos_esq, int pos_dir);

// Fun��o para balancear a �rvore ap�s uma remo��o
// Pr�-condi��es: Todos os n�s e arquivos devem estar acess�veis e corretamente inicializados.
//                A posi��o do pai, �ndice do filho removido e posi��o da remo��o devem ser v�lidos.
// P�s-condi��es: A �rvore � balanceada de acordo com as regras da �rvore B.
void balancear(ARQUIVOS files, int pos_pai, int indice_filho, int pos_remocao);

// Fun��o para verificar e atualizar o pai ap�s uma remo��o.
// Pr�-condi��es: Todos os n�s e arquivos devem estar acess�veis e corretamente inicializados.
//                A posi��o do pai a ser verificado deve ser v�lida.
// P�s-condi��es: O pai � atualizado conforme necess�rio, mantendo a integridade da �rvore B.
void verificar_pai(ARQUIVOS files, int pos_pai);

// Fun��o para remover uma chave da �rvore B-B
// Pr�-condi��es: Todos os n�s e arquivos devem estar acess�veis e corretamente inicializados.
//                O c�digo da chave a ser removida, a posi��o da raiz e a posi��o de remo��o devem ser v�lidos.
// P�s-condi��es: A chave � removida da �rvore B-B, mantendo sua estrutura e integridade.
void remover(ARQUIVOS files, int codigo, int pos_raiz, int pos_remocao);

#endif //PRODUTO_REMOCAO_H
