#include "stdio.h"
#include "stdlib.h"
#include "produto_impressao.h"
#include "produto_insercao.h"
#include "arvore_utils.h"

// Fun��o para imprimir a lista de produtos armazenados na �rvore B-B
// Pr�-condi��es: Todos os arquivos e n�s devem estar acess�veis e corretamente inicializados.
//                A posi��o atual na �rvore deve ser v�lida.
// P�s-condi��es: A lista de produtos � impressa na sa�da padr�o, seguindo a estrutura definida.
void imprimir_lista_produtos(ARQUIVOS files, int pos_atual) {
    CABECALHO_INDICES *cab_indices = le_cabecalho_indices(files.file_indices);

    if (pos_atual == -1) {
        free(cab_indices);
        return;
    }else{
        ARVOREB *no = ler_no(files.file_indices, pos_atual);

        for (int i = 0; i <= no->num_chaves; i++) {
            imprimir_lista_produtos(files, no->filho[i]);

            if (i != no->num_chaves) {
                DADOS_REGISTRO *registro = ler_registro(files.file_dados, no->pt_dados[i]);
                printf("| %-8d | %-50s | %-25s | %-8d | R$ %-7.2f |\n", registro->produto.codigo, registro->produto.nome, registro->produto.marca, registro->produto.estoque, registro->produto.preco);
                free(registro);
            }
        }

        free(no);
        free(cab_indices);
    }

}

// Fun��o para imprimir as informa��es de um produto espec�fico na �rvore B
// Pr�-condi��es: Todos os arquivos e n�s devem estar acess�veis e corretamente inicializados.
//                O c�digo do produto a ser buscado deve ser v�lido e existir na �rvore.
// P�s-condi��es: As informa��es do produto s�o impressas na sa�da padr�o, seguindo a estrutura definida.
void imprimir_informacoes_produto(ARQUIVOS files, int codigo){
    CABECALHO_INDICES *cab_indices = le_cabecalho_indices(files.file_indices);

    int pos = buscar_no(files.file_indices, codigo);

    if(pos != -1) {
        ARVOREB * r = ler_no(files.file_indices, pos);

        int i;
        for(i = 0; i < r->num_chaves; i++){
            if(r->chave[i] == codigo) break;
        }

        DADOS_REGISTRO * informacoes_produto = ler_registro(files.file_dados, r->pt_dados[i]);
        printf("\n");
        printf("\n");
        printf("+-------------------------------------------------------------------------------------------------------------------+\n");
        printf("|                                                 Lista de Produtos                                                 |\n");
        printf("+-------------------------------------------------------------------------------------------------------------------+\n");
        printf("| CODIGO   | NOME                                               | MARCA                     | ESTOQUE  | PRECO      |\n");
        printf("+-------------------------------------------------------------------------------------------------------------------+\n");
        printf("| %-8d | %-50s | %-25s | %-8d | R$ %-7.2f |\n", informacoes_produto->produto.codigo, informacoes_produto->produto.nome, informacoes_produto->produto.marca, informacoes_produto->produto.estoque, informacoes_produto->produto.preco);
        printf("+-------------------------------------------------------------------------------------------------------------------+\n");

        free(informacoes_produto);
    } else {
        printf("---> Elemento nao encontrado!\n");
    }

    free(cab_indices);
}

// Fun��o para imprimir a lista de �ndices livres no arquivo de �ndices
// Pr�-condi��es: O arquivo de �ndices deve estar acess�vel e corretamente inicializado.
//                A posi��o atual na lista de �ndices livres deve ser v�lida.
// P�s-condi��es: A lista de posi��es livres no arquivo de �ndices � impressa na sa�da padr�o.
void imprimir_lista_indices_livres(FILE * file_indices, int pos_atual){
    CABECALHO_INDICES * cab_indices = le_cabecalho_indices(file_indices);

    if (pos_atual == -1) {
        free(cab_indices);
        return;
    }

    int prox;

    printf("+--------------------------------------------------+\n");
    printf("|  Lista de Posicoes Livres no Arquivo de Indices  |\n");
    printf("+--------------------------------------------------+\n");

    ARVOREB * no = ler_no(file_indices, pos_atual);
    printf("| %2d                                           |\n", pos_atual);

    while(no->prox_livre != -1 ){
        printf("| %2d                                           |\n", no->prox_livre);
        prox = no->prox_livre;
        free(no);
        no = ler_no(file_indices, prox);
    }

    printf("+--------------------------------------------------+\n");

    free(no);
    free(cab_indices);
}

