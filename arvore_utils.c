#include "arvore_utils.h"
#include "cabecalho_indices.h"
#include <stdio.h>
#include <stdlib.h>

// Verifica se uma �rvore B est� vazia
// Pr�-condi��o: nenhuma
// P�s-condi��o: retorna 1 se a �rvore estiver vazia, 0 caso contr�rio
int vazia(ARVOREB * r) {
    return (r == NULL);
}

// Verifica se um n� da �rvore B � folha
// Pr�-condi��o: o n� deve existir
// P�s-condi��o: retorna 1 se o n� for uma folha, 0 caso contr�rio
int eh_folha(ARVOREB * r) {
    return r->filho[0] == -1;
}

// Verifica se um n� � a raiz da �rvore B
// Pr�-condi��o: o arquivo de �ndices deve ser v�lido e a posi��o do n� deve ser v�lida
// P�s-condi��o: retorna 1 se o n� for a raiz, 0 caso contr�rio
int eh_raiz(FILE * file_indices, int pos) {
    CABECALHO_INDICES * cab = le_cabecalho_indices(file_indices);

    int pos_pai = buscar_pai_by_pos(file_indices, pos);

    if ( pos_pai == -1) {
        free(cab);
        return 1;
    }

    free(cab);
    return 0;
}

// Busca a posi��o de uma chave em um n� da �rvore B
// Pr�-condi��o: o n� deve existir
// P�s-condi��o: retorna a posi��o da chave se encontrada, -1 caso contr�rio
int buscar_pos_chave(ARVOREB * r, int codigo){
    int pos_codigo;

    // Encontra a pos dentro do n�
    for(pos_codigo = 0; pos_codigo < r->num_chaves; pos_codigo++){
        if( r->chave[pos_codigo] == codigo)
            return pos_codigo;
    }

    return -1;
}

// Busca a posi��o de um filho em um n� da �rvore B
// Pr�-condi��o: o n� deve existir
// P�s-condi��o: retorna a posi��o do filho se encontrado, -1 caso contr�rio
int buscar_pos_filho(ARVOREB * r, int pos){
    int filho;

    // Encontra a pos dentro do n�
    for(filho = 0; filho <= r->num_chaves; filho++){
        if(r->filho[filho] == pos)
            return filho;
    }

    return -1;
}

// Verifica se um n� possui mais chaves que o m�nimo exigido
// Pr�-condi��o: o n� deve existir
// P�s-condi��o: retorna 1 se o n�mero de chaves for maior que o m�nimo, 0 caso contr�rio
int mais_chaves_que_min(ARVOREB * r){
    return r->num_chaves > MIN;
}

// Fun��o para buscar um n� na �rvore B a partir de um c�digo de produto
// Pr�-condi��o: o arquivo de �ndices deve ser v�lido
// P�s-condi��o: retorna a posi��o do n� se encontrado, -1 caso contr�rio
int buscar_no(FILE * file_indices, int codigo){
    CABECALHO_INDICES * cab = le_cabecalho_indices(file_indices);
    int pos_raiz = cab->pos_raiz;
    free(cab);
    printf("\n---> pos raiz = %d", pos_raiz);
    if(pos_raiz == -1){
        printf("\n---> a funcao buscra no retornour -1 pos_raiz == -1");
        return -1;
    }

    return buscar_no_aux(file_indices, codigo, pos_raiz);
}

// Fun��o auxiliar para buscar um n� na �rvore B a partir de um c�digo de produto
// Pr�-condi��o: o arquivo de �ndices deve ser v�lido e a posi��o do n� atual deve ser v�lida
// P�s-condi��o: retorna a posi��o do n� se encontrado, -1 caso contr�rio
int buscar_no_aux(FILE * file_indices, int codigo, int pos){
    if(pos == -1){
        printf("\n---> a funcao buscra no retornour -1");
        return -1;
    }

    ARVOREB * r = ler_no(file_indices, pos);
    int i;

    for(i = 0; i < r->num_chaves; i++){
        if(r->chave[i] == codigo ){
            free(r);
            return pos;
        } else if (r->chave[i] > codigo ){
            int pos_atual = r->filho[i];
            free(r);
            return buscar_no_aux(file_indices, codigo, pos_atual);
        }
    }

    int pos_atual = r->filho[i];
    free(r);

    return buscar_no_aux(file_indices, codigo, pos_atual);
}

