#include <stdio.h>
#include <stdlib.h>
#include "produto_remocao.h"
#include "arvore_utils.h"

// Fun��o para remover um produto da �rvore B
// Pr�-condi��es: ARQUIVOS files cont�m os arquivos necess�rios para opera��es de leitura e escrita,
//                 incluindo os arquivos de �ndices e de dados.
// P�s-condi��es: O produto com o c�digo especificado � removido da �rvore B, se presente. Caso contr�rio,
//                 mensagens apropriadas s�o exibidas.
void remover_produto(ARQUIVOS files){
    CABECALHO_INDICES * cab_indices = le_cabecalho_indices(files.file_indices);

    int codigo, pos_codigo, pos_raiz, pos_esq, pos_dir;

    printf ("Insira o codigo a ser removido: ");
    scanf ("%d", &codigo);

    pos_codigo = buscar_no(files.file_indices, codigo);

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

// Fun��o para atualizar a lista de posi��es livres no arquivo de dados ap�s a remo��o de um registro
// Pr�-condi��es: O arquivo de dados deve estar aberto e a posi��o do registro removido deve ser v�lida.
// P�s-condi��es: A lista de posi��es livres no arquivo de dados � atualizada.
void atualizar_pos_livres_dados(FILE * file_dados, int pos) {
    CABECALHO_DADOS * cab = le_cabecalho_dados(file_dados);
    DADOS_REGISTRO * p = ler_registro(file_dados, pos);

    if (p != NULL) {
        p->prox_livre = cab->pos_livre;
        escreve_registro(file_dados, p, pos);
        cab->pos_livre = pos;
        escreve_cabecalho_dados(file_dados, cab);
        free(p);
    }
    free(cab);
}


// Fun��o para atualizar a lista de posi��es livres no arquivo de �ndices ap�s a remo��o de um n�
// Pr�-condi��es: O arquivo de �ndices deve estar aberto e a posi��o do n� removido deve ser v�lida.
// P�s-condi��es: A lista de posi��es livres no arquivo de �ndices � atualizada.
void atualizar_pos_livres_indices(FILE * file_indices, int pos) {
    CABECALHO_INDICES * cab = le_cabecalho_indices(file_indices);
    ARVOREB * no = ler_no(file_indices, pos);//REVER, era ler produto

    if( no != NULL) {
        no->prox_livre = cab->pos_livre;
        escreve_no(file_indices, no, pos);
        cab->pos_livre = pos;
        escreve_cabecalho_indices(file_indices, cab);
        free(no);
    }
    free(cab);
}

// Fun��o para atualizar um n� ap�s a remo��o de um registro em um n� folha
// Pr�-condi��es: O arquivo de �ndices deve estar aberto e o n� folha, bem como as posi��es de remo��o e c�digo, devem ser v�lidos.
// P�s-condi��es: O n� � atualizado com as informa��es da remo��o.
void atualiza_no_remocao_folha(ARQUIVOS files, ARVOREB * folha, int pos_remocao, int pos_codigo){
    CABECALHO_DADOS * cab_dados = le_cabecalho_dados(files.file_dados);

    // A posi��o dos dados no arq de dados deve ser atualizado
    // folha->pt_dados[pos_codigo] => posi��o dos dados no file de dados
    int pos_registro = folha->pt_dados[pos_codigo];

    atualizar_pos_livres_dados(files.file_dados, pos_registro);

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

    escreve_no(files.file_indices, folha, pos_remocao);
    free(cab_dados);
}

// Fun��o para tratar a remo��o quando esta � feita em um n� folha com n�mero de chaves maior que o m�nimo (ORDEM/2 + ORDEM%2 - 1)
// Pr�-condi��es: O arquivo de �ndices deve estar aberto e o n� a ser removido, bem como o c�digo e a posi��o de remo��o, devem ser v�lidos.
// P�s-condi��es: A remo��o � tratada de acordo com o caso espec�fico.
void remover_caso1(ARQUIVOS files, ARVOREB * no_a_remover, int codigo, int pos_remocao){
    int pos_codigo;

    // Encontra a pos dentro do n�
    pos_codigo = buscar_pos_chave(no_a_remover, codigo);
    atualiza_no_remocao_folha(files, no_a_remover, pos_remocao, pos_codigo);
}

// Fun��o para buscar a chave sucessora em um n� folha
// Pr�-condi��es: O arquivo de �ndices deve estar aberto e o n� a ser removido, bem como a posi��o da chave e a posi��o do n� sucessor, devem ser v�lidos.
// P�s-condi��es: A chave sucessora � retornada e a posi��o do n� sucessor � atualizada.
int buscar_chave_sucessora_folha(ARQUIVOS files, ARVOREB * no_a_remover, int pos_codigo, int * pos_no_sucessor){
    int pos_chave_sucessora = no_a_remover->filho[pos_codigo + 1];
    int chave_sucessora;

    ARVOREB * no_sucessor = ler_no(files.file_indices, pos_chave_sucessora);

    if(eh_folha(no_sucessor)){
        *pos_no_sucessor = pos_chave_sucessora;
        chave_sucessora = no_sucessor->chave[0];
        free(no_sucessor);
        return chave_sucessora;
    }

    return buscar_chave_sucessora_folha(files, no_sucessor, -1, pos_no_sucessor);
}

// Fun��o para tratar a remo��o quando esta � feita em um n� interno
// Pr�-condi��es: O arquivo de �ndices deve estar aberto e o n� a ser removido, bem como o c�digo e a posi��o de remo��o, devem ser v�lidos.
// P�s-condi��es: A remo��o � tratada de acordo com o caso espec�fico.
int remover_caso2(ARQUIVOS files, ARVOREB * no_a_remover, int codigo, int pos_remocao){
    // Primeiramente, busca a posi��o da chave (dentro do n�) a ser removida no n�
    int pos_codigo = buscar_pos_chave(no_a_remover, codigo);
    int pos_no_sucessor;

    // Busca a chave sucessora para inseri-l� na pos_codigo encontrada
    // Tamb�m retorna a posi��o no arq de indices do n� sucessor
    int chave_sucessora = buscar_chave_sucessora_folha(files, no_a_remover, pos_codigo, &pos_no_sucessor);

    // Preciso atualizar a folha que continha a chave sucessora
    ARVOREB * no_sucessor = ler_no(files.file_indices, pos_no_sucessor);

    int pt_dados_remocao = no_sucessor->pt_dados[0];

    atualiza_no_remocao_folha(files, no_sucessor, pos_no_sucessor, 0);

    // Realiza a substitui��o da chave removida pela chave sucessora
    no_a_remover->chave[pos_codigo] = chave_sucessora;
    no_a_remover->pt_dados[pos_codigo] = pt_dados_remocao;
    escreve_no(files.file_indices, no_a_remover, pos_remocao);

    free(no_sucessor);
    return pos_no_sucessor;
}

// Fun��o para verificar se � poss�vel realizar a redistribui��o de chaves entre irm�os
// Pr�-condi��es: O arquivo de �ndices deve estar aberto, a posi��o do pai, o �ndice do filho, bem como os ponteiros para as posi��es dos filhos esquerdo e direito, devem ser v�lidos.
// P�s-condi��es: Retorna 1 se a redistribui��o for poss�vel, caso contr�rio, retorna 0.
int pode_redistribuir(ARQUIVOS files, int pos_pai, int indice_filho, int * pegar_esq, int * pegar_dir) {

    // Buscar o pai
    ARVOREB * pai = ler_no(files.file_indices, pos_pai);
    ARVOREB * esq = (ARVOREB *) malloc(sizeof(ARVOREB));
    ARVOREB * dir = (ARVOREB *) malloc(sizeof(ARVOREB));

    // Depois obter os filhos a esquerda e direita
    if (indice_filho == 0) {
        dir = ler_no(files.file_indices, pai->filho[1]);

        if (dir->num_chaves > MIN) {
            *pegar_esq = -1;
            *pegar_dir = pai->filho[1];
            free(pai);
            free(esq);
            free(dir);
            return 1;
        }

    } else if (indice_filho == pai->num_chaves ){
        esq = ler_no(files.file_indices, pai->filho[pai->num_chaves - 1]);

        if (esq->num_chaves > MIN) {
            *pegar_esq = pai->filho[pai->num_chaves - 1];
            *pegar_dir = -1;
            free(pai);
            free(esq);
            free(dir);
            return 1;
        }

    } else {
        esq = ler_no(files.file_indices, pai->filho[indice_filho - 1]);
        dir = ler_no(files.file_indices, pai->filho[indice_filho + 1]);

        if (dir->num_chaves > MIN) {
            *pegar_esq = -1;
            *pegar_dir = pai->filho[indice_filho + 1];
            free(pai);
            free(esq);
            free(dir);
            return 1;

        } else if (esq->num_chaves > MIN) {
            *pegar_esq = pai->filho[indice_filho - 1];
            *pegar_dir = -1;
            free(pai);
            free(esq);
            free(dir);
            return 1;
        }
    }

    *pegar_esq = -1;
    *pegar_dir = -1;
    free(pai);
    free(esq);
    free(dir);
    return 0;
}

// Fun��o para redistribuir chaves a partir do irm�o direito
// Pr�-condi��es: O arquivo de �ndices deve estar aberto e as posi��es do pai, remo��o, filho da remo��o e irm�o direito devem ser v�lidos.
// P�s-condi��es: As chaves s�o redistribu�das entre o n� e seu irm�o direito.
void redistribuir_partir_da_direita(ARQUIVOS files, int pos_pai, int pos_remocao, int pos_filho_remocao, int pos_dir){
    ARVOREB * pai = ler_no(files.file_indices, pos_pai);
    ARVOREB * no_remocao = ler_no(files.file_indices, pos_remocao);
    ARVOREB * dir = ler_no(files.file_indices, pos_dir);

    no_remocao->chave[no_remocao->num_chaves] = pai->chave[pos_filho_remocao];
    no_remocao->pt_dados[no_remocao->num_chaves] = pai->pt_dados[pos_filho_remocao];
    no_remocao->num_chaves++;
    no_remocao->filho[no_remocao->num_chaves] = dir->filho[0];

    pai->chave[pos_filho_remocao] = dir->chave[0];
    pai->pt_dados[pos_filho_remocao] = dir->pt_dados[0];

    int i;
    for(i = 0; i < dir->num_chaves - 1; i++){
        dir->chave[i] = dir->chave[i+1];
        dir->pt_dados[i] = dir->pt_dados[i+1];
        dir->filho[i] = dir->filho[i+1];
    }
    dir->filho[i] = dir->filho[dir->num_chaves];
    dir->num_chaves--;

    escreve_no(files.file_indices, pai, pos_pai);
    escreve_no(files.file_indices, no_remocao, pos_remocao);
    escreve_no(files.file_indices, dir, pos_dir);

    free(pai);
    free(no_remocao),
    free(dir);
}

// Fun��o para redistribuir chaves a partir do irm�o esquerdo
// Pr�-condi��es: O arquivo de �ndices deve estar aberto e as posi��es do pai, remo��o, filho da remo��o e irm�o esquerdo devem ser v�lidos.
// P�s-condi��es: As chaves s�o redistribu�das entre o n� e seu irm�o esquerdo.
void redistribuir_partir_da_esquerda(ARQUIVOS files, int pos_pai, int pos_remocao, int pos_filho_remocao, int pos_esq){
    ARVOREB *pai = ler_no(files.file_indices, pos_pai);
    ARVOREB *no_remocao = ler_no(files.file_indices, pos_remocao);
    ARVOREB *esq = ler_no(files.file_indices, pos_esq);

    // Movendo a chave e o ponteiro de dados do pai para o n� de remo��o
    int i;
    for(i = no_remocao->num_chaves; i > 0; i--){
        no_remocao->chave[i] = no_remocao->chave[i - 1];
        no_remocao->pt_dados[i] = no_remocao->pt_dados[i - 1];
        no_remocao->filho[i + 1] = no_remocao->filho[i];
    }
    no_remocao->filho[1] = no_remocao->filho[0];
    no_remocao->chave[0] = pai->chave[pos_filho_remocao - 1];
    no_remocao->pt_dados[0] = pai->pt_dados[pos_filho_remocao - 1];
    no_remocao->num_chaves++;

    // Movendo a chave e o ponteiro de dados do n� esquerdo para o pai
    pai->chave[pos_filho_remocao - 1] = esq->chave[esq->num_chaves - 1];
    pai->pt_dados[pos_filho_remocao - 1] = esq->pt_dados[esq->num_chaves - 1];

    // Movendo o filho mais � direita do n� esquerdo para o n� de remo��o
    no_remocao->filho[0] = esq->filho[esq->num_chaves];

    // Atualizando o n� esquerdo
    esq->num_chaves--;

    // Escrevendo as altera��es de volta nos arquivos
    escreve_no(files.file_indices, pai, pos_pai);
    escreve_no(files.file_indices, no_remocao, pos_remocao);
    escreve_no(files.file_indices, esq, pos_esq);

    free(pai);
    free(no_remocao);
    free(esq);
}

// Fun��o para realizar a redistribui��o de chaves entre irm�os
// Pr�-condi��es: O arquivo de �ndices deve estar aberto e as posi��es do pai, remo��o, filho da remo��o e irm�os devem ser v�lidos.
// P�s-condi��es: As chaves s�o redistribu�das entre os irm�os conforme necess�rio.
void redistribuir(ARQUIVOS files, int pos_pai, int pos_remocao, int pos_filho_remocao, int pos_esq, int pos_dir){

    if(pos_dir != -1){
        redistribuir_partir_da_direita(files, pos_pai, pos_remocao, pos_filho_remocao, pos_dir);
    } else {
        redistribuir_partir_da_esquerda(files, pos_pai, pos_remocao, pos_filho_remocao, pos_esq);
    }

}

// Fun��o para buscar os filhos esquerdo e direito de um n� pai
// Pr�-condi��es: O arquivo de �ndices deve estar aberto e acess�vel. A posi��o do n� pai deve ser v�lida.
// P�s-condi��es: As posi��es dos filhos esquerdo e direito s�o retornadas por meio dos par�metros pos_esq e pos_dir, respectivamente.
void buscar_filhos_esq_dir(ARQUIVOS files, int pos_pai, int indice_remocao, int * pos_esq, int * pos_dir){
    // Buscar o pai
    ARVOREB * pai = ler_no(files.file_indices, pos_pai);

    // Depois obter os filhos a esquerda e direita
    if (indice_remocao == 0) {
        *pos_dir = pai->filho[indice_remocao + 1];
        *pos_esq = pai->filho[indice_remocao];
        free(pai);
        return;
    } else if (indice_remocao == pai->num_chaves) {
        *pos_esq = pai->filho[pai->num_chaves - 1];
        *pos_dir = -1;
        free(pai);
        return;
    }

    *pos_esq = pai->filho[indice_remocao - 1];
    *pos_dir = pai->filho[indice_remocao + 1];
    free(pai);
}

// Fun��o para concatenar o n� da esquerda com o n� a ser removido
// Pr�-condi��es: Todos os n�s e arquivos devem estar acess�veis e corretamente inicializados.
//                O n� a ser removido n�o deve ser a raiz e deve conter as chaves a serem concatenadas.
// P�s-condi��es: O n� da esquerda � concatenado com o n� a ser removido, mantendo a estrutura da �rvore B.
void concatenar_esquerda(ARQUIVOS files, ARVOREB * pai, ARVOREB * esq, ARVOREB * no_a_remover, int indice_filho){
    int i;

    esq->chave[esq->num_chaves] = pai->chave[indice_filho - 1];
    esq->pt_dados[esq->num_chaves] = pai->pt_dados[indice_filho - 1];
    esq->num_chaves++;

    // Trazer as infos do n� removido para o n� da esquerda
    for(i = 0; i < no_a_remover->num_chaves; i++){
        esq->chave[esq->num_chaves] = no_a_remover->chave[i];
        esq->pt_dados[esq->num_chaves] = no_a_remover->pt_dados[i];
        esq->filho[esq->num_chaves] = no_a_remover->filho[i];
        esq->num_chaves++;
    }

    esq->filho[esq->num_chaves] = no_a_remover->filho[i];

    // Atualizar a lista de livres, pois agora o n� removido � vazio (NULL = -1)
    atualizar_pos_livres_indices(files.file_indices, pai->filho[indice_filho]);

    // Atualizar n� pai
    for(i = indice_filho; i < pai->num_chaves - 1; i++){
        pai->chave[i] = pai->chave[i+1];
        pai->pt_dados[i] = pai->pt_dados[i+1];
        pai->filho[i+1] = pai->filho[i+2];
    }

    pai->num_chaves--;
}

// Fun��o para concatenar o n� da direita com o n� a ser removido
// Pr�-condi��es: Todos os n�s e arquivos devem estar acess�veis e corretamente inicializados.
//                O n� a ser removido n�o deve ser a raiz e deve conter as chaves a serem concatenadas.
// P�s-condi��es: O n� da direita � concatenado com o n� a ser removido, mantendo a estrutura da �rvore B.
void concatenar_direita(ARQUIVOS files, ARVOREB * pai, ARVOREB * dir, ARVOREB * no_a_remover, int pos_filho_remocao){
    int i;

    // Mover a chave do pai para o n� a ser removido na posi��o correta
    no_a_remover->chave[no_a_remover->num_chaves] = pai->chave[pos_filho_remocao];
    no_a_remover->pt_dados[no_a_remover->num_chaves] = pai->pt_dados[pos_filho_remocao];
    no_a_remover->num_chaves++;

    // Trazer as informa��es do n� direito para o n� a ser removido
    for (i = 0; i < dir->num_chaves; i++) {
        no_a_remover->chave[no_a_remover->num_chaves] = dir->chave[i];
        no_a_remover->pt_dados[no_a_remover->num_chaves] = dir->pt_dados[i];
        no_a_remover->filho[no_a_remover->num_chaves] = dir->filho[i];
        no_a_remover->num_chaves++;
    }
    no_a_remover->filho[no_a_remover->num_chaves] = dir->filho[i]; // �ltimo filho

    // Atualizar a lista de livres, pois o n� direito agora est� vazio
    atualizar_pos_livres_indices(files.file_indices, pai->filho[pos_filho_remocao] + 1);

    // Atualizar n� pai
    for (i = pos_filho_remocao; i < pai->num_chaves - 1; i++) {
        pai->chave[i] = pai->chave[i + 1];
        pai->pt_dados[i] = pai->pt_dados[i + 1];
        pai->filho[i + 1] = pai->filho[i + 2];
    }

    pai->num_chaves--;
}

// Fun��o para realizar a concatena��o dos n�s
// Pr�-condi��es: Todos os n�s e arquivos devem estar acess�veis e corretamente inicializados.
//                As posi��es dos n�s pai, remo��o, filhos esquerdo e direito devem ser v�lidas.
// P�s-condi��es: Os n�s s�o concatenados conforme necess�rio, mantendo a estrutura da �rvore B.
void concatenar(ARQUIVOS files, int pos_pai, int pos_remocao, int indice_filho, int pos_esq, int pos_dir){
    ARVOREB * pai = ler_no(files.file_indices, pos_pai);
    ARVOREB * no_a_remover = ler_no(files.file_indices, pos_remocao);
    ARVOREB * esq = (ARVOREB *) malloc(sizeof(ARVOREB));
    ARVOREB * dir = (ARVOREB *) malloc(sizeof(ARVOREB));

    if(pos_dir == -1){
        esq = ler_no(files.file_indices, pos_esq);

        concatenar_esquerda(files, pai, esq, no_a_remover, indice_filho);
        escreve_no(files.file_indices, pai, pos_pai);
        escreve_no(files.file_indices, esq, pos_esq);

    } else {
        dir = ler_no(files.file_indices, pos_dir);

        concatenar_direita(files, pai, dir, no_a_remover, indice_filho);
        escreve_no(files.file_indices, pai, pos_pai);
        escreve_no(files.file_indices, no_a_remover, pos_remocao);
    }

    free(no_a_remover);
    free(esq);
    free(dir);
}

// Fun��o para balancear a �rvore ap�s uma remo��o
// Pr�-condi��es: Todos os n�s e arquivos devem estar acess�veis e corretamente inicializados.
//                A posi��o do pai, �ndice do filho removido e posi��o da remo��o devem ser v�lidos.
// P�s-condi��es: A �rvore � balanceada de acordo com as regras da �rvore B.
void balancear(ARQUIVOS files, int pos_pai, int indice_filho, int pos_remocao) {
    CABECALHO_INDICES * cab_indices = le_cabecalho_indices(files.file_indices);

    if (pos_pai != -1) {

        // 1� Verifica-se se � poss�cel realizar a redistribui��o
        int pos_esq, pos_dir;
        // A fun��o pode_redistribuir() verifica se � poss�vel realizar a redistribui��o e indica a partir de qual lado
        int teste_pode_redistribuir = pode_redistribuir(files, pos_pai, indice_filho, &pos_esq, &pos_dir);

        if ( teste_pode_redistribuir ) {
            // Aqui ser� inserido a l�gica da redistribui��o
            // A fun��o pode_redistribuir guarda a posi��o no arquivo dos filhos da esq e dir
            redistribuir(files, pos_pai, pos_remocao, indice_filho, pos_esq, pos_dir);

        } else {
            // Aqui ser� inserido a l�gica da concatena��o
            buscar_filhos_esq_dir(files, pos_pai, indice_filho, &pos_esq, &pos_dir);
            concatenar(files, pos_pai, pos_remocao, indice_filho, pos_esq, pos_dir);
        }

    }

    free(cab_indices);
}

// Fun��o para verificar e atualizar o pai ap�s uma remo��o.
// Pr�-condi��es: Todos os n�s e arquivos devem estar acess�veis e corretamente inicializados.
//                A posi��o do pai a ser verificado deve ser v�lida.
// P�s-condi��es: O pai � atualizado conforme necess�rio, mantendo a integridade da �rvore B.
void verificar_pai(ARQUIVOS files, int pos_pai) {

    ARVOREB * pai = ler_no(files.file_indices, pos_pai);
    int pos_pai_do_pai, indice_filho; // pos_no_sucessor = pos_pai

    while ( pai->num_chaves < MIN && !eh_raiz(files.file_indices, pos_pai) ) {
        pos_pai_do_pai = buscar_pai_by_pos(files.file_indices, pos_pai);
        ARVOREB * pai_do_pai = ler_no(files.file_indices, pos_pai_do_pai);
        indice_filho = buscar_pos_filho(pai_do_pai, pos_pai);

        free(pai_do_pai);

        balancear(files, pos_pai_do_pai, indice_filho, pos_pai);

        free(pai);
        pai = ler_no(files.file_indices, pos_pai_do_pai);
        pos_pai = pos_pai_do_pai;
    }

    if(pai->num_chaves == 0){
        CABECALHO_INDICES * cab_indices = le_cabecalho_indices(files.file_indices);
        cab_indices->pos_raiz = pai->filho[0];
        escreve_cabecalho_indices(files.file_indices, cab_indices);
        free(cab_indices);
    }

    free(pai);
}

// Fun��o para remover uma chave da �rvore B-B
// Pr�-condi��es: Todos os n�s e arquivos devem estar acess�veis e corretamente inicializados.
//                O c�digo da chave a ser removida, a posi��o da raiz e a posi��o de remo��o devem ser v�lidos.
// P�s-condi��es: A chave � removida da �rvore B-B, mantendo sua estrutura e integridade.
void remover(ARQUIVOS files, int codigo, int pos_raiz, int pos_remocao){
    CABECALHO_INDICES * cab_indices = le_cabecalho_indices(files.file_indices);
    CABECALHO_DADOS * cab_dados = le_cabecalho_dados(files.file_dados);

    // Buscar o n� que possui a chave a ser removida a ser removido
    ARVOREB * no_a_remover = ler_no(files.file_indices, pos_remocao);

    // 1� CASO: a remo��o � feita em um n� folha com n�mero de chaves maior que o m�nimo (ORDEM/2)
    if(eh_raiz(files.file_indices, pos_remocao) && eh_folha(no_a_remover)) {
        remover_caso1(files, no_a_remover, codigo, pos_remocao);
        if(no_a_remover->num_chaves == 0){
            cab_indices->pos_raiz = -1;
            cab_indices->pos_topo = 0;
            cab_indices->pos_livre = -1;
            escreve_cabecalho_indices(files.file_indices, cab_indices);

            cab_dados->pos_topo = 0;
            cab_dados->pos_livre = -1;
            escreve_cabecalho_dados(files.file_dados, cab_dados);
        }

    }else if( (mais_chaves_que_min(no_a_remover) && eh_folha(no_a_remover)) ){
        // Logo, apenas remove a chave do n�, realizando as altera��es necess�rias e gravando novamente no arquivo
        remover_caso1(files, no_a_remover, codigo, pos_remocao);

    } else if ( !eh_folha(no_a_remover) ) { // CASO 2�: a remo��o � feita em um n� interno
        // Logo, busca-se a chave sucessora e a insere no lugar da chave removida no n� interno
        // Al�m disso, a fun��o deve retornar a posi��o do n� chave sucessora, pois esse n� deve ser tratado ap�s remo��o
        int pos_codigo, pos_no_suc, chave_sucessora, pos_no_sucessor, pos_pai;
        pos_codigo = buscar_pos_chave(no_a_remover, codigo);
        chave_sucessora = buscar_chave_sucessora_folha(files, no_a_remover, pos_codigo, &pos_no_suc);
        pos_pai = buscar_pai(files.file_indices, chave_sucessora);

        pos_no_sucessor = remover_caso2(files, no_a_remover, codigo, pos_remocao);
        ARVOREB * no_sucessor = ler_no(files.file_indices, pos_no_sucessor);

        if( no_sucessor->num_chaves < MIN ){
            ARVOREB * pai_no_sucessor = ler_no(files.file_indices, pos_pai);
            int indice_filho = buscar_pos_filho(pai_no_sucessor , pos_no_sucessor);
            free(pai_no_sucessor);

            balancear(files, pos_pai, indice_filho, pos_no_sucessor);
            verificar_pai(files, pos_pai);
        }

    } else if ( !mais_chaves_que_min(no_a_remover) && eh_folha(no_a_remover) ){
        // CASO 3�: a remo��o � feita em um n� com numero minimo de chaves
        // Logo, � necess�rio verificar, PRIMEIRAMENTE, se pode ser feito a redistribui��o
        // Caso contr�rio, fazer concatena��o

        //Busca o pai para poder veirificar os filhos irm�o e verificar se podem emprestar
        int pos_pai = buscar_pai(files.file_indices, codigo);
        ARVOREB * pai = ler_no(files.file_indices, pos_pai);
        int indice_filho = buscar_pos_filho(pai , pos_remocao);
        free(pai);

        remover_caso1(files, no_a_remover, codigo, pos_remocao);

        if( no_a_remover->num_chaves < MIN ){
            balancear(files, pos_pai, indice_filho, pos_remocao);
            verificar_pai(files, pos_pai);
        }
    }

    free(cab_indices);
}