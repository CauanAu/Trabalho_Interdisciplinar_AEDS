#ifndef VEICULO_H_INCLUDED
#define VEICULO_H_INCLUDED

typedef struct
{
    int codigo;
    char descricao[200];
    char modelo[20];
    char cor[20];
    char placa[10];
    float valor_diaria;
    int quantidade_ocupantes;
    char status[20];
} VEICULO;

int verifica_codigo(int codigoDigitado);
int qtdVeiculosDisponiveis;
void menuVeiculo();
void cadastrar_veiculo();
void lerArquivo();
void lerVeiculosDisponiveis();
int qtdDisponivel();
void alterarStatus();
void alterarStatusBaixa();
void calcularValorFinal();
void pesquisarDadosVeiculo();

#endif // VEICULO_H_INCLUDED
