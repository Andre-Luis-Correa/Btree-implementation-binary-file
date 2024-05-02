#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lote.h"

// Insere um lote de produtos no sistema
// Pr�-condi��es: Os arquivos de �ndices e de dados devem existir e estar formatados corretamente
// P�s-condi��es: Os produtos do lote s�o inseridos no sistema, caso n�o existam ainda
void inserir_lote(ARQUIVOS files, PRODUTO_DATA produto) {
    if(buscar_no(files.file_indices, produto.codigo) == -1){
        // Realizar a inser��o do n�, pois ainda n�o existe esse c�digo na �rvore
        printf("\nNova insercao!\n");
        cadastrar_produto_file(files, &produto);
    }else{
        printf("\nATENCAO: Codigo de produto ja existente!\n");
    }
}

// Atualiza o pre�o de um lote de produtos no sistema
// Pr�-condi��es: Os arquivos de �ndices e de dados devem existir e estar formatados corretamente
// P�s-condi��es: O pre�o dos produtos do lote s�o atualizados no sistema
void atualizar_preco_lote(ARQUIVOS files, int codigo, float preco){
    if(buscar_no(files.file_indices, codigo) == -1){
        printf("\n---> Nao foi possivel atualizar, pois o codigo do produto e inexistente!");
        return;
    }
    atualizar_preco_produto(files, codigo, preco);

}

// Atualiza o estoque de um lote de produtos no sistema
// Pr�-condi��es: Os arquivos de �ndices e de dados devem existir e estar formatados corretamente
// P�s-condi��es: O estoque dos produtos do lote s�o atualizados no sistema
void atualizar_estoque_lote(ARQUIVOS files, int codigo, int novo_estoque){
    if(buscar_no(files.file_indices, codigo) == -1){
        printf("\n---> Nao foi possivel atualizar, pois o codigo do produto e inexistente!");
        return;
    }

    atualizar_estoque_produto(files, codigo, novo_estoque);
}

// Remove um lote de produtos do sistema
// Pr�-condi��es: Os arquivos de �ndices e de dados devem existir e estar formatados corretamente
// P�s-condi��es: Os produtos do lote s�o removidos do sistema, caso existam
void remover_lote(ARQUIVOS files, int codigo) {
    CABECALHO_INDICES * cab = le_cabecalho_indices(files.file_indices);
    int pos_raiz = cab->pos_raiz, pos_no = buscar_no(files.file_indices, codigo);
    free(cab);

    if( pos_no == -1){
        printf("\n---> Nao foi possivel remover, pois o codigo do produto e inexistente!");
        return;
    }
    remover(files, codigo, pos_raiz, pos_no);
}

// Processa uma linha do arquivo contendo informa��es sobre um lote de produtos
// Pr�-condi��es: Os arquivos de �ndices e de dados devem existir e estar formatados corretamente
// P�s-condi��es: As opera��es correspondentes ao tipo de linha s�o realizadas no sistema
void processar_linha(ARQUIVOS files, char *linha) {
    char tipo;
    sscanf(linha, "%c", &tipo);

    if (tipo == 'I') {
        PRODUTO_DATA produto;

        char * comma_ptr = strchr(linha, ',');
        if( comma_ptr != NULL){
            *comma_ptr = '.';
        }

        sscanf(linha, "%*c;%d;%[^;];%[^;];%[^;];%d;%f", &produto.codigo, produto.nome, produto.marca, produto.categoria, &produto.estoque, &produto.preco);
        inserir_lote(files, produto);
    } else if (tipo == 'A') {

        int codigo, estoque;
        float preco;
        sscanf(linha, "%*c;%d;%d;%f", &codigo, &estoque, &preco);
        if (estoque != -1) {
            PRODUTO_DATA produto = {codigo, "", "", "", estoque, preco};
            atualizar_estoque_lote(files, produto.codigo, produto.estoque);
        } else if (preco != -1) {
            PRODUTO_DATA produto = {codigo, "", "", "", 0, preco};
            atualizar_preco_lote(files, produto.codigo, produto.preco);
        }

    } else if (tipo == 'R') {
        int codigo;
        sscanf(linha, "%*c;%d", &codigo);
        remover_lote(files, codigo);
    }
}

// L� um arquivo contendo lotes de produtos e processa suas linhas
// Pr�-condi��es: Os arquivos de �ndices e de dados devem existir e estar formatados corretamente
// P�s-condi��es: As opera��es correspondentes �s linhas do arquivo s�o realizadas no sistema
void ler_arquivo(ARQUIVOS files, char * nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("ATENCAO: Erro ao abrir o arquivo.\n");
        exit(1);
    }

    char linha[100];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        processar_linha(files, linha);
    }

    fclose(arquivo);
}
