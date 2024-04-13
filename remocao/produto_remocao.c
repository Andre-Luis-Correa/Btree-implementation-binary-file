//
// Created by andre on 08/04/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include "produto_remocao.h"

#define MIN (ORDEM/2 + ORDEM%2 - 1)

void remover_produto(ARQUIVOS files){
    CABECALHO_INDICES * cab_indices = le_cabecalho_indices(files.file_indices);

    int codigo, pos_codigo, pos_raiz, pos_esq, pos_dir;

    printf ("Insira o codigo a ser removido: ");
    scanf ("%d", &codigo);

    pos_codigo = buscar_no(files.file_indices, codigo, cab_indices->pos_raiz);

    if(cab_indices->pos_raiz == -1){
        printf("A arvore encontra-se vazia!\n");
        free(cab_indices);
        return;
    } else if (pos_codigo == -1) {
        printf("Produto nao encontrado na arvore!\n");
        free(cab_indices);
        return;
    }

    printf("---> Iniciando a remocao da chave %d!\n", codigo);
    ARVOREB * raiz = ler_no(files.file_indices, cab_indices->pos_raiz);
    remover(files, codigo, cab_indices->pos_raiz, pos_codigo);

    free(raiz);
    free(cab_indices);
}

int mais_chaves_que_min(ARVOREB * r){
    return r->num_chaves > MIN;
}

int busca_pos_chave(ARVOREB * r, int codigo){
    int pos_codigo;

    // Encontra a pos dentro do n�
    for(pos_codigo = 0; pos_codigo < r->num_chaves; pos_codigo++){
        if( r->chave[pos_codigo] == codigo)
            return pos_codigo;
    }

    return -1;
}

void atualiza_lista_dados_livres(ARQUIVOS files, CABECALHO_DADOS * cab_dados, int pos_registro){

    DADOS_REGISTRO * dados_atual = ler_registro(files.file_dados, cab_dados->pos_livre);

    int pos_atual = cab_dados->pos_livre;

    while(dados_atual->prox_livre != -1){
        pos_atual = dados_atual->prox_livre;
        dados_atual = ler_registro(files.file_dados, dados_atual->prox_livre);
    }

    dados_atual->prox_livre = pos_registro;
    escreve_registro(files.file_dados, dados_atual, pos_atual);
}

void atualiza_no_remocao_folha(ARQUIVOS files, ARVOREB * folha, int pos_remocao, int pos_codigo){
    CABECALHO_DADOS * cab_dados = le_cabecalho_dados(files.file_dados);

    // A posi��o dos dados no arq de dados deve ser atualizado
    // folha->pt_dados[pos_codigo] => posi��o dos dados no file de dados
    int pos_registro = folha->pt_dados[pos_codigo];

    if(cab_dados->pos_livre == -1){
        cab_dados->pos_livre = pos_registro;
        // Atualiza o cab_dados com a posi��o livre
        escreve_cabecalho_dados(files.file_dados, cab_dados);

    }else {
        atualiza_lista_dados_livres(files, cab_dados, pos_registro);
    }

    printf("\n ---> CASO 1 - ANTES ATT - REMOCAO: \n");
    imprimir_no(folha);

    // Agora realizar a atualiza��o do n� folha
    int i;
    for(i = pos_codigo; i < folha->num_chaves-1; i++){
        folha->chave[i] = folha->chave[i+1];
        folha->pt_dados[i] = folha->pt_dados[i+1];
        folha->filho[i] = folha->filho[i+1];
    }

    // Atualiza o �ltimo fiilho
    folha->filho[i] = folha->filho[folha->num_chaves];
    folha->num_chaves--;

    printf("\n ---> CASO 1 - DEPOIS ATT - REMOCAO: \n");
    imprimir_no(folha);

    escreve_no(files.file_indices, folha, pos_remocao);
    free(cab_dados);
}

// 1� CASO: a remo��o � feita em um n� folha com n�mero de chaves maior que o m�nimo (ORDEM/2)
void remover_caso1(ARQUIVOS files, ARVOREB * no_a_remover, int codigo, int pos_remocao){
    int pos_codigo, i;

    // Encontra a pos dentro do n�
    pos_codigo = busca_pos_chave(no_a_remover, codigo);
    atualiza_no_remocao_folha(files, no_a_remover, pos_remocao, pos_codigo);
}

int buscar_chave_sucessora_folha(ARQUIVOS files, ARVOREB * no_a_remover, int pos_codigo, int * pos_no_sucessor){
    int pos_chave_sucessora = no_a_remover->filho[pos_codigo + 1];
    int chave_sucessora;

    ARVOREB * no_sucessor = ler_no(files.file_indices, pos_chave_sucessora);

    if(eh_folha(no_sucessor)){
        *pos_no_sucessor = pos_chave_sucessora;
        chave_sucessora = no_sucessor->chave[0];
        printf("\n ----> Essa � a chave sucessora: %d\n", chave_sucessora);
        free(no_sucessor);
        return chave_sucessora;
    }

    return buscar_chave_sucessora_folha(files, no_sucessor, -1, pos_no_sucessor);
}

int remover_caso2(ARQUIVOS files, ARVOREB * no_a_remover, int codigo, int pos_remocao){
    // Primeiramente, busca a posi��o da chave (dentro do n�) a ser removida no n�
    int pos_codigo = busca_pos_chave(no_a_remover, codigo);
    int pos_no_sucessor;

    // Busca a chave sucessora para inseri-l� na pos_codigo encontrada
    // Tamb�m retorna a posi��o no arq de indices do n� sucessor
    int chave_sucessora = buscar_chave_sucessora_folha(files, no_a_remover, pos_codigo, &pos_no_sucessor);

    // Preciso atualizar a folha que continha a chave sucessora
    ARVOREB  * no_sucessor = ler_no(files.file_indices, pos_no_sucessor);
    atualiza_no_remocao_folha(files, no_sucessor, pos_no_sucessor, 0);

    // Realiza a substitui��o da chave removida pela chave sucessora
    no_a_remover->chave[pos_codigo] = chave_sucessora;
    escreve_no(files.file_indices, no_a_remover, pos_remocao);

    return pos_no_sucessor;
}

void remover(ARQUIVOS files, int codigo, int pos_raiz, int pos_remocao){
    CABECALHO_INDICES * cab_indices = le_cabecalho_indices(files.file_indices);

    // Buscar o n� que possui a chave a ser removida a ser removido
    ARVOREB * no_a_remover = ler_no(files.file_indices, pos_remocao);

    // 1� CASO: a remo��o � feita em um n� folha com n�mero de chaves maior que o m�nimo (ORDEM/2)
    if( mais_chaves_que_min(no_a_remover) && eh_folha(no_a_remover) ){
        // Logo, apenas remove a chave do n�, realizando as altera��es necess�rias e gravando novamente no arquivo
        remover_caso1(files, no_a_remover, codigo, pos_remocao);

    } else if ( !eh_folha(no_a_remover ) ) { // CASO 2�: a remo��o � feita em um n� interno
        // Logo, busca-se a chave sucessora e a insere no lugar da chave removida no n� interno
        // Al�m disso, a fun��o deve retornar a posi��o do n� chave sucessora, pois esse n� deve ser tratado ap�s remo��o
        int pos_no_sucessor = remover_caso2(files, no_a_remover, codigo, pos_remocao);
    }


    free(cab_indices);
}