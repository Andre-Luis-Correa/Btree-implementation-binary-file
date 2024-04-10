#include <stdio.h>
#include "menu.h"
#include "../insercao/produto_insercao.h"

// Exibe o cabe�alho do menu
// Pr�-condi��es: Nenhuma
// P�s-condi��o: O cabe�alho do menu � exibido no console
void header_menu(){
    printf("\nMenu:\n");
    printf("1. Cadastrar Produto\n");
    printf("2. Remover Produto\n");
    printf("3. Atualizar Preco\n");
    printf("4. Atualizar Estoque\n");
    printf("5. Imprimir Informacoes de um Produto\n");
    printf("6. Imprimir Lista de Todos os Produtos\n");
    printf("7. Imprimir Arvore\n");
    printf("8. Imprimir Lista de Livres do Arquivo de Indices\n");
    printf("9. Imprimir Lista de Livres do Arquivo de Dados\n");
    printf("10. Realizar Operacoes em Lote\n");
    printf("0. Sair\n");
}

// Cadastra um novo produto
// Pr�-condi��es: Nenhuma
// P�s-condi��o: Um novo produto � cadastrado
void cadastrarProduto(ARQUIVOS files) {
    printf("Operacao: Cadastrar Produto\n");
    cadastrar_produto(files);
    // Implemente a l�gica para cadastrar um produto
}

// Remove um produto existente
// Pr�-condi��es: Nenhuma
// P�s-condi��o: O produto � removido do cadastro
void removerProduto() {
    printf("Operacao: Remover Produto\n");
    // Implemente a l�gica para remover um produto
}

// Atualiza o pre�o de um produto
// Pr�-condi��es: Nenhuma
// P�s-condi��o: O pre�o do produto � atualizado
void atualizarPreco() {
    printf("Operacao: Atualizar Preco\n");
    // Implemente a l�gica para atualizar o pre�o de um produto
}

// Atualiza o estoque de um produto
// Pr�-condi��es: Nenhuma
// P�s-condi��o: O estoque do produto � atualizado
void atualizarEstoque() {
    printf("Operacao: Atualizar Estoque\n");
    // Implemente a l�gica para atualizar o estoque de um produto
}

// Imprime as informa��es de um produto espec�fico
// Pr�-condi��es: Nenhuma
// P�s-condi��o: As informa��es do produto s�o impressas no console
void imprimirInformacoes() {
    printf("Operacao: Imprimir Informacoes de um Produto\n");
    // Implemente a l�gica para imprimir as informa��es de um produto
}

// Imprime a lista de todos os produtos cadastrados
// Pr�-condi��es: Nenhuma
// P�s-condi��o: A lista de todos os produtos � impressa no console
void imprimirListaProdutos() {
    printf("Operacao: Imprimir Lista de Todos os Produtos\n");
    // Implemente a l�gica para imprimir a lista de todos os produtos
}

// Imprime a �rvore de produtos
// Pr�-condi��es: Nenhuma
// P�s-condi��o: A �rvore de produtos � impressa no console
void imprimirArvore() {
    printf("Operacao: Imprimir Arvore\n");
    // Implemente a l�gica para imprimir a �rvore de produtos
}

// Imprime a lista de n�s livres do arquivo de �ndices
// Pr�-condi��es: Nenhuma
// P�s-condi��o: A lista de n�s livres do arquivo de �ndices � impressa no console
void imprimirListaLivresIndices() {
    printf("Operacao: Imprimir Lista de Livres do Arquivo de Indices\n");
    // Implemente a l�gica para imprimir a lista de livres do arquivo de �ndices
}

// Imprime a lista de registros livres do arquivo de dados
// Pr�-condi��es: Nenhuma
// P�s-condi��o: A lista de registros livres do arquivo de dados � impressa no console
void imprimirListaLivresDados() {
    printf("Operacao: Imprimir Lista de Livres do Arquivo de Dados\n");
    // Implemente a l�gica para imprimir a lista de livres do arquivo de dados
}

// Realiza opera��es em lote a partir de um arquivo de texto
// Pr�-condi��es: O arquivo de texto contendo as opera��es em lote deve existir e estar formatado corretamente
// P�s-condi��o: As opera��es em lote s�o executadas
void realizarOperacoesEmLote() {
    printf("Operacao: Realizar Operacoes em Lote\n");
    // Implemente a l�gica para realizar opera��es em lote
}

// Exibe o menu principal e processa a escolha do usu�rio
// Pr�-condi��es: Nenhuma
// P�s-condi��o: O usu�rio escolhe uma op��o do menu e a opera��o correspondente � executada
void menu_principal(ARQUIVOS files){
    int opcao;

    do {
        header_menu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarProduto(files);
                break;
            case 2:
                removerProduto();
                break;
            case 3:
                atualizarPreco();
                break;
            case 4:
                atualizarEstoque();
                break;
            case 5:
                imprimirInformacoes();
                break;
            case 6:
                imprimirListaProdutos();
                break;
            case 7:
                imprimirArvore();
                break;
            case 8:
                imprimirListaLivresIndices();
                break;
            case 9:
                imprimirListaLivresDados();
                break;
            case 10:
                realizarOperacoesEmLote();
                break;
            case 0:
                printf("Saindo do programa. Ate mais!\n");
                break;
            default:
                printf("Opcao invalida! Escolha uma opcao valida.\n");
        }
    } while (opcao != 0);

    return;
}
