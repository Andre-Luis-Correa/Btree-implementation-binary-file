#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "curso.h"
#include "cabecalho.h"
#include "file.h"

// Fun��o para ler as informa��es de um curso do usu�rio e retornar uma struct GRADUACAO
// Pr�-condi��o: Nenhuma
// P�s-condi��o: Retorna uma struct GRADUACAO com as informa��es do curso inseridas pelo usu�rio
GRADUACAO ler_curso(){
    GRADUACAO curso;
    printf("\n--> CADASTRO DE CURSO: \n");
    printf("Insira o codigo do curso: ");
    scanf("%d%*c", &curso.codigo);
    printf("Insira o nome do curso: ");
    scanf("%[^\n]%*c", curso.nome);
    printf("Insira a area do curso: ");
    scanf("%c", &curso.area);

    return curso;
}

// Fun��o para ler um n� em uma determinada posi��o do arquivo
// Pr�-condi��o: O arquivo deve estar aberto e ser um arquivo de lista, pos deve ser uma posi��o v�lida da lista
// P�s-condi��o: Retorna um ponteiro para o n� lido do arquivo
NO_CURSO * le_no_curso(FILE* file_curso, int pos) {
    NO_CURSO* no_graduacao = (NO_CURSO*)malloc(sizeof(NO_CURSO));
    fseek(file_curso, sizeof(CABECALHO) + pos * sizeof(NO_CURSO), SEEK_SET);
    fread(no_graduacao, sizeof(NO_CURSO), 1, file_curso);
    return no_graduacao;
}

//Escreve um n� em uma determinada posi��o do arquivo
//Pr�-condi��o: arquivo deve estar aberto e ser um arquivo de lista, pos deve ser uma posi��o v�lida do arquivo
//P�s-condi��o: n� escrito no arquivo
void escreve_no_curso(FILE* file_curso, NO_CURSO * graduacao, int pos) {
    fseek(file_curso, sizeof(CABECALHO) + pos * sizeof(NO_CURSO), SEEK_SET);
    fwrite(graduacao, sizeof(NO_CURSO), 1, file_curso);
}

// Fun��o para buscar um curso no arquivo pelo c�digo
// Pr�-condi��o: O arquivo deve estar aberto, cod � o c�digo a ser buscado, pos � a posi��o inicial da busca
// P�s-condi��o: Retorna a posi��o do curso com o c�digo especificado, -1 se n�o encontrado
int buscar_curso(FILE * file_curso, int cod, int pos){
    if(pos == -1) return -1; // Caso a raiz seja nula, n�o ha elementos na arvore
    else{
        NO_CURSO * curso = le_no_curso(file_curso, pos);
        if(curso->curso.codigo > cod) return buscar_curso(file_curso, cod, curso->esq);//todos os codigos menores que a raiz se encontram a esquerda
        else if(curso->curso.codigo < cod) return buscar_curso(file_curso, cod, curso->dir);//todos os codigos maiores que a raiz se encontram a direita
        free(curso);
    }
    return pos;
}

// Fun��o para buscar um n� de curso no arquivo pelo c�digo
// Pr�-condi��o: O arquivo deve estar aberto, cod � o c�digo a ser buscado, pos � a posi��o inicial da busca
// P�s-condi��o: Retorna o n� curso com o c�digo especificado, NULL se n�o encontrado
NO_CURSO * buscar_no_curso(FILE * file_curso, int cod, int pos){
    if(pos == -1) return NULL; // Caso a raiz seja nula, n�o ha elementos na arvore
    NO_CURSO * curso = le_no_curso(file_curso, pos);
    if(curso->curso.codigo > cod) return buscar_no_curso(file_curso, cod, curso->esq);//todos os codigos menores que a raiz se encontram a esquerda
    else if(curso->curso.codigo < cod) return buscar_no_curso(file_curso, cod, curso->dir);//todos os codigos maiores que a raiz se encontram a direita

    return curso;
}

// Fun��o para buscar um n� de curso no arquivo pelo c�digo
// Pr�-condi��o: O arquivo deve estar aberto, cod � o c�digo a ser buscado, pos � a posi��o inicial da busca
// P�s-condi��o: Retorna o n� curso com o c�digo especificado, NULL se n�o encontrado
NO_CURSO * buscar_info_curso(ARQUIVOS files, int codigo){
    CABECALHO * cab = le_cabecalho(files.file_curso);
    NO_CURSO * no_curso = (NO_CURSO*)malloc(sizeof(NO_CURSO));
    no_curso = buscar_no_curso(files.file_curso, codigo, cab->pos_raiz);
    free(cab);
    return no_curso;
}

// Fun��o para inserir um curso no arquivo
// Pr�-condi��o: O arquivo deve estar aberto
// P�s-condi��o: O curso � inserido no arquivo, e uma mensagem � exibida indicando o resultado da opera��o

void inserir_curso(FILE* file_curso){
    CABECALHO * cab = le_cabecalho(file_curso);

    NO_CURSO * no_curso = (NO_CURSO*)malloc(sizeof(NO_CURSO));
    no_curso->esq = no_curso->dir = -1;
    no_curso->curso = ler_curso();

    if(buscar_curso(file_curso, no_curso->curso.codigo, cab->pos_raiz) == -1){
        //chama a fun��o recursiva para inserir o curso na �rvore
        inserir_curso_rec(file_curso, no_curso, cab->pos_raiz, cab);
        printf("--> Cadastro de curso realizado com sucesso\n");
    }else{
        printf("--> Codigo de curso ja existente\n");
    }

    free(cab);
    free(no_curso);
}

