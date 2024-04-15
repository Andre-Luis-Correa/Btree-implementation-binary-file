//
// Created by andre on 08/04/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produto_insercao.h"
#include "../impressao/produto_impressao.h"

// Fun��o para ler os dados de um produto
PRODUTO_DATA * ler_produto() {
    PRODUTO_DATA * produto = (PRODUTO_DATA*) malloc(sizeof (PRODUTO_DATA));

    printf("\n--> CADASTRO DE PRODUTO: \n");
    printf("Insira o codigo do produto: ");
    scanf("%d", &produto->codigo);
    printf("Insira o nome do produto: ");
    scanf(" %[^\n]", produto->nome);
    printf("Insira a marca do produto: ");
    scanf(" %[^\n]", produto->marca);
    printf("Insira a categoria do produto: ");
    scanf(" %[^\n]", produto->categoria);
    printf("Insira a quantidade em estoque do produto: ");
    scanf("%d", &produto->estoque);
    printf("Insira o preco do produto: ");
    scanf("%f", &produto->preco);

    return produto;
}

int overflow (ARVOREB * r){
    return r->num_chaves == ORDEM;
}

int eh_folha(ARVOREB * r){
    return r->filho[0] == -1;
}

// Fun��o para imprimir os dados de um produto
void imprimir_produto(PRODUTO_DATA * produto) {
    printf("\nDados do Produto:\n");
    printf("Codigo: %d\n", produto->codigo);
    printf("Nome: %s\n", produto->nome);
    printf("Marca: %s\n", produto->marca);
    printf("Categoria: %s\n", produto->categoria);
    printf("Estoque: %d\n", produto->estoque);
    printf("Preco: R$ %.2f\n", produto->preco);
}

// Fun��o para ler um n� do arquivo de �ndices da �rvore B de produtos
ARVOREB * ler_no (FILE * file_indices, int pos){
    ARVOREB * no = (ARVOREB*) malloc(sizeof (ARVOREB));
    fseek (file_indices, sizeof (CABECALHO_INDICES) + pos * sizeof (ARVOREB), SEEK_SET);
    fread (no, sizeof (ARVOREB), 1, file_indices);
    return no;
}

// Fun��o para escrever um n� no arquivo de �ndices
void escreve_no (FILE * file_indices, ARVOREB * no, int pos){
    fseek (file_indices, sizeof (CABECALHO_INDICES) + pos * sizeof (ARVOREB), SEEK_SET);
    fwrite (no, sizeof (ARVOREB), 1, file_indices);
}

// Fun��o para ler um registro de dados de um produto no arquivo de dados
DADOS_REGISTRO * ler_registro(FILE * file_dados, int pos){
    DADOS_REGISTRO * registro = (DADOS_REGISTRO*) malloc(sizeof (DADOS_REGISTRO));
    fseek (file_dados, sizeof (CABECALHO_DADOS) + pos * sizeof (DADOS_REGISTRO), SEEK_SET);
    fread (registro, sizeof (DADOS_REGISTRO), 1, file_dados);
    return registro;
}

// Fun��o para escrever um n� no arquivo de �ndices
void escreve_registro (FILE * file_dados, DADOS_REGISTRO * registro, int pos){
    fseek (file_dados, sizeof (CABECALHO_DADOS ) + pos * sizeof (DADOS_REGISTRO), SEEK_SET);
    fwrite (registro, sizeof (DADOS_REGISTRO), 1, file_dados);
}

// Fun��o para verificar se j� existe um determinado c�digo na �rvore e retorna a posi��o do mesmo
int buscar_no(FILE * file_indices, int codigo, int pos){
    //faz a busca no metodo �rvore binaria de busca
    if(pos == -1) return -1;

    ARVOREB * r = ler_no(file_indices, pos);
    int i;

    for(i = 0; i < r->num_chaves; i++){
        if(r->chave[i] == codigo ){
            free(r);
            return pos;
        } else if (r->chave[i] > codigo ){
            int pos_atual = r->filho[i];
            free(r);
            return buscar_no(file_indices, codigo, pos_atual);
        }
    }
    int pos_atual = r->filho[i];
    free(r);
    return buscar_no(file_indices, codigo, pos_atual);
}

