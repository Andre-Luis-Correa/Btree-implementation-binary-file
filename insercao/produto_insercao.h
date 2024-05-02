//
// Created by andre on 09/04/2024.
//

#ifndef PRODUTO_INSERCAO_H
#define PRODUTO_INSERCAO_H

#include "../produto/produto.h"
#include "../file/file.h"
#include "../cabecalho/cabecalho_indices.h"
#include "../cabecalho/cabecalho_dados.h"

// Fun��o para ler os dados de um produto
// Pr�-condi��o: nenhuma
// P�s-condi��o: retorna um ponteiro para a estrutura PRODUTO_DATA preenchida com os dados inseridos pelo usu�rio
PRODUTO_DATA * ler_produto();

// Verifica se um n� da �rvore B est� em overflow
// Pr�-condi��o: o n� deve existir
// P�s-condi��o: retorna 1 se o n�mero de chaves for igual � ordem da �rvore, 0 caso contr�rio
int overflow (ARVOREB * r);

// Fun��o para cadastrar um produto na �rvore B
// Pr�-condi��o: o arquivo de �ndices deve existir e estar aberto para escrita
// P�s-condi��o: o produto � cadastrado na �rvore B e nos dados, se o c�digo do produto ainda n�o existir na �rvore
void cadastrar_produto(ARQUIVOS files);

// Cria um novo n� na �rvore B
// Pr�-condi��o: nenhuma
// P�s-condi��o: o n� � criado com as informa��es passadas por par�metro
void cria_no(ARVOREB * r, int codigo, int pos_dados);

// Cria um novo registro com os dados de um produto
// Pr�-condi��o: nenhuma
// P�s-condi��o: o registro � criado com os dados do produto
void cria_registro(DADOS_REGISTRO * dados, PRODUTO_DATA * produto);

// Fun��o para dividir um n� da �rvore B em dois
// Pr�-condi��o: o n� x deve existir e n�o estar vazio
// P�s-condi��o: divide o n� x em dois n�s, retornando a posi��o do novo n� criado
int split(ARQUIVOS files, ARVOREB * x, int pos, int * meio, int * pos_meio, CABECALHO_INDICES * cab_indices);

// Obt�m a pr�xima posi��o livre no arquivo de dados
// Pr�-condi��o: o arquivo de dados deve estar aberto para leitura e escrita
// P�s-condi��o: retorna a pr�xima posi��o livre no arquivo de dados
int get_pos_livre_dados(FILE * file, CABECALHO_DADOS * cab);

// Obt�m a pr�xima posi��o livre no arquivo de �ndices
// Pr�-condi��o: o arquivo de �ndices deve estar aberto para leitura e escrita
// P�s-condi��o: retorna a pr�xima posi��o livre no arquivo de �ndices
int get_pos_livre_indices(FILE * file, CABECALHO_INDICES * cab);

// Fun��o principal para cadastrar um produto na �rvore B
// Pr�-condi��o: o arquivo de �ndices e o arquivo de dados devem existir e estar abertos para escrita
// P�s-condi��o: o produto � cadastrado na �rvore B e nos dados, se o c�digo do produto ainda n�o existir na �rvore
void cadastrar_produto_file(ARQUIVOS files, PRODUTO_DATA * produto);

// Fun��o para encontrar a posi��o onde uma chave deve ser inserida em um n� da �rvore B
// Pr�-condi��o: o n� r deve existir e n�o estar vazio
// P�s-condi��o: retorna 1 se a chave j� existe no n�, caso contr�rio retorna 0 e atualiza a posi��o onde a chave deve ser inserida
int busca_pos(ARVOREB * r, int codigo, int * pos);

// Fun��o para adicionar uma chave e seu ponteiro de dados � direita de uma posi��o espec�fica em um n� da �rvore B
// Pr�-condi��o: o n� r deve existir e n�o estar cheio
// P�s-condi��o: a chave e seu ponteiro de dados s�o inseridos na posi��o especificada
void adiciona_direita(ARVOREB * r, int pos, int codigo, int pt_dados, int p);

// Fun��o auxiliar para cadastrar um produto na �rvore B
// Pr�-condi��o: o arquivo de �ndices deve existir e estar aberto para escrita
// P�s-condi��o: o produto � cadastrado na �rvore B e nos dados, se o c�digo do produto ainda n�o existir na �rvore
void cadastrar_aux(ARQUIVOS files, ARVOREB * r, int codigo, int pt_dados, int pos_atual, CABECALHO_INDICES * cab_indices);


#endif //PRODUTO_INSERCAO_H
