#ifndef DISTRIBUICAO_H
#define DISTRIBUICAO_H
#include "disciplina.h"
#include "file.h"
#include "curso.h"
#include "disciplina.h"
#include "professor.h"

#define MAXKEY 10

// Estrutura que representa uma distribui��o.
typedef struct {
    char cod_distribuicao[MAXKEY];  // C�digo �nico da distribui��o.
    int cod_disciplina;  // C�digo da disciplina associada � distribui��o.
    char ano_letivo[5];  // Ano letivo ao qual a distribui��o est� relacionada.
    int cod_professor;  // C�digo do professor respons�vel pela disciplina na distribui��o.
} DISTRIBUICAO;

// Estrutura de n� para a �rvore de distribui��es.
typedef struct {
    DISTRIBUICAO distribuicao;  // Informa��es da distribui��o.
    int esq;  // �ndice do n� � esquerda na �rvore.
    int dir;  // �ndice do n� � direita na �rvore.
} NO_DISTRIBUICAO;

// Fun��o para ler os dados de uma distribui��o do usu�rio.
// Pr�-condi��o: Nenhuma.
// P�s-condi��o: Retorna uma distribui��o lida do usu�rio.
DISTRIBUICAO ler_distribuicao();

// L� um n� da distribui��o do arquivo bin�rio.
// Pr�-condi��o: O arquivo de distribui��o deve estar aberto e ser um arquivo bin�rio v�lido.
//               A posi��o (pos) deve ser uma posi��o v�lida no arquivo.
// P�s-condi��o: Retorna um ponteiro para um novo n� lido do arquivo na posi��o especificada.
NO_DISTRIBUICAO * le_no_distribuicao (FILE * file_distribuicao, int pos);

// Fun��o para buscar uma distribui��o no arquivo com base na chave.
// Pr�-condi��o: O arquivo file_distribuicao deve estar aberto.
// P�s-condi��o: Retorna a posi��o no arquivo onde a distribui��o com a chave foi encontrada.
int buscar_distribuicao(FILE *file_distribuicao, char chave[MAXKEY], int pos);

// Fun��o para formatar o c�digo da distribui��o.
// Pr�-condi��o: Nenhuma.
// P�s-condi��o: O par�metro 'codigo' � preenchido com o c�digo formatado.
void formatar_codigo(int codigo_disciplina, char ano_letivo[10], char codigo[10]);

// Fun��o para inserir uma distribui��o.
// Pr�-condi��o: O arquivo file_distribuicao deve estar aberto.
// P�s-condi��o: Insere uma nova distribui��o no arquivo.
void inserir_distribuicao(FILE *file_distribuicao);

// Fun��o para inserir uma distribui��o proveniente do arquivo lote txt no arquivo bin�rio.
// Pr�-condi��o: O arquivo file_distribuicao deve estar aberto.
// P�s-condi��o: Insere uma nova distribui��o no arquivo.
void inserir_distribuicao_do_lote(FILE* file_distribuicao, DISTRIBUICAO distribuicao);

// Fun��o para inserir uma distribui��o no arquivo.
// Pr�-condi��o: O arquivo file_distribuicao deve estar aberto.
// P�s-condi��o: Insere um novo n� no arquivo de distribui��es.
void inserir_distribuicao_file(FILE *file_distribuicao, NO_DISTRIBUICAO *no_distribuicao);

// Fun��o para escrever um n� no arquivo.
// Pr�-condi��o: O arquivo file_distribuicao deve estar aberto.
// P�s-condi��o: Escreve o n� no arquivo de distribui��es na posi��o especificada.
void escreve_no_distribuicao(FILE *file_distribuicao, NO_DISTRIBUICAO *no_distribuicao, int pos);

// Fun��o para inserir um n� sem considerar n�s livres.
// Pr�-condi��o: O arquivo file_distribuicao deve estar aberto.
// P�s-condi��o: Insere um novo n� no arquivo sem considerar n�s livres.
void inserir_sem_nos_livres(FILE *file_distribuicao, NO_DISTRIBUICAO *no_distribuicao, int pos);