// Fun��o para imprimir a lista de registros livres no arquivo de dados
// Pr�-condi��es: O arquivo de dados deve estar acess�vel e corretamente inicializado.
//                A posi��o atual na lista de registros livres deve ser v�lida.
// P�s-condi��es: A lista de posi��es livres no arquivo de dados � impressa na sa�da padr�o.
void imprimir_lista_registros_livres(FILE * file_dados, int pos_atual){
    CABECALHO_INDICES * cab_dados = le_cabecalho_indices(file_dados);

    if (pos_atual == -1) {
        free(cab_dados);
        return;
    }

    int prox;
    printf("\n---> Pos atual = %d\n", pos_atual);
    printf("+--------------------------------------------------+\n");
    printf("|  Lista de Posicoes Livres no Arquivo de Dados    |\n");
    printf("+--------------------------------------------------+\n");

    DADOS_REGISTRO * registro = ler_registro(file_dados, pos_atual);
    printf("| %2d                                               |\n", pos_atual);

    while(registro->prox_livre != -1 ){
        printf("| %2d                                               |\n", registro->prox_livre);
        prox = registro->prox_livre;
        free(registro);
        registro = ler_registro(file_dados, prox);
    }

    printf("+--------------------------------------------------+\n");

    free(registro);
    free(cab_dados);
}

// Fun��o para buscar o n�vel de um determinado n� na �rvore B-B
// Pr�-condi��es: O arquivo de �ndices deve estar acess�vel e corretamente inicializado.
//                O n� e a chave a serem buscados devem ser v�lidos.
// P�s-condi��es: O n�vel do n� na �rvore � retornado.
int buscaNivel(FILE* arq, ARVOREB* r, int chave, int count){
    if ( r!= NULL)
    {
        for (int i = 0; i < r->num_chaves; i++){
            if (r->chave[i] == chave)
            {
                return count;
            }
        }

        for (int i = 0; i < r->num_chaves; i++)
        {
            if (chave < r->chave[i])
            {
                ARVOREB* filho_i = ler_no(arq, r->filho[i]);
                if (filho_i != NULL)
                {
                    int retorno = 1 + buscaNivel(arq, filho_i, chave, count);
                    free(filho_i);
                    return retorno;
                }

            }
        }

        ARVOREB* filho_i = ler_no(arq, r->filho[r->num_chaves]);
        if (filho_i != NULL)
        {
            int retorno = 1 + buscaNivel(arq, filho_i, chave, count);
            free(filho_i);
            return retorno;
        }

    }
    return 0;
}

// Fun��o para buscar um n� espec�fico na �rvore B
// Pr�-condi��es: O arquivo de �ndices deve estar acess�vel e corretamente inicializado.
//                O n� e a chave a serem buscados devem ser v�lidos.
// P�s-condi��es: O n� contendo a chave � retornado, ou NULL se a chave n�o for encontrada.
ARVOREB * busca(FILE* arq, ARVOREB* r, int info, int * pos){
    if (r == NULL)
        return NULL;
    int i = 0;
    while (i < r->num_chaves && r->chave[i] < info) i++;
    if ((i + 1) > r->num_chaves|| r->chave[i] > info)
    {
        ARVOREB* aux = ler_no(arq, r->filho[i]);
        ARVOREB* b = busca(arq, aux, info, pos);
        if (b != aux)
        {
            free(aux);
        }
        return b;
    }
    *pos = i;
    return r;
}

// Fun��o para verificar se a fila est� vazia
// Pr�-condi��es: A fila deve estar corretamente inicializada.
// P�s-condi��es: Retorna 1 se a fila estiver vazia, caso contr�rio retorna 0.
int vaziaFila(Fila* f){
    return (f->inicio == NULL);
}


