#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gerenciamento.h"

// Variáveis globais
Produto *produtos = NULL;
Transacao *transacoes = NULL;
int numProdutos = 0;
int numTransacoes = 0;
float fluxoCaixa = 0.0;

void salvarDados() {
    FILE *file = fopen("dados.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para salvar os dados.\n");
        return;
    }
    
    fwrite(&numProdutos, sizeof(int), 1, file);
    fwrite(&numTransacoes, sizeof(int), 1, file);
    fwrite(&fluxoCaixa, sizeof(float), 1, file);
    fwrite(produtos, sizeof(Produto), numProdutos, file);
    fwrite(transacoes, sizeof(Transacao), numTransacoes, file);
    fclose(file);
}

void carregarDados() {
    FILE *file = fopen("dados.txt", "r");
    if (file == NULL) {
        printf("Nenhum dado salvo encontrado. Iniciando com dados vazios.\n");
        return;
    }
    
    fread(&numProdutos, sizeof(int), 1, file);
    fread(&numTransacoes, sizeof(int), 1, file);
    fread(&fluxoCaixa, sizeof(float), 1, file);

    produtos = malloc(MAX_PRODUTOS * sizeof(Produto));
    transacoes = malloc(MAX_TRANSACOES * sizeof(Transacao));
    
    if (numProdutos > 0 && produtos) {
        fread(produtos, sizeof(Produto), numProdutos, file);
    }
    
    if (numTransacoes > 0 && transacoes) {
        fread(transacoes, sizeof(Transacao), numTransacoes, file);
    }

    fclose(file);
}

void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void limpaBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Implementação das funções de gerenciamento
void cadastrarProduto() { 
    Produto p;
    int tipo, unidadeOpcao;
    char nome[30];
    float precoCusto = 0, precoVenda = 0;
    float quantidadeOuPeso;
    int produtoExistente;

    if (numProdutos >= MAX_PRODUTOS) {
        printf("Erro: Capacidade maxima de produtos atingida.\n");
        return;
    }

    do {
        limparTela();
        printf("\n ----------------------------\n");
        printf("    CADASTRO DE PRODUTOS\n");
        printf(" ----------------------------\n");
        printf("  1. Fruta\n  2. Verdura\n  3. Legume\n  4. Hortalica\n  5. Voltar ao menu principal\n");
        printf(" Escolha uma opcao: ");
        if (scanf("%d", &tipo) != 1) {
            printf("Erro ao ler tipo de produto.\n");
        	limpaBuffer();
        	continue;
        }

        if (tipo < 1 || tipo > 5) {
            printf(" Opcao invalida! Tente novamente.\n");
        } else if (tipo != 5) {
            printf("\n Nome do produto: ");
            if (scanf(" %29[^\n]", nome) != 1) {
    			printf("Erro ao ler nome do produto.\n");
    			limpaBuffer();
    			continue;
			}
            limpaBuffer();
            
            produtoExistente = buscarProdutoPorNome(nome);
            if (produtoExistente != -1) {
                printf("Erro: Produto ja cadastrado.\n");	
                continue;
            }
            
            printf(" Escolha a unidade de medida:\n 1. (kg)\n 2. (g)\n 3. (mg)\n Opcao: ");
            if (scanf("%d", &unidadeOpcao) != 1) {
                printf("Erro ao ler unidade de medida.\n");
                limpaBuffer();
                continue;
            }

            switch (unidadeOpcao) {
                case 1:
                    strcpy(p.unidadeMedida, "kg");
                    printf(" Peso total (kg): ");
                    if (scanf("%f", &quantidadeOuPeso) != 1) {
                        printf("Erro ao ler peso.\n");
                        limpaBuffer();
                        continue;
                    }
                    p.kg = quantidadeOuPeso;
                    p.quantidade = quantidadeOuPeso; // Armazena também em gramas para consistência
                    break;
                case 2:
                    strcpy(p.unidadeMedida, "g");
                    printf(" Quantidade total (g): ");
                    if (scanf("%f", &quantidadeOuPeso) != 1) {
                        printf("Erro ao ler quantidade.\n");
                        limpaBuffer();
                        continue;
                    }
                    p.quantidade = quantidadeOuPeso;
                    p.kg = quantidadeOuPeso / 1000; // Armazena também em kg para consistência
                    break;
                case 3:
                    strcpy(p.unidadeMedida, "mg");
                    printf(" Quantidade total (mg): ");
                    if (scanf("%f", &quantidadeOuPeso) != 1) {
                        printf("Erro ao ler quantidade.\n");
                        limpaBuffer();
                        continue;
                    }
                    p.quantidade = quantidadeOuPeso;
                    p.kg = quantidadeOuPeso / 1000000; // Converte para kg
                    break;
                default:
                    printf(" Opcao invalida. Usando kg como padrao.\n");
                    strcpy(p.unidadeMedida, "kg");
                    printf(" Peso total (kg): ");
                    if (scanf("%f", &quantidadeOuPeso) != 1) {
                        printf("Erro ao ler peso.\n");
                        limpaBuffer();
                        continue;
                    }
                    p.kg = quantidadeOuPeso;
                    p.quantidade = quantidadeOuPeso * 1000;
            }
            limpaBuffer();

            printf(" Preco de custo: ");
            if (scanf("%f", &precoCusto) != 1) {
                printf("Erro ao ler preco de custo.\n");
                limpaBuffer();
                continue;
            }
            printf(" Preco de venda: ");
            if (scanf("%f", &precoVenda) != 1) {
                printf("Erro ao ler preco de venda.\n");
                limpaBuffer();
                continue;
            }
            limpaBuffer();	

            strcpy(p.nome, nome);
            p.precoCusto = precoCusto;
            p.precoVenda = precoVenda;
            p.tipo = tipo;

            produtos[numProdutos++] = p;
            printf("\n Produto %s cadastrado com sucesso!\n", p.nome);
            
            salvarDados();
        }
    } while (tipo != 5);
}

 
int buscarProdutoPorNome(const char *nome) {
    int i;
    for (i = 0; i < numProdutos; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
            return i; // Retorna o índice do produto encontrado
        }
    }
    return -1; // Retorna -1 se não encontrar o produto
}

