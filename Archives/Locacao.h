#ifndef LOCACAO_H_INCLUDED
#define LOCACAO_H_INCLUDED

typedef struct
{
    char nome[50];
    int codigo;
    int codigoCliente;
    int dia1;
    int mes1;
    int ano1;
    int dia2;
    int mes2;
    int ano2;
    int seguro;
    int qtd_ocupantes;
    int codigoVeiculo;
    float valor_final;
    int qtd_diarias;
    int pontosFidelidade;
} LOCACAO;

typedef struct
{
    int codigo;
    float valor_final;
    int dia;
    int mes;
    int ano;
} BAIXA;

int codDigitado;
int Diarias;
int seguro;
int diasDeAtraso;
float valorFinal;
int diaDevolucao;
int mesDevolucao;
int anoDevolucao;

void menuLocacao();
void cadastrar_locacao();
void lista_locacoes();

void baixaNaLocacao();
void encontrarDataDevolucao(int codDigitado);
int verificarCodigoLocacao(int codDigitado);
int verificaCliente_contrato(int cod);

void pesquisarLocacoesClientes();
int codigoClienteGlobal;

void programaFidelidade(int qtdDiarias);
void pesquisarPontosTotais();
int pontos;

#endif // LOCACAO_H_INCLUDED