// Fun��o para enfileirar um elemento na fila
// Pr�-condi��es: A fila deve estar corretamente inicializada.
// P�s-condi��es: O elemento � enfileirado na fila.
void enqueue(Fila* f, int x) {
    struct noLista* aux = (struct noLista*)malloc(sizeof(struct noLista));
    aux->info = x;
    aux->prox = NULL;
    if (vaziaFila(f))
    {
        f->inicio = aux;
    }
    else
    {
        f->fim->prox = aux;
    }
    f->fim = aux;
}

// Fun��o para enfileirar os filhos de um n� na fila
// Pr�-condi��es: O arquivo de �ndices e o n� devem estar acess�veis e corretamente inicializados.
//                A fila deve estar corretamente inicializada.
// P�s-condi��es: Os filhos do n� s�o enfileirados na fila.
void enfileiraFilhos(FILE* arq, ARVOREB* r, Fila* f) {
    ARVOREB* aux = NULL;
    int p = 0;
    aux = busca(arq,r, f->inicio->info,&p);

    if (aux != NULL)
    {
        if (aux->filho[0] != -1)
        {
            int i;
            for (i = 0; i <= aux->num_chaves; i++)
            {
                ARVOREB* filho_i = ler_no(arq, aux->filho[i]);
                if (filho_i != NULL)
                {
                    enqueue(f, filho_i->chave[0]);
                }
                free(filho_i);
                filho_i = NULL;
            }

        }
    }
}

// Fun��o para desenfileirar um elemento da fila
// Pr�-condi��es: A fila deve estar corretamente inicializada e n�o vazia.
// P�s-condi��es: O elemento � desenfileirado da fila e retornado.
int * dequeue(Fila* f) {
    if (!vaziaFila(f)) {
        int* x = (int*)malloc(sizeof(int));

        struct noLista* aux = f->inicio;
        *x = f->inicio->info;

        if (f->inicio == f->fim)
        {
            f->fim = NULL;
        }
        f->inicio = f->inicio->prox;
        free(aux);
        return x;
    }
    else
    {
        return NULL;
    }
}

// Fun��o para criar uma fila vazia
// Pr�-condi��es: Nenhuma.
// P�s-condi��es: Retorna a fila vazia.
Fila* cria_fila_vazia() {
    Fila* f = (Fila*)malloc(sizeof(Fila));
    f->inicio = NULL;
    f->fim = NULL;
    return f;
}

// Fun��o para imprimir a �rvore B-B por n�veis
// Pr�-condi��es: Todos os arquivos e n�s devem estar acess�veis e corretamente inicializados.
//                O n� raiz da �rvore deve ser v�lido.
// P�s-condi��es: A �rvore B-B � impressa por n�veis na sa�da padr�o.
void imprimir_por_niveis(ARQUIVOS files, ARVOREB* r) {
    Fila* f = cria_fila_vazia();
    int atual = 0, ant = 0;
    if (r != NULL)
    {
        enqueue(f, r->chave[0]);
        while (!vaziaFila(f))
        {
            ARVOREB* aux = NULL;
            enfileiraFilhos(files.file_indices, r, f);
            atual = buscaNivel(files.file_indices, r, f->inicio->info, 0);
            if (atual != ant)
            {
                printf("\n");
                ant = atual;
            }
            int p = 0;
            aux = busca(files.file_indices, r, f->inicio->info, &p);
            int j;
            printf("[");
            for (j = 0; j < aux->num_chaves; j++) {
                printf("%d ", aux->chave[j]);
            }
            printf("] ");
            dequeue(f);
        }
    }
}

// Fun��o para imprimir a �rvore B-B
// Pr�-condi��es: O arquivo de �ndices deve estar acess�vel e corretamente inicializado.
// P�s-condi��es: A �rvore B-B � impressa na sa�da padr�o.
void imprimir_arvore(ARQUIVOS files) {
    CABECALHO_INDICES *cab_indices = le_cabecalho_indices(files.file_indices);
    int pos =  cab_indices->pos_raiz;
    if (pos == -1) {
        free(cab_indices);
        return;
    } else {
        ARVOREB *no = ler_no(files.file_indices, pos);
        printf("---> Imprimindo Arvore por niveis :\n\n");
        imprimir_por_niveis(files, no);
        printf("\n\n");
    }
}