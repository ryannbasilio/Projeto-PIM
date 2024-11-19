#include <stdio.h>
#include <stdlib.h>
#include "gerenciamento.h"

int main() {
    int opcao;

    // Alocação inicial de memória para os produtos e transações
    produtos = malloc(MAX_PRODUTOS * sizeof(Produto));
    transacoes = malloc(MAX_TRANSACOES * sizeof(Transacao));

    if (produtos == NULL || transacoes == NULL) {
        printf("Erro ao alocar memória para produtos ou transações.\n");
        return 1;
    }

    carregarDados();

    do {
        limparTela();
        printf("\n  ============================\n");
        printf("         MENU PRINCIPAL\n");
        printf("  ============================\n");
        printf("  1. Cadastrar Produto\n");
        printf("  2. Registrar Compra\n");
        printf("  3. Registrar Venda\n");
        printf("  4. Registrar Doacao\n");
        printf("  5. Exibir Estoque\n");
        printf("  6. Exibir Fluxo de Caixa\n");
        printf("  7. Remover Todos os Dados\n");
        printf("  8. Sair\n");
        printf(" Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarProduto();
                break;
            case 2:
                registrarTransacao(0);
                break;
            case 3:
                registrarTransacao(1);
                break;
            case 4:
                registrarDoacao();
                break;
            case 5:
                visualizarEstoque();
                break;
            case 6:
                visualizarFluxoCaixa();
                break;
            case 7:
                removerDados();
                break;
            case 8:
                salvarDados();
                printf("\n Programa encerrado.\n");
                break;
            default:
                printf(" Opcao invalida! Tente novamente.\n");
        }

        if (opcao != 8) {
            printf("\n Pressione Enter para continuar...");
            getchar();
            getchar();
        }
    } while (opcao != 8);

    free(produtos);
    free(transacoes);

    return 0;
}