void registrarTransacao(int tipoTransacao) { char nome[30];
    float quantidade;
    float valorTotal;
    int index;
    char continuar;

    const char *tipoStr = tipoTransacao == 0 ? "Compra" : "Venda";
    const int tipoOp = tipoTransacao;

    do {
        limparTela();
        printf("\n  --- Registrar %s ---\n", tipoStr);
        printf(" Digite o nome do produto: ");
        scanf(" %29[^\n]", nome);

        index = buscarProdutoPorNome(nome);
        if (index == -1) {
            printf(" Erro: Produto nao encontrado!\n");
            return;
        }

        Produto *p = &produtos[index];
        printf(" Digite a quantidade%s: ", strcmp(p->unidadeMedida, "kg") == 0 ? " em kg" : "");
        scanf("%f", &quantidade);

        // Verifica se e uma venda e se ha estoque suficiente
        if (tipoOp == 1) {  // Venda
            if ((strcmp(p->unidadeMedida, "kg") == 0 && p->kg < quantidade) ||
                (strcmp(p->unidadeMedida, "kg") != 0 && p->quantidade < quantidade)) {
                printf(" Erro: Estoque insuficiente!\n");
                return;
            }
        }

        // Calcula o valor total da transacao
        valorTotal = (tipoOp == 1 ? p->precoVenda : p->precoCusto) * quantidade;

        // Atualiza o estoque
        if (strcmp(p->unidadeMedida, "kg") == 0) {
            p->kg += (tipoOp == 0 ? quantidade : -quantidade);  // Adiciona na compra, subtrai na venda
        } else {
            p->quantidade += (tipoOp == 0 ? quantidade : -quantidade);  // Adiciona na compra, subtrai na venda
        }

        // Registra a transacao
        strcpy(transacoes[numTransacoes].nomeProduto, nome);
        strcpy(transacoes[numTransacoes].tipo, tipoStr);
        transacoes[numTransacoes].quantidade = quantidade;
        transacoes[numTransacoes].valorTotal = valorTotal;
        numTransacoes++;

        fluxoCaixa += (tipoOp == 0 ? -valorTotal : valorTotal);

        printf("\n Transacao registrada com sucesso! Novo fluxo de caixa: %.2f\n", fluxoCaixa);
        salvarDados();

        printf("\n Deseja registrar outra transacao? (S/N): ");
        scanf(" %c", &continuar);
    } while (continuar == 'S' || continuar == 's');}
    
