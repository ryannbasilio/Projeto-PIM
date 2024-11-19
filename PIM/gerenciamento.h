#ifndef GERENCIAMENTO_H
#define GERENCIAMENTO_H

#define MAX_PRODUTOS 100
#define MAX_TRANSACOES 200
#define ESTOQUE_MINIMO 10

typedef struct {
    char nome[30];
    float precoCusto;
    float precoVenda;
    float quantidade;
    float kg;
    int tipo;
    char unidadeMedida[5];
} Produto;

typedef struct {
    char nomeProduto[30];
    char tipo[10];
    float quantidade;
    float valorTotal;
} Transacao;

// Variáveis globais
extern Produto *produtos;
extern Transacao *transacoes;
extern int numProdutos;
extern int numTransacoes;
extern float fluxoCaixa;

// Funções
void salvarDados();
void carregarDados();
void limparTela();
void limpaBuffer();
void cadastrarProduto();
int buscarProdutoPorNome(const char *nome);
void registrarTransacao(int tipoTransacao);
void registrarDoacao();
void visualizarEstoque();
void visualizarFluxoCaixa();
void removerDados();

#endif