void imprimir_cabecalho_indices(CABECALHO_INDICES * cab){
    printf("\nCABECALHO DE INDICES: raiz: %d topo: %d livre: %d\n", cab->pos_raiz, cab->pos_topo, cab->pos_livre);
}

void imprimir_cabecalho_dados(CABECALHO_DADOS * cab){
    printf("\nCABECALHO DE DADOS: topo: %d livre: %d\n", cab->pos_topo, cab->pos_livre);
}

void cadastrar_produto(ARQUIVOS files){
    CABECALHO_INDICES * cab_indices = le_cabecalho_indices(files.file_indices);
    CABECALHO_DADOS * cab_dados = le_cabecalho_dados(files.file_dados);

    PRODUTO_DATA * produto = ler_produto();

    if(buscar_no(files.file_indices, produto->codigo, cab_indices->pos_raiz) == -1){
        // Realizar a inser��o do n�, pois ainda n�o existe esse c�digo na �rvore
        cadastrar_produto_files(files, produto);
    }else{
        printf("\nATENCAO: Codigo de produto ja existente!\n");
    }

    free(cab_indices);
    free(produto);
}

void cadastrar_produto_files(ARQUIVOS files, PRODUTO_DATA * produto) {
    CABECALHO_INDICES * cab_indices = le_cabecalho_indices(files.file_indices);

    if(cab_indices->pos_livre == -1) {
        printf("A insercao sera feita considerando que nao ha nos livres!\n");
        cadastrar_produto_sem_nos_livres(files, produto, cab_indices->pos_raiz);
    } else {
        printf("A insercao sera feita considerando que ha nos livres!\n");
        cadastrar_produto_com_nos_livres(files, produto, cab_indices->pos_raiz);
    }

    free(cab_indices);
}

void cria_no(ARVOREB * r, int codigo, int pos_dados) {
    r->chave[0] = codigo;
    r->num_chaves = 1;
    r->prox_livre = -1;
    r->pt_dados[0] = pos_dados;
    for(int i = 0; i <= r->num_chaves; i++)
        r->filho[i] = -1;
}

void cria_registro(DADOS_REGISTRO * dados, PRODUTO_DATA * produto){
    dados->produto = *produto;
    dados->prox_livre = -1;
}

int split (ARQUIVOS files, ARVOREB * x, int pos, int * meio, int * pos_meio, CABECALHO_INDICES * cab_indices){
    ARVOREB* y = (ARVOREB*) malloc (sizeof(ARVOREB));

    // O q representa o a metade do n�
    int q = x->num_chaves/2;
    y->num_chaves = x->num_chaves - q - 1;
    x->num_chaves = q;

    escreve_no(files.file_indices, x, pos);

    *meio = x->chave[q];
    *pos_meio = x->pt_dados[q];
    int i;
    y->filho[0] = x->filho[q+1];
    for (i = 0 ; i < y->num_chaves ; i++){
        y->chave[i] = x->chave[q+i+1];
        y->pt_dados[i] = x->pt_dados[q+i+1];
        y->filho[i+1] = x->filho[q+i+2];
    }

    int pos_y = cab_indices->pos_topo;
    escreve_no(files.file_indices, y, cab_indices->pos_topo);
    cab_indices->pos_topo++;
    escreve_cabecalho_indices(files.file_indices, cab_indices);
    return pos_y;
}

