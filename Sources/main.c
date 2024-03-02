#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "Modulos.h"
#include "Veiculo.h"
#include "Locacao.h"

int main()
{
    setlocale(LC_ALL,"Portuguese");
    int op;

    printf("Trabalho Interdisciplinar - Alice Martins, Cauan Augusto e Vitor Ferreira.\n");
    printf("Locadora LocaMais.\n");
    do
    {
        menu(&op);
        switch(op)
        {
        case 1:
            menuCliente();
            break;
        case 2:
            menuVeiculo();
            break;
        case 3:
            menuLocacao();
            break;
        case -1:
            printf("Encerrando o programa.\n");
            break;
        default:
            printf("Opção incorreta.\n");
        }
    }while(op!=-1);

    return 0;
}
