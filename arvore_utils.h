//
// Created by andre on 29/04/2024.
//

#ifndef TRABALHO_AED_3_BRUNA_ANDRE_ARVORE_UTILS_H
#define TRABALHO_AED_3_BRUNA_ANDRE_ARVORE_UTILS_H

#include "produto.h"
#include "file.h"

// Verifica se uma �rvore B est� vazia
// Pr�-condi��o: nenhuma
// P�s-condi��o: retorna 1 se a �rvore estiver vazia, 0 caso contr�rio
int vazia(ARVOREB * r);

// Verifica se um n� da �rvore B � folha
// Pr�-condi��o: o n� deve existir
// P�s-condi��o: retorna 1 se o n� for uma folha, 0 caso contr�rio
int eh_folha(ARVOREB * r);

// Verifica se um n� � a raiz da �rvore B
// Pr�-condi��o: o arquivo de �ndices deve ser v�lido e a posi��o do n� deve ser v�lida
// P�s-condi��o: retorna 1 se o n� for a raiz, 0 caso contr�rio
int eh_raiz(FILE * file_indices, int pos);

// Busca a posi��o de uma chave em um n� da �rvore B
// Pr�-condi��o: o n� deve existir
// P�s-condi��o: retorna a posi��o da chave se encontrada, -1 caso contr�rio
int buscar_pos_chave(ARVOREB * r, int codigo);

// Busca a posi��o de um filho em um n� da �rvore B
// Pr�-condi��o: o n� deve existir
// P�s-condi��o: retorna a posi��o do filho se encontrado, -1 caso contr�rio
int buscar_pos_filho(ARVOREB * r, int pos);

// Verifica se um n� possui mais chaves que o m�nimo exigido
// Pr�-condi��o: o n� deve existir
// P�s-condi��o: retorna 1 se o n�mero de chaves for maior que o m�nimo, 0 caso contr�rio
int mais_chaves_que_min(ARVOREB * r);

// Fun��o para buscar um n� na �rvore B a partir de um c�digo de produto
// Pr�-condi��o: o arquivo de �ndices deve ser v�lido
// P�s-condi��o: retorna a posi��o do n� se encontrado, -1 caso contr�rio
int buscar_no(FILE * file_indices, int codigo);

// Fun��o auxiliar para buscar um n� na �rvore B a partir de um c�digo de produto
// Pr�-condi��o: o arquivo de �ndices deve ser v�lido e a posi��o do n� atual deve ser v�lida
// P�s-condi��o: retorna a posi��o do n� se encontrado, -1 caso contr�rio
int buscar_no_aux(FILE * file_indices, int codigo, int pos);

// Fun��o para buscar o pai de um n� na �rvore B a partir de um c�digo de produto
// Pr�-condi��o: o arquivo de �ndices deve ser v�lido
// P�s-condi��o: retorna a posi��o do pai se encontrado, -1 caso contr�rio
int buscar_pai(FILE * file_indices, int codigo);

// Fun��o auxiliar para buscar o pai de um n� na �rvore B a partir de um c�digo de produto
// Pr�-condi��o: o arquivo de �ndices deve ser v�lido e a posi��o do n� atual deve ser v�lida
// P�s-condi��o: retorna a posi��o do pai se encontrado, -1 caso contr�rio
int buscar_pai_aux(FILE * file_indices, int pos_raiz, int codigo);

// Fun��o para buscar o pai de um n� na �rvore B a partir de uma posi��o de n�
// Pr�-condi��o: o arquivo de �ndices deve ser v�lido
// P�s-condi��o: retorna a posi��o do pai se encontrado, -1 caso contr�rio
int buscar_pai_by_pos(FILE * file_indices, int pos_pai);

// Fun��o auxiliar para buscar o pai de um n� na �rvore B a partir de uma posi��o de n�
// Pr�-condi��o: o arquivo de �ndices deve ser v�lido e a posi��o do n� atual deve ser v�lida
// P�s-condi��o: retorna a posi��o do pai se encontrado, -1 caso contr�rio
int buscar_pai_by_pos_aux(FILE * file_indices, int pos_raiz, int pos_pai);

#endif //TRABALHO_AED_3_BRUNA_ANDRE_ARVORE_UTILS_H
