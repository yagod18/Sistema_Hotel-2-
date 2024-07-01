#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "habitaciones.h"
#include "huespedes.h"
#include "reservas.h"


int main()
{

    int op;
    do{
        printf("\n--------------------------------------\n");
        printf("|           Menu Principal           |");
        printf("\n--------------------------------------\n");
        printf("\n 1) Gestionar huespedes\n\n 2) Gestionar habitaciones\n\n 3) Gestionar reservas\n\n 0) Salir\n");
        printf("\n Ingrese una opcion: ");
        fflush(stdin); scanf("%d",&op);
        system("cls");
        switch(op){
            case 1: GestionarHuesped();
                break;
            case 2: GestionarHabitaciones();
                break;
            case 3: GestionarReservas();
                break;
            case 0:
                printf(LGREEN"\n---------------------------------\n"RESET);
                printf(LGREEN" Saliendo del programa..."RESET);
                printf(LGREEN"\n---------------------------------\n"RESET);
                break;
            default: printf("\n Ingrese una opcion valida\n");
                break;
        }
    }while(op != 0);

    return 0;
}