// Fun��o para inserir um n� considerando n�s livres.
// Pr�-condi��o: O arquivo file_distribuicao deve estar aberto.
// P�s-condi��o: Insere um novo n� no arquivo considerando n�s livres.
void inserir_com_nos_livres(FILE *file_distribuicao, NO_DISTRIBUICAO *no_distribuicao, int pos);

// Fun��o para imprimir as informa��es de uma distribui��o.
// Pr�-condi��o: Nenhuma.
// P�s-condi��o: Imprime as informa��es do n� de distribui��o.
void imprimir_info_distribuicao(ARQUIVOS files, NO_DISTRIBUICAO * no_distribuicao);

// Fun��o para imprimir as distribui��es em ordem no arquivo.
// Pr�-condi��o: O arquivo file_distribuicao deve estar aberto.
// P�s-condi��o: Imprime as distribui��es do arquivo em ordem a partir da posi��o especificada.
void imprimir_in_order_distribuicao(ARQUIVOS files, FILE* file_distribuicao, int pos_atual);

// Fun��o para imprimir a lista de distribui��es no arquivo.
// Pr�-condi��o: Os arquivos no struct ARQUIVOS devem estar abertos.
// P�s-condi��o: Imprime a lista de distribui��es do arquivo.
void imprimir_lista_distribuicao(ARQUIVOS files);

// Fun��o para imprimir a lista de posi��es livres no arquivo.
// Pr�-condi��o: O arquivo file_distribuicao deve estar aberto.
// P�s-condi��o: Imprime a lista de posi��es livres no arquivo a partir da posi��o especificada.
void imprimir_posicoes_livres(FILE *file_distribuicao, int pos_atual);

// Fun��o para imprimir a lista de posi��es livres no arquivo.
// Pr�-condi��o: O arquivo file_distribuicao deve estar aberto.
// P�s-condi��o: Imprime a lista de posi��es livres no arquivo.
void imprimir_posicoes_livres_wrapper(FILE *file_distribuicao);

// Fun��o para remover uma distribui��o.
// Pr�-condi��o: O arquivo file_distribuicao deve estar aberto.
// P�s-condi��o: Remove uma distribui��o do arquivo.
void retira_distribuicao(FILE *file_distribuicao);

// Fun��o para troca de distribui��es
// Pr�-condi��o: O arquivo file_distribuicao deve estar aberto, pos_destino e pos_origem devem ser posi�oes validas no arquivo
// P�s-condi��o: Troca as informa��es entre dois n�s no arquivo.
void trocar_info(FILE *file_distribuicao, int pos_destino, int pos_origem);

// Fun��o para remover um n� do arquivo.
// Pr�-condi��o: O arquivo file_distribuicao deve estar aberto.
// P�s-condi��o: Remove um n� do arquivo e retorna a nova posi��o do n� de substitui��o.
int remover_no_distribuicao(FILE *file_distribuicao, int pos, char chave[MAXKEY]);

// Fun��o para encontrar o n� m�ximo em uma sub�rvore.
// Pr�-condi��o: O arquivo file_distribuicao deve estar aberto.
// P�s-condi��o: Encontra e imprime a distribui��o com a chave m�xima na sub�rvore especificada.
void maximo(FILE *file_distribuicao, int pos, char chave[MAXKEY]);

// Fun��o para encontrar o n� m�nimo em uma sub�rvore.
// Pr�-condi��o: O arquivo file_distribuicao deve estar aberto.
// P�s-condi��o: Encontra e imprime a distribui��o com a chave m�nima na sub�rvore especificada.
void minimo(FILE *file_distribuicao, int pos, char chave[MAXKEY]);

// Fun��o para imprimir os n�veis da �rvore de distribuicao com suas respectivas chaves.
// Pr�-condi��o: O arquivo file_distribuicao deve estar aberto.
// P�s-condi��o: imprime os n�veis da �rvore na tela ou mensagem de �rvore vazia.
void imprimir_niveis(FILE * file_distribuicao);

#endif //DISTRIBUICAO_H
