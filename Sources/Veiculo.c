#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Veiculo.h"
#include "Locacao.h"

char arquivoVeiculos[]="veiculos.dat";

void menuVeiculo()
{
    int op;
    printf("\n1 - Cadastrar veículo.\n");
    printf("2 - Veículos cadastrados.\n");
    printf("3 - Pesquisar dados do veículo pelo código.\n");
    printf("Escolha uma opção: ");
    scanf("%d",&op);
    printf("\n");
    printf("-------------------------------------------------\n");
    switch(op)
    {
    case 1:
        cadastrar_veiculo();
        break;
    case 2:
        lerArquivo();
        break;
    case 3:
        pesquisarDadosVeiculo();
        break;
    default:
        printf("Opção incorreta.\n");
    }
}

/**Cadastrar veículo.**/
void cadastrar_veiculo()
{
    int cod=0,codDigitado;

    FILE *arquivo;
    arquivo=fopen(arquivoVeiculos,"a+b");
    if(arquivo==NULL)
    {
        printf("Erro de abertura no arquivo veículos.");
        return;
    }

    VEICULO v;
    printf("\nCodigo do veiculo: ");
    scanf("%d",&v.codigo);
    /**Verificar se o código digitado já está cadastrado no sistema.**/
    codDigitado=v.codigo;
    do
    {
        cod=verifica_codigo(codDigitado);
        if(cod==1)
        {
            printf("Código já existente no sistema, digite outro: ");
            scanf("%d",&v.codigo);
            codDigitado=v.codigo;
        }
    }while (cod==1);
    /**Verificar se o código digitado já está cadastrado no sistema.**/

    printf("Descrição do carro: ");
    fflush(stdin);
    fgets(v.descricao,sizeof(VEICULO),stdin);
    printf("Modelo do carro: ");
    fflush(stdin);
    fgets(v.modelo,sizeof(VEICULO),stdin);
    printf("Cor do carro: ");
    fflush(stdin);
    fgets(v.cor,sizeof(VEICULO),stdin);
    printf("Placa: ");
    fflush(stdin);
    fgets(v.placa,sizeof(VEICULO),stdin);
    printf("Valor diária: ");
    scanf("%f",&v.valor_diaria);
    printf("Quantidade de ocupantes: ");
    scanf("%d",&v.quantidade_ocupantes);
    strcpy(v.status,"Disponivel\n");
    printf("\n-------------------------------------------------\n");

    fwrite(&v,sizeof(VEICULO),1,arquivo);
    fclose(arquivo);
}

/**Listar veículos cadastrados.**/
void lerArquivo()
{
    FILE *arquivo;
    arquivo=fopen(arquivoVeiculos,"rb");
    if(arquivo==NULL)
    {
        printf("Erro de abertura no arquivo dos veículos.");
        return;
    }

    VEICULO v;

    while(fread(&v,sizeof(VEICULO),1,arquivo)==1)
    {
        printf("Codigo do carro: %d\n",v.codigo);
        printf("Descrição do carro: %s",v.descricao);
        printf("Modelo do carro: %s",v.modelo);
        printf("Cor do carro: %s",v.cor);
        printf("Placa: %s",v.placa);
        printf("Valor diária: %.f\n",v.valor_diaria);
        printf("Quantidade de ocupantes: %d\n",v.quantidade_ocupantes);
        printf("Status: %s",v.status);
        printf("-------------------------------------------------\n");
    }

    fclose(arquivo);
}

/**Calcular a quantidade de carros disponíveis**/
int qtdDisponivel()
{
    int i=0;
    FILE *arquivo;
    arquivo=fopen(arquivoVeiculos,"rb");
    if(arquivo==NULL)
    {
        printf("Erro de abertura no arquivo dos veículos.");
        return 1;
    }

    VEICULO v;
    while(fread(&v,sizeof(VEICULO),1,arquivo)==1)
    {
        if(strcmp(v.status,"Disponivel\n")==0)
        {
            i++;
        }
    }

    return i;
    fclose(arquivo);
}

/**Listar veículos disponíveis**/
void lerVeiculosDisponiveis()
{
    FILE *arquivo;
    arquivo=fopen(arquivoVeiculos,"rb");
    if(arquivo==NULL)
    {
        printf("Erro de abertura no arquivo dos veículos.");
        return;
    }

    VEICULO v;
    while(fread(&v,sizeof(VEICULO),1,arquivo)==1)
    {
        if(strcmp(v.status,"Disponivel\n")==0)
        {
            printf("\nCodigo do carro: %d\n",v.codigo);
            printf("Descrição do carro: %s",v.descricao);
            printf("Modelo do carro: %s",v.modelo);
            printf("Cor do carro: %s",v.cor);
            printf("Placa: %s",v.placa);
            printf("Valor diária: %.f\n",v.valor_diaria);
            printf("Quantidade de ocupantes: %d\n",v.quantidade_ocupantes);
            printf("Status: %s",v.status);
            printf("-------------------------------------------------");
        }
    }

    fclose(arquivo);
}