// Fun��o para inserir um curso no arquivo proveniente do arquivo lote txt
// Pr�-condi��o: O arquivo deve estar aberto
// P�s-condi��o: O curso � inserido no arquivo, e uma mensagem � exibida indicando o resultado da opera��o
void inserir_curso_do_lote(FILE* file_curso, GRADUACAO graduacao){
    CABECALHO * cab = le_cabecalho(file_curso);

    NO_CURSO * no_curso = (NO_CURSO*)malloc(sizeof(NO_CURSO));
    no_curso->esq = no_curso->dir = -1;
    no_curso->curso = graduacao;

    if(buscar_curso(file_curso, no_curso->curso.codigo, cab->pos_raiz) == -1){
        //chama a fun��o recursiva para inserir o curso na �rvore
        inserir_curso_rec(file_curso, no_curso, cab->pos_raiz, cab);
        printf("--> Cadastro de curso realizado com sucesso\n");
    }else{
        printf("--> Codigo de curso ja existente\n");
    }

    free(cab);
    free(no_curso);
}

//fun��o recursiva para inserir um curso na �rvore
// Pr�-condi��o: O arquivo deve estar aberto
// P�s-condi��o: O curso � inserido no arquivo, e uma mensagem � exibida indicando o resultado da opera��o
void inserir_curso_rec(FILE* file_curso, NO_CURSO * no_curso, int pos, CABECALHO * cab){
    if(pos == -1){//pos_raiz == -1 �rvore vazia ou a fun��o recebeu uma posi��o (pos) vazia
        if(is_vazia_arvore(cab)) cab->pos_raiz=0;//cabeca apenas alterada se for a primeira insers�o (-1 -> 0)
        escreve_no_curso(file_curso, no_curso, cab->pos_topo);//escreve o novo n� na proxima posicao livre
        cab->pos_topo++;
        escreve_cabecalho(file_curso, cab);
    } else {
        NO_CURSO * prox = le_no_curso(file_curso, pos);
        if(no_curso->curso.codigo < prox->curso.codigo){
            if(prox->esq == -1){
                prox->esq = cab->pos_topo;
                escreve_no_curso(file_curso, prox, pos);
                //chama a fun��o recursiva com pos = -1 para inserir o curso na esquerda
                inserir_curso_rec(file_curso, no_curso, -1, cab);
            } else {
                //chama a fun��o recursiva com pos = prox->esq para inserir o curso na esquerda
                inserir_curso_rec(file_curso, no_curso, prox->esq, cab);
            }
        } else {
            if(no_curso->curso.codigo > prox->curso.codigo){
                if(prox->dir == -1){
                    prox->dir = cab->pos_topo;
                    escreve_no_curso(file_curso, prox, pos);
                    //chama a fun��o recursiva com pos = -1 para inserir o curso na direita
                    inserir_curso_rec(file_curso, no_curso, -1, cab);
                } else {
                    //chama a fun��o recursiva com pos = prox->dir para inserir o curso na direita
                    inserir_curso_rec(file_curso, no_curso, prox->dir, cab);
                }
            }
        }
        free(prox);
    }
}
// Fun��o para imprimir as informa��es de um curso contidas em um n�
// Pr�-condi��o: O n� deve ser v�lido
// P�s-condi��o: As informa��es do curso no n� s�o impressas na tela
void imprimir_info_curso(NO_CURSO * no_graduacao) {
    printf("| %03d    ", no_graduacao->curso.codigo);
    printf("%-50s", no_graduacao->curso.nome);
   // printf("esq: %d   dir: %d  ", no_graduacao->esq, no_graduacao->dir);
    printf("%c  |\n", no_graduacao->curso.area);
}

// Fun��o para imprimir os cursos em ordem no arquivo
// Pr�-condi��o: O arquivo deve estar aberto e conter cursos
// P�s-condi��o: Os cursos s�o impressos em ordem na tela

void imprimir_in_order_curso(FILE* file_curso, int pos_atual){
    if (pos_atual != -1) {
        NO_CURSO * no_atual = le_no_curso(file_curso, pos_atual);

        // Recursivamente imprime a sub�rvore esquerda
        imprimir_in_order_curso(file_curso, no_atual->esq);

        imprimir_info_curso(no_atual);

        // Recursivamente imprime a sub�rvore direita
        imprimir_in_order_curso(file_curso, no_atual->dir);
    }
}

// Fun��o para imprimir todos os cursos em ordem no arquivo
// Pr�-condi��o: O arquivo deve estar aberto
// P�s-condi��o: Os cursos s�o impressos em ordem na tela
void imprimir_lista_cursos(FILE* file_curso) {
    CABECALHO *cab = le_cabecalho(file_curso);

    if(is_vazia_arvore(cab)) printf("--> Nao ha cursos cadastrados\n");
    else{
        printf(" ----------------------- Lista de Cursos ---------------------\n");
        printf("| COD.   NOME                                            AREA |\n");
        imprimir_in_order_curso(file_curso, cab->pos_raiz);
        printf(" -------------------------------------------------------------\n");
    }

    free(cab);
}