void cadastrar_produto_sem_nos_livres(ARQUIVOS files, PRODUTO_DATA * produto, int pos){
    CABECALHO_INDICES * cab_indices = le_cabecalho_indices(files.file_indices);
    CABECALHO_DADOS * cab_dados = le_cabecalho_dados(files.file_dados);
    ARVOREB * r = (ARVOREB*) malloc(sizeof (ARVOREB));
    DADOS_REGISTRO * dados = (DADOS_REGISTRO *) malloc(sizeof (DADOS_REGISTRO));

    if (pos == -1) { // pos -> pos_raiz == -1 �rvore vazia ou a fun��o recebeu uma posi��o (pos) vazia

        // Atualiza a pois��o da cabe�a
        cab_indices->pos_raiz = 0;

        // Escreve no arquivo de dados as informa��es do produto a ser inserido
        cria_registro(dados, produto);
        escreve_registro(files.file_dados, dados, cab_dados->pos_topo);

        // Cria a raiz com as informa��es: c�digo do produto e o ponteiro de dados do arquivo de dados
        cria_no(r, produto->codigo, cab_dados->pos_topo);
        escreve_no(files.file_indices, r, cab_indices->pos_topo);

        // Atualiza os cabe�alhos de AMBOS arquivos
        cab_indices->pos_topo++;
        cab_dados->pos_topo++;
        escreve_cabecalho_indices(files.file_indices, cab_indices);
        escreve_cabecalho_dados(files.file_dados, cab_dados);

    } else { // �rvore n�o vazia

        // L� a raiz da �rvore
        ARVOREB * raiz = ler_no(files.file_indices, cab_indices->pos_raiz);

        // Cria um registro e escreve no arquivo de dados
        DADOS_REGISTRO * dados_novos = (DADOS_REGISTRO*) malloc(sizeof (DADOS_REGISTRO));
        cria_registro(dados_novos, produto);
        escreve_registro(files.file_dados, dados_novos, cab_dados->pos_topo);

        // Guarda a informa��o de onde o novo registro foi inserido no arquivo de dados
        int pt_dados_atual = cab_dados->pos_topo;

        // Atualiza o cabecalho do ARQUIVO de DADOS
        cab_dados->pos_topo++;
        escreve_cabecalho_dados(files.file_dados, cab_dados);

        // Chama a fun��o auxiliar de inser��o
        // Os par�metros s�o: Os arquivos (AMBOS), c�digo do produto, ponteiro de dado do produto, posi��o da raiz
        cadastrar_aux (files, raiz, produto->codigo, pt_dados_atual, cab_indices->pos_raiz);

        // L� o cabe�alho atual do arquivo de �ndices
        CABECALHO_INDICES * cab_indices_atual = le_cabecalho_indices(files.file_indices);

        if(overflow(raiz)){

            int meio;

            int pos_meio;
            int arvore_x = split(files, raiz, cab_indices_atual->pos_raiz, &meio, &pos_meio, cab_indices_atual);

            ARVOREB * teste_x = (ARVOREB*) malloc(sizeof (ARVOREB));
            teste_x = ler_no(files.file_indices, arvore_x);

            ARVOREB * nova_raiz = (ARVOREB*) malloc(sizeof (ARVOREB));
            nova_raiz->chave[0] = meio;
            nova_raiz->pt_dados[0] = pos_meio;
            nova_raiz->filho[0] = cab_indices_atual->pos_raiz;
            nova_raiz->filho[1] = arvore_x;
            for (int i = (((int) ORDEM/2) +1) ; i < ORDEM ; i++){
                raiz->filho[i] = -1;
            }
            nova_raiz->num_chaves = 1;

            escreve_no(files.file_indices, nova_raiz, cab_indices_atual->pos_topo);
            cab_indices_atual->pos_raiz = cab_indices_atual->pos_topo;

            cab_indices_atual->pos_topo++;

            escreve_cabecalho_indices(files.file_indices, cab_indices_atual);
            free(nova_raiz);
            free(teste_x);
        }
        free(raiz);
        free(cab_indices_atual);
    }

    free(r);
    free(dados);
    free(cab_indices);
    free(cab_dados);
    printf("\nInsercao realizada com sucesso!\n");
}

