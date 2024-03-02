#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Modulos.h"
#include "Locacao.h"
#include "Veiculo.h"
#include "QuantidadeDiarias.h"

char arquivoLocacao[]="locacao.dat";

void menuLocacao()
{
    int op;
    printf("\n1 - Cadastrar loca��o.\n");
    printf("2 - Lista de todos contratos cadastrados no sistema.\n");
    printf("3 - Encerrar contrato de loca��o.\n");
    printf("4 - Pesquisar contratos de loca��o.\n");
    printf("5 - Pesquisar se cliente pode ser contemplado com kit LocaMais.\n");
    printf("Escolha uma op��o: ");
    scanf("%d",&op);
    printf("\n-------------------------------------------------\n");
    switch(op)
    {
    case 1:
        cadastrar_locacao();
        break;
    case 2:
        lista_locacoes();
        break;
    case 3:
        baixaNaLocacao();
        break;
    case 4:
        pesquisarLocacoesClientes();
        break;
    case 5:
        pesquisarPontosTotais();
        break;
    default:
        printf("Op��o incorreta.\n");
    }
}

/**Cadastrar loca��es.**/
void cadastrar_locacao()
{
    int verificaNome,cod;
    FILE *arquivo;
    arquivo=fopen(arquivoLocacao,"a+b");
    if(arquivo==NULL)
    {
        printf("Erro na abertura no arquivo loca��o.");
        return;
    }

    LOCACAO ca;

    printf("\nNome do cliente: ");
    fflush(stdin);
    fgets(ca.nome,sizeof(LOCACAO),stdin);
    verificaNome=verifica_Cliente(ca.nome);
    ca.codigoCliente=codigoClienteGlobal;
    if(verificaNome==1)
    {
        printf("C�digo do cliente cadastrado: %d\n",ca.codigoCliente);
    }
    if(verificaNome!=1)
    {
        printf("Cliente n�o cadastrado.\n");
        return;
    }

    printf("C�digo da loca��o: ");
    scanf("%d",&ca.codigo);
    codDigitado=ca.codigo;
    do
    {
        cod=verificarCodigoLocacao(codDigitado);
        if(cod==1)
        {
            printf("C�digo j� existente no sistema, digite outro: ");
            scanf("%d",&ca.codigo);
            codDigitado=ca.codigo;
        }
    }while (cod==1);

    printf("Data de retirada\n");
    printf("Dia: ");
    scanf("%d",&ca.dia1);
    printf("M�s: ");
    scanf("%d",&ca.mes1);
    printf("Ano: ");
    scanf("%d",&ca.ano1);

    printf("Data de devolu��o\n");
    printf("Dia: ");
    scanf("%d",&ca.dia2);
    printf("M�s: ");
    scanf("%d",&ca.mes2);
    printf("Ano: ");
    scanf("%d",&ca.ano2);

    /**Calcular a quantidade de dias que o carro vai ficar alugado.**/
    Diarias=diferencaEntreDatas(ca.dia1,ca.mes1,ca.ano1,ca.dia2,ca.mes2,ca.ano2);
    programaFidelidade(Diarias);
    ca.pontosFidelidade=pontos;

    printf("Seguro - Digite 1 para sim ou 0 para n�o: ");
    scanf("%d",&ca.seguro);
    seguro=ca.seguro;

    printf("Quantidade de ocupantes: ");
    scanf("%d",&ca.qtd_ocupantes);

    /**Mostrar os ve�culos dispon�veis.**/
    qtdVeiculosDisponiveis=qtdDisponivel();
    if(qtdVeiculosDisponiveis>0)
    {
        printf("\nLista de ve�culos dispon�veis - \n");
        lerVeiculosDisponiveis();
    }else
    {
        printf("\nNenhum ve�culo dispon�vel.\n");
        return;
    }


    printf("\n\nC�digo do ve�culo a ser alugado: ");
    scanf("%d",&ca.codigoVeiculo);
    codDigitado=ca.codigoVeiculo;
    printf("\n-------------------------------------------------\n");

    alterarStatus();/**Altera o status do ve�culo para alugado**/
    calcularValorFinal();/**Calcula o valor final referente a data de in�cio e fim**/
    ca.valor_final=valorFinal;
    ca.qtd_diarias=Diarias;

    fwrite(&ca,sizeof(LOCACAO),1,arquivo);
    fclose(arquivo);
}

/**Lista de loca��es cadastradas.**/
void lista_locacoes()
{
    char seg[5];
    FILE *arquivo;
    arquivo=fopen(arquivoLocacao,"rb");
    if(arquivo==NULL)
    {
        printf("Erro na abertura no arquivo loca��o.");
        return;
    }

    LOCACAO ca;
    while(fread(&ca,sizeof(LOCACAO),1,arquivo)==1)
    {
        printf("Nome do cliente: %s",ca.nome);
        printf("C�digo da loca��o: %d\n",ca.codigo);
        if(ca.seguro==1)
        {
            strcpy(seg,"sim");
        }else
        {
            strcpy(seg,"nao");
        }
        printf("Seguro: %s\n",seg);
        printf("Quantidade de ocupantes: %d\n",ca.qtd_ocupantes);
        printf("C�digo do ve�culo: %d\n",ca.codigoVeiculo);
        printf("Data de retirada: %d/%d/%d\n",ca.dia1,ca.mes1,ca.ano1);
        printf("Data de devolu��o: %d/%d/%d\n",ca.dia2,ca.mes2,ca.ano2);
        printf("Quantidade de di�rias: %d\n",ca.qtd_diarias);
        printf("Valor a pagar: %.f\n",ca.valor_final);
        printf("Pontos acumulados: %d\n",ca.pontosFidelidade);
        printf("-------------------------------------------------\n");
    }

    fclose(arquivo);
}

