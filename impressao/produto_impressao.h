//
// Created by andre on 09/04/2024.
//

#ifndef PRODUTO_IMPRESSAO_H
#define PRODUTO_IMPRESSAO_H

#include "../produto/produto.h"

#define TAMANHO_FILA 100 // Ajuste o tamanho conforme necess�rio

typedef struct {
    ARVOREB *elementos[TAMANHO_FILA];
    int frente; // �ndice do primeiro elemento na fila
    int tras;   // �ndice do pr�ximo espa�o dispon�vel na fila
} Fila;

void imprimir_lista_produtos(ARQUIVOS files, int pos_atual);
void imprimir_informacoes_produto(ARQUIVOS files, int codido);
void imprimir_no(ARVOREB *r);
void imprimir_arvore(ARQUIVOS files);
void imprimir_info_nos_chaves(ARQUIVOS files, int pos);

#endif //PRODUTO_IMPRESSAO_H