int busca_pos (ARVOREB * r, int codigo, int * pos){
    // O loop busca pela posi��o onde a nova chave (codigo) deveria entrar
    for ((*pos) = 0 ; (*pos) < r->num_chaves ; (*pos)++){
        if (codigo == r->chave[(*pos)]){
            return 1;
        }
        else if (codigo < r->chave[(*pos)]){
            break;
        }
    }

    return 0;
}

void adiciona_direita (ARVOREB * r, int pos, int codigo, int pt_dados, int p){
    int i;

    // Desloca todas as chaves e pt_dados e filhos para frente da pos encontrada, deixondo-a livre
    for (i = r->num_chaves ; i > pos ; i--){
        r->chave [i] = r->chave[i-1];
        r->pt_dados[i] = r->pt_dados[i-1];
        r->filho[i+1] = r->filho[i];
    }

    // A nova chave (codigo) � inserida na posi��o livre
    r->chave[pos] = codigo;

    // O ponteiro de dados da nova chave tambpem � inserido na posi��o livre, ap�s deslocamento
    r->pt_dados[pos] = pt_dados;

    // O filho � direita da chave na posi��o os recebe a pos "p"
    r->filho[pos+1] = p;

    // Incrementa o n�mero de chaves, devido a inser��o do novo c�digo
    r->num_chaves++;
}

// Os par�metros s�o: Os arquivos (AMBOS), c�digo do produto, ponteiro de dado do produto, posi��o da raiz (Na 1� chamada)
void cadastrar_aux(ARQUIVOS files, ARVOREB * r, int codigo, int pt_dados, int pos_atual){
    // Vari�vel para armazenar a posi�ao na qual uma determinada chave deveria entrar
    int pos;
    CABECALHO_INDICES  *  cab_indices = (CABECALHO_INDICES*) malloc(sizeof (CABECALHO_INDICES));
    cab_indices = le_cabecalho_indices(files.file_indices);

    if(!busca_pos(r, codigo, &pos)){
        // O trecho de c�digo abaixo � respons�vel por printar a pos encontrada e o n� analisado em rela��o a essa pos
        // A fun��o eh-folha verifica se o n� r � folha
        if (eh_folha(r)){
            // Caso o n� seja uma folha a inser��o � feita nele mesmo: EM UMA �RVORE B A INSER��O � SEMPRE NA FOLHA
            // Os par�metros da fun��o adiciona_direita s�o: o n� r, a pos onde o novo c�digo deveria ser inserido
            // pt_dados indicando os dados relacionados ao c�digo no arquivo de dados
            // p = -1 quando � uma folha, pois os filhos s�o sempre nulos, no caso de arquivo "-1"
            adiciona_direita(r, pos, codigo, pt_dados, -1);

            // Ap�s ter feito o deslocamento e inser��o, escreve o n� atualizado no arquivo
            escreve_no(files.file_indices, r, pos_atual);
        } else{
            // O trecho de c�digo abaixo � respons�vel por fazer a inser��o a partir de um n� interno
            ARVOREB * filho= ler_no(files.file_indices, r->filho[pos]);
            cadastrar_aux(files, filho, codigo, pt_dados, r->filho[pos]);

            if(overflow(filho)){

                int meio;
                int pos_meio;

                int posicao_no_pos_split = split(files, filho, r->filho[pos], &meio, &pos_meio, cab_indices);

                escreve_no(files.file_indices, filho, r->filho[pos]);

                adiciona_direita(r, pos, meio, pos_meio, posicao_no_pos_split);

                escreve_no(files.file_indices, r, pos_atual);

                escreve_cabecalho_indices(files.file_indices, cab_indices);
            }
            free(filho);
        }
    }
}

void cadastrar_produto_com_nos_livres(ARQUIVOS files, PRODUTO_DATA * produto, int pos){

}