/**Verificar se j� existe o c�digo da loca��o digitado**/
int verificarCodigoLocacao(int codDigitado)
{
    int verificacao=0,codigoExistente;

    FILE *arquivo;
    arquivo=fopen(arquivoLocacao,"rb");
    if(arquivo==NULL)
    {
        printf("Erro de abertura no arquivo dos ve�culos.");
        return 0;
    }

    LOCACAO ca;
    while(fread(&ca,sizeof(LOCACAO),1,arquivo)==1)
    {
        codigoExistente=ca.codigo;
        if(codigoExistente==codDigitado)
        {
            verificacao=1;
            break;
        }
    }

    fclose(arquivo);
    return verificacao;
}

/**Fun��o para encerrar contrato de loca��o.**/
void baixaNaLocacao()
{
    LOCACAO ca;
    BAIXA b;
    int verificaNome,codExistente;
    float juros,valorFinalJuros;
    printf("\nDigite o c�digo da loca��o: ");
    scanf("%d",&b.codigo);
    codDigitado=b.codigo;
    /**Verificar se o nome do cliente digitado existe no sistema.**/
    verificaNome=verificaCliente_contrato(codDigitado);
    if(verificaNome!=1)
    {
        printf("\nContrato n�o encontrado.\n");
        printf("\n-------------------------------------------------\n");
        return;
    }
    /**Verificar se o nome do cliente digitado existe no sistema.**/

    printf("Data prevista de devolu��o - \n");
    printf("Dia: ");
    scanf("%d",&b.dia);
    printf("M�s: ");
    scanf("%d",&b.mes);
    printf("Ano: ");
    scanf("%d",&b.ano);
    encontrarDataDevolucao(codDigitado);
    diasDeAtraso=0;
    diasDeAtraso=diferencaEntreDatas(diaDevolucao,mesDevolucao,anoDevolucao,b.dia,b.mes,b.ano);

    FILE *arquivo;
    arquivo=fopen(arquivoLocacao,"r+b");
    if(arquivo==NULL)
    {
        printf("Erro de abertura no arquivo dos contratos de loca��o.");
        return;
    }

    while(fread(&ca,sizeof(LOCACAO),1,arquivo)==1)
    {
        codExistente=ca.codigo;
        if(codDigitado==codExistente)
        {
            if(diasDeAtraso>0)
            {
                valorFinalJuros=ca.valor_final;
                juros=(5.0/100.0);
                juros=valorFinalJuros*juros;
                valorFinalJuros+=juros;
                for(int i=0;i<diasDeAtraso;i++)
                {
                    valorFinalJuros+=30.0;
                }
            }else if(diasDeAtraso==0)
            {
                valorFinalJuros=ca.valor_final;
            }else if(diasDeAtraso<0)
            {
                valorFinalJuros=ca.valor_final;
                juros=valorFinalJuros*0.1;
                valorFinalJuros-=juros;
            }

            b.valor_final=valorFinalJuros;
            codDigitado=ca.codigoVeiculo;
            alterarStatusBaixa();

            printf("Dias de atraso: %d\n",diasDeAtraso);
            printf("Valor a pagar pelo Cliente: %.2f\n",b.valor_final);
            printf("\n-------------------------------------------------\n");

            break;
        }
    }

    fclose(arquivo);
}

/**Econtrar data de devolu��o cadastrada no sistema.**/
void encontrarDataDevolucao(int codDigitado)
{
    LOCACAO ca;

    int codExistente;

    FILE *arquivo;
    arquivo=fopen(arquivoLocacao,"rb");
    if(arquivo==NULL)
    {
        printf("Erro de abertura no arquivo dos ve�culos.");
        return;
    }

    while(fread(&ca,sizeof(LOCACAO),1,arquivo)==1)
    {
        codExistente=ca.codigo;
        if(codExistente==codDigitado)
        {
            diaDevolucao=ca.dia2;
            mesDevolucao=ca.mes2;
            anoDevolucao=ca.ano2;
            break;
        }
    }

    fclose(arquivo);
}

/**Verificar se o c�digo de loca��o existe no sistema.**/
int verificaCliente_contrato(int codDigitado)
{
    int verificacao=0,codigoExistente;

    FILE *arquivo;
    arquivo=fopen(arquivoLocacao,"rb");
    if(arquivo==NULL)
    {
        printf("Erro de abertura no arquivo dos contratos de loca��o.");
        return 0;
    }

    LOCACAO ca;

    while(fread(&ca,sizeof(LOCACAO),1,arquivo)==1)
    {
        codigoExistente=ca.codigo;
        if(codigoExistente==codDigitado)
        {
            verificacao=1;
            break;
        }
    }

    fclose(arquivo);
    return verificacao;
}