/**Verificar se o código do veículo já está cadastrado.**/
int verifica_codigo(int codigoDigitado)
{
    int verificacao=0,codigoExistente;

    FILE *arquivo;
    arquivo=fopen(arquivoVeiculos,"rb");
    if(arquivo==NULL)
    {
        printf("Erro de abertura no arquivo dos veículos.");
        return 0;
    }

    VEICULO v;
    while(fread(&v,sizeof(VEICULO),1,arquivo)==1)
    {
        codigoExistente=v.codigo;
        if(codigoExistente==codigoDigitado)
        {
            verificacao=1;
            break;
        }
    }

    fclose(arquivo);
    return verificacao;
}

/**Alterar status para alugado.**/
void alterarStatus()
{
    FILE *arquivo;
    arquivo=fopen(arquivoVeiculos,"rb+");/**realizar leitura e escrita**/
    if(arquivo==NULL)
    {
        printf("Erro de abertura no arquivo dos veículos.");
        return;
    }

    rewind(arquivo);
    VEICULO v;
    while(fread(&v,sizeof(VEICULO),1,arquivo)==1)
    {
        if(v.codigo==codDigitado)
        {
            strcpy(v.status,"Alugado\n");
            long position = ftell(arquivo);/**Obter a posição atual do ponteiro no arquivo**/
            if(position - sizeof(VEICULO) >= 0)
            {
                fseek(arquivo,-sizeof(VEICULO),SEEK_CUR);
                fwrite(&v,sizeof(VEICULO),1,arquivo);
            }else
            {
                printf("Não é possível retroceder o arquivo.");
            }

            break;
        }
    }
    fclose(arquivo);
}

/**Alterar status do veículo para disponível, depois de dar baixa no sistema.**/
void alterarStatusBaixa()
{
    FILE *arquivo;
    arquivo=fopen(arquivoVeiculos,"rb+");/**realizar leitura e escrita**/
    if(arquivo==NULL)
    {
        printf("Erro de abertura no arquivo dos veículos.");
        return;
    }

    rewind(arquivo);
    VEICULO v;
    while(fread(&v,sizeof(VEICULO),1,arquivo)==1)
    {
        if(v.codigo==codDigitado)
        {
            strcpy(v.status,"Disponivel\n");
            long position = ftell(arquivo);/**Obter a posição atual do ponteiro no arquivo**/
            if(position - sizeof(VEICULO) >= 0)
            {
                fseek(arquivo,-sizeof(VEICULO),SEEK_CUR);
                fwrite(&v,sizeof(VEICULO),1,arquivo);
            }else
            {
                printf("Não é possível retroceder o arquivo.");
            }

            break;
        }
    }
    fclose(arquivo);
}

/**Calcular valor final com data de início e término**/
void calcularValorFinal()
{
    float valorDiaria,soma=0;
    FILE *arquivo;
    arquivo=fopen(arquivoVeiculos,"rb");
    if(arquivo==NULL)
    {
        printf("Erro de abertura no arquivo dos veículos.");
        return;
    }

    rewind(arquivo);
    VEICULO v;

    while(fread(&v,sizeof(VEICULO),1,arquivo)==1)
    {
        if(v.codigo==codDigitado)
        {
            valorDiaria=v.valor_diaria;
            if(seguro==1)
            {
                soma=Diarias*valorDiaria;
                soma=soma+50;
            }else
            {
                soma=Diarias*valorDiaria;
            }
            break;
        }
    }
    valorFinal=soma;

    fclose(arquivo);
}

void pesquisarDadosVeiculo()
{
    int cod,codNoSistema,verificacao;

    FILE *arquivo;
    arquivo=fopen(arquivoVeiculos,"rb");
    if(arquivo==NULL)
    {
        printf("Erro na abertura do arquivo veiculos.");
        return;
    }

    printf("\nDigite o código do veículo: ");
    scanf("%d",&cod);
    printf("\n");

    do
    {
        verificacao=verifica_codigo(cod);
        if(verificacao==0)
        {
            printf("Veiculo não encontrado, digite outro: ");
            scanf("%d",&cod);
            printf("\n");
        }
    }while (verificacao==0);

    VEICULO v;
    while(fread(&v,sizeof(VEICULO),1,arquivo)==1)
    {
        codNoSistema=v.codigo;
        if(cod==codNoSistema)
        {
            printf("-------------------------------------------------\n");
            printf("Codigo do carro: %d\n",v.codigo);
            printf("Descrição do carro: %s",v.descricao);
            printf("Modelo do carro: %s",v.modelo);
            printf("Cor do carro: %s",v.cor);
            printf("Placa: %s",v.placa);
            printf("Valor diária: %.f\n",v.valor_diaria);
            printf("Quantidade de ocupantes: %d\n",v.quantidade_ocupantes);
            printf("Status: %s",v.status);
            printf("-------------------------------------------------\n");
        }
    }

    fclose(arquivo);
}