// Fun��o para buscar o pai de um n� na �rvore B a partir de um c�digo de produto
// Pr�-condi��o: o arquivo de �ndices deve ser v�lido
// P�s-condi��o: retorna a posi��o do pai se encontrado, -1 caso contr�rio
int buscar_pai(FILE * file_indices, int codigo){
    CABECALHO_INDICES * cab_indices = le_cabecalho_indices(file_indices);
    int pos_raiz = cab_indices->pos_raiz;
    free(cab_indices);

    ARVOREB * r = ler_no(file_indices, pos_raiz);

    if (eh_folha(r) || buscar_pos_chave(r, codigo) != -1) { // A propria raiz � o pai, pois nao tem pai acima
        free(r);
        return -1; // Retorna -1 indicando que n�o foi encontrado
    }

    free(r);
    return buscar_pai_aux(file_indices, pos_raiz, codigo);
}

// Fun��o auxiliar para buscar o pai de um n� na �rvore B a partir de um c�digo de produto
// Pr�-condi��o: o arquivo de �ndices deve ser v�lido e a posi��o do n� atual deve ser v�lida
// P�s-condi��o: retorna a posi��o do pai se encontrado, -1 caso contr�rio
int buscar_pai_aux(FILE * file_indices, int pos_raiz, int codigo){
    ARVOREB * r = ler_no(file_indices, pos_raiz);

    int i;
    for (i = 0; i < r->num_chaves; i++) {
        // Se o c�digo for menor que a chave atual, desce para o filho � esquerda
        if (codigo < r->chave[i]) {
                ARVOREB * filho = ler_no(file_indices, r->filho[i]);
                int pos_codigo = buscar_pos_chave(filho, codigo);
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
    int pos_codigo = buscar_pos_chave(filho, codigo);
    free(filho);

    // Se a chave for encontrada no �ltimo filho, retorna o n� atual como pai
    if (pos_codigo != -1) {
        return pos_raiz;
    }

    // Se n�o, continua a busca descendente
    return buscar_pai_aux(file_indices, r->filho[i], codigo);
}

// Fun��o para buscar o pai de um n� na �rvore B a partir de uma posi��o de n�
// Pr�-condi��o: o arquivo de �ndices deve ser v�lido
// P�s-condi��o: retorna a posi��o do pai se encontrado, -1 caso contr�rio
int buscar_pai_by_pos(FILE * file_indices, int pos){
    CABECALHO_INDICES * cab_indices = le_cabecalho_indices(file_indices);
    int pos_raiz = cab_indices->pos_raiz;
    free(cab_indices);

    ARVOREB * r = ler_no(file_indices, pos_raiz);

    if (eh_folha(r) || (pos == pos_raiz) ) { // A propria raiz � o pai, pois nao tem pai acima
        free(r);
        return -1; // Retorna -1 indicando que n�o foi encontrado
    }

    free(r);
    printf("\n---> indo para recursao encontrar pai filho");
    return buscar_pai_by_pos_aux(file_indices, pos_raiz, pos);

}

// Fun��o auxiliar para buscar o pai de um n� na �rvore B a partir de uma posi��o de n�
// Pr�-condi��o: o arquivo de �ndices deve ser v�lido e a posi��o do n� atual deve ser v�lida
// P�s-condi��o: retorna a posi��o do pai se encontrado, -1 caso contr�rio
int buscar_pai_by_pos_aux(FILE * file_indices, int pos_raiz, int pos) {
    if( pos_raiz != -1 ) {

        ARVOREB * r = ler_no(file_indices, pos_raiz);

        int i, pai = -1; // Inicialize pai com -1 para indicar que n�o foi encontrado

        // Verifica se a posi��o est� entre os filhos do n� atual
        for (i = 0; i <= r->num_chaves; i++) {
            if (r->filho[i] == pos) {
                free(r);
                return pos_raiz;
            }
        }

        // Caso a posi��o n�o seja um dos filhos, procuramos recursivamente nos filhos
        for (i = 0; i <= r->num_chaves; i++) { // Modificado para <=
            pai = buscar_pai_by_pos_aux(file_indices, r->filho[i], pos);
            if (pai != -1) // Se encontrou o pai, encerra o loop
                break;
        }

        free(r);
        return pai;
    }

    return -1;
}