/**Pesquisar a loca��o dos clientes pelo c�digo deles.**/
void pesquisarLocacoesClientes()
{
    int cod,codNoSistema,verificacao;
    char seg[5];

    FILE *arquivo;
    arquivo=fopen(arquivoLocacao,"rb");
    if(arquivo==NULL)
    {
        printf("Erro de abertura no arquivo dos contratos de loca��o.");
        return;
    }

    printf("\nDigite o c�digo do cliente: ");
    scanf("%d",&cod);
    printf("\n");
    do
    {
        verificacao=verificaCodigo(cod);
        if(verificacao==0)
        {
            printf("Cliente n�o encontrado, digite outro: ");
            scanf("%d",&cod);
            printf("\n");
        }
    }while (verificacao==0);
    printf("-------------------------------------------------\n");

    LOCACAO c;
    while(fread(&c,sizeof(LOCACAO),1,arquivo)==1)
    {
        codNoSistema=c.codigoCliente;
        {
            if(cod==codNoSistema)
            {
                printf("Nome do cliente: %s",c.nome);
                printf("C�digo da loca��o: %d\n",c.codigo);
                if(c.seguro==1)
                {
                    strcpy(seg,"sim");
                }else
                {
                    strcpy(seg,"nao");
                }
                printf("Seguro: %s\n",seg);
                printf("Quantidade de ocupantes: %d\n",c.qtd_ocupantes);
                printf("C�digo do ve�culo: %d\n",c.codigoVeiculo);
                printf("Data de retirada: %d/%d/%d\n",c.dia1,c.mes1,c.ano1);
                printf("Data de devolu��o: %d/%d/%d\n",c.dia2,c.mes2,c.ano2);
                printf("Quantidade de di�rias: %d\n",c.qtd_diarias);
                printf("Valor a pagar: %.f\n",c.valor_final);
                printf("Pontos de fidelidade: %d\n",c.pontosFidelidade);
                printf("-------------------------------------------------\n");
            }
        }
    }

    fclose(arquivo);
}

/**Calcular a qtd de pontos de uma loca��o.**/
void programaFidelidade(int qtdDiarias)
{
    pontos=0;
    FILE *arquivo;
    arquivo=fopen(arquivoLocacao,"rb");
    if(arquivo==NULL)
    {
        printf("Erro de abertura no arquivo dos contratos de loca��o.");
        return;
    }

    for(int i=0;i<qtdDiarias;i++)
    {
        pontos=pontos+10;
    }

    fclose(arquivo);
}

void pesquisarPontosTotais()
{
    int codExistente,soma=0,codigoDigitado,verificacao,qtdAcumuladoDePremios,recebido;
    FILE *arquivo;
    arquivo=fopen(arquivoLocacao,"rb+");
    if(arquivo==NULL)
    {
        printf("Erro de abertura no arquivo dos contratos de loca��o.");
        return;
    }

    printf("\nDigite o c�digo do cliente: ");
    scanf("%d",&codigoDigitado);
    do
    {
        verificacao=verificaCodigo(codigoDigitado);
        if(verificacao==0)
        {
            printf("\nCliente n�o encontrado, digite outro: ");
            scanf("%d",&codigoDigitado);
        }
    }while (verificacao==0);

    LOCACAO c;
    while(fread(&c,sizeof(LOCACAO),1,arquivo)==1)
    {
        codExistente=c.codigoCliente;
        if(codExistente==codigoDigitado)
        {
            soma=soma+c.pontosFidelidade;
        }
    }

    if(soma<500)
    {
        printf("\nCliente ainda n�o possui pontos necess�rios.\n");
        printf("\n-------------------------------------------------\n");
    }else if(soma>=500)
    {
        qtdAcumuladoDePremios=soma/500;
        printf("Cliente possui pontos necess�rios.\n");
        printf("Quantidade de pr�mios acumulados: %d\n",qtdAcumuladoDePremios);
        printf("Digite 1 para entregue ou 0 para n�o: ");
        scanf("%d",&recebido);
        printf("\n-------------------------------------------------\n");

        if (recebido == 1) {
            fseek(arquivo, 0, SEEK_SET); // Reposiciona o ponteiro do arquivo no in�cio
            while (fread(&c, sizeof(LOCACAO), 1, arquivo) == 1) {
                if (c.codigoCliente == codigoDigitado) {
                    c.pontosFidelidade = 0; // Altera os pontos acumulados para 0
                    fseek(arquivo, -sizeof(LOCACAO), SEEK_CUR); // Reposiciona o ponteiro antes da escrita
                    fwrite(&c, sizeof(LOCACAO), 1, arquivo); // Escreve os dados atualizados
                    fseek(arquivo, 0, SEEK_CUR); // Move o ponteiro para o pr�ximo registro
                }
            }
            printf("Pontos acumulados atualizados para 0 ap�s a entrega do pr�mio.\n");
        }
    }

    fclose(arquivo);
}
