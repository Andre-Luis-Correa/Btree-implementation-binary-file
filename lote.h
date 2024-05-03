#ifndef LOTE_H
#define LOTE_H

#include "produto.h"
#include "arvore_utils.h"
#include "produto_insercao.h"
#include "produto_atualizacao.h"
#include "produto_remocao.h"

// Insere um lote de produtos no sistema
// Pr�-condi��es: Os arquivos de �ndices e de dados devem existir e estar formatados corretamente
// P�s-condi��es: Os produtos do lote s�o inseridos no sistema, caso n�o existam ainda
void inserir_lote(ARQUIVOS files, PRODUTO_DATA produto);

// Atualiza o pre�o de um lote de produtos no sistema
// Pr�-condi��es: Os arquivos de �ndices e de dados devem existir e estar formatados corretamente
// P�s-condi��es: O pre�o dos produtos do lote s�o atualizados no sistema
void atualizar_preco_lote(ARQUIVOS files, int codigo, float preco);

// Atualiza o estoque de um lote de produtos no sistema
// Pr�-condi��es: Os arquivos de �ndices e de dados devem existir e estar formatados corretamente
// P�s-condi��es: O estoque dos produtos do lote s�o atualizados no sistema
void atualizar_estoque_lote(ARQUIVOS files, int codigo, int novo_estoque);

// Remove um lote de produtos do sistema
// Pr�-condi��es: Os arquivos de �ndices e de dados devem existir e estar formatados corretamente
// P�s-condi��es: Os produtos do lote s�o removidos do sistema, caso existam
void remover_lote(ARQUIVOS files, int codigo);

// Processa uma linha do arquivo contendo informa��es sobre um lote de produtos
// Pr�-condi��es: Os arquivos de �ndices e de dados devem existir e estar formatados corretamente
// P�s-condi��es: As opera��es correspondentes ao tipo de linha s�o realizadas no sistema
void processar_linha(ARQUIVOS files, char *linha);

// L� um arquivo contendo lotes de produtos e processa suas linhas
// Pr�-condi��es: Os arquivos de �ndices e de dados devem existir e estar formatados corretamente
// P�s-condi��es: As opera��es correspondentes �s linhas do arquivo s�o realizadas no sistema
void ler_arquivo(ARQUIVOS files, char *nome_arquivo);

#endif //LOTE_H