void registrarDoacao() {  char nome[30];
    float quantidade;
    float valorTotal;
    int index;
    char continuar;

    do {
        limparTela();
        printf("\n  --- Registrar Doacao ---\n");
        printf(" Digite o nome do produto: ");
        scanf(" %29[^\n]", nome);

        index = buscarProdutoPorNome(nome);
        if (index == -1) {
            printf(" Erro: Produto nao encontrado!\n");
            return;
        }

        Produto *p = &produtos[index];
        printf(" Digite a quantidade%s: ", strcmp(p->unidadeMedida, "kg") == 0 ? " em kg" : "");
        scanf("%f", &quantidade);

        // Verifica se há estoque suficiente para a doação
        if ((strcmp(p->unidadeMedida, "kg") == 0 && p->kg < quantidade) ||
            (strcmp(p->unidadeMedida, "kg") != 0 && p->quantidade < quantidade)) {
            printf(" Erro: Estoque insuficiente para doação!\n");
            return;
        }

        // Calcula o valor total da doação com base no preço de custo
        valorTotal = p->precoCusto * quantidade;

        // Atualiza o estoque
        if (strcmp(p->unidadeMedida, "kg") == 0) {
            p->kg -= quantidade;
        } else {
            p->quantidade -= quantidade;
        }

        // Atualiza o fluxo de caixa com um saldo negativo
        fluxoCaixa -= valorTotal;

        // Registra a transação de doação
        Transacao novaTransacao;
        strcpy(novaTransacao.nomeProduto, p->nome);
        strcpy(novaTransacao.tipo, "Doacao");
        novaTransacao.quantidade = quantidade;
        novaTransacao.valorTotal = valorTotal;
        transacoes[numTransacoes++] = novaTransacao;

        printf("\n Doacao registrada com sucesso! Valor total da doacao (saida): R$ %.2f\n", valorTotal);
        printf(" Deseja registrar outra doacao? (s/n): ");
        scanf(" %c", &continuar);
    } while (continuar == 's' || continuar == 'S'); }
    
void visualizarEstoque() {
    int i;

    limparTela();
    printf("\n ----------------------------\n");
    printf("        ESTOQUE ATUAL\n");
    printf(" ----------------------------\n");

    if (numProdutos == 0) {
        printf("     Estoque esta vazio!\n");
    } else {
        for (i = 0; i < numProdutos; i++) {
            Produto p = produtos[i];

            if (strcmp(p.unidadeMedida, "kg") == 0) {
                printf(" Nome: %s | Tipo: %d | Quantidade: %.2f %s | Preco de venda: %.2f | Preco de custo: %.2f\n",
                       p.nome, p.tipo, p.kg, p.unidadeMedida, p.precoVenda, p.precoCusto);
            } else {
                printf(" Nome: %s | Tipo: %d | Quantidade: %.0f %s | Preco de venda: %.2f | Preco de custo: %.2f\n",
                       p.nome, p.tipo, p.quantidade, p.unidadeMedida, p.precoVenda, p.precoCusto);
            }
        }
    }}
void visualizarFluxoCaixa() {
    limparTela();
    printf("\n ----------------------------\n");
    printf("    	FLUXO DE CAIXA\n");
    printf(" ----------------------------\n");
    printf(" Fluxo de Caixa Atual: R$ %.2f\n", fluxoCaixa);}
void removerDados() { free(produtos);
    free(transacoes);

    produtos = malloc(MAX_PRODUTOS * sizeof(Produto));
    transacoes = malloc(MAX_TRANSACOES * sizeof(Transacao));

    numProdutos = 0;
    numTransacoes = 0;
    fluxoCaixa = 0.0;

    FILE *file = fopen("dados.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para remover os dados.\n");
        return;
    }

    fwrite(&numProdutos, sizeof(int), 1, file);
    fwrite(&numTransacoes, sizeof(int), 1, file);
    fwrite(&fluxoCaixa, sizeof(float), 1, file);

    fclose(file);  // Fechar o arquivo corretamente

    printf("Todos os dados foram removidos com sucesso!\n"); }

