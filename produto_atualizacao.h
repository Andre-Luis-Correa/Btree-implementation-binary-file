#ifndef PRODUTO_ATUALIZACAO_H
#define PRODUTO_ATUALIZACAO_H

#include "produto.h"
#include "file.h"
#include "cabecalho_indices.h"
#include "cabecalho_dados.h"

// Fun��o para atualizar o pre�o de um produto a partir do seu c�digo
// Pr�-condi��es: O arquivo de �ndices e o arquivo de dados devem existir e estar formatados corretamente
// P�s-condi��es: O pre�o do produto � atualizado no arquivo de dados
void atualizar_preco_produto(ARQUIVOS files, int codigo_produto, float novo_preco);

// Fun��o para atualizar o estoque de um produto a partir do seu c�digo
// Pr�-condi��es: O arquivo de �ndices e o arquivo de dados devem existir e estar formatados corretamente
// P�s-condi��es: O estoque do produto � atualizado no arquivo de dados
void atualizar_estoque_produto(ARQUIVOS files, int codigo_produto, int novo_estoque);

#endif //PRODUTO_ATUALIZACAO_H