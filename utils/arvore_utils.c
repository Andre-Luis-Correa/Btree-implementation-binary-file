//
// Created by andre on 29/04/2024.
//

#include "arvore_utils.h"
#include "../cabecalho/cabecalho_indices.h"
#include <stdio.h>
#include <stdlib.h>

//verifica se um n�(lido) � nulo(arvore vazia)
//pre-condi�ao: n� lido
//pos-condi�ao: 0 se nao for vazia, !=0 se vazia
int vazia(ARVOREB * r) {
    return (r == NULL);
}

//verifica se o no e folha
//pr�-condi�ao: no nao nulo
//pos-conid�ao: 1 = sim, 0 = nao
int eh_folha(ARVOREB * r) {
    return r->filho[0] == -1;
}

//testa se � raiz
//pre-condi��o: r nao pode ser NULL
//pos-condi��o: 1 se for raiz, 0 se nao for
//int eh_raiz(ARQUIVOS files, ARVOREB * r) {
//    if (r != NULL) {
//        int pos_pai = buscar_pai(files.file_indices, int pos_raiz, int codigo, int * pos_filho_remocao);
//
//        if ( buscar_pai(ARQUIVOS files, int pos_raiz, int codigo, int * pos_filho_remocao) == -1) {
//            return 1;
//        }
//    }
//    return 0;
//}

//verifica se deu underflow
//pre-condi�ao: arvore nao nula
//pos-conidi��o: 1 se deu, 0 se nao deu, underflow
//int underflow(ARVOREB * r) {
//    if (r != NULL) {
//        if (r->num_chaves < MIN && !eh_raiz(r) )
//            return 1;
//        return 0;
//    }
//    return 0;
//}

int busca_pos_chave(ARVOREB * r, int codigo){
    int pos_codigo;

    // Encontra a pos dentro do n�
    for(pos_codigo = 0; pos_codigo < r->num_chaves; pos_codigo++){
        if( r->chave[pos_codigo] == codigo)
            return pos_codigo;
    }

    return -1;
}

int buscar_pai(FILE * file, int codigo){
    CABECALHO_INDICES * cab = le_cabecalho_indices(file);
    int pos_raiz = cab->pos_raiz;
    free(cab);

    ARVOREB * r = ler_no(file, pos_raiz);

    if ( eh_folha(r) || busca_pos_chave(r, codigo) != -1) { // A propria raiz � o pai, pois nao tem pai acima
        free(r);
        return -1; // Retorna -1 indicando que n�o foi encontrado
    }

    free(r);
    return buscar_pai_aux(file, pos_raiz, codigo);
}

int buscar_pai_aux(FILE * file_indices, int pos_raiz, int codigo){
    ARVOREB * r = ler_no(file_indices, pos_raiz);

    int i;
    for (i = 0; i < r->num_chaves; i++) {
        // Se o c�digo for menor que a chave atual, desce para o filho � esquerda
        if (codigo < r->chave[i]) {
                ARVOREB * filho = ler_no(file_indices, r->filho[i]);
                int pos_codigo = busca_pos_chave(filho, codigo);
                free(filho);
                // Se a chave for encontrada no filho, retorna o n� atual como pai
                if (pos_codigo != -1) {
                    return pos_raiz;
                }
                // Se n�o, continua a busca descendente
                return buscar_pai_aux(file_indices, r->filho[i], codigo);
            }
        }

    // Se o c�digo for maior que todas as chaves, desce para o �ltimo filho
    ARVOREB * filho = ler_no(file_indices, r->filho[i]);
    int pos_codigo = busca_pos_chave(filho, codigo);
    free(filho);

    // Se a chave for encontrada no �ltimo filho, retorna o n� atual como pai
    if (pos_codigo != -1) {
        return pos_raiz;
    }

    // Se n�o, continua a busca descendente
    return buscar_pai_aux(file_indices, r->filho[i], codigo);
}
