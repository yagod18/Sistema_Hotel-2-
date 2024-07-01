#include "habitaciones.h"
#include "huespedes.h"
#include "reservas.h"
#define MIN_DNI 6000000 //Se define un DNI minimo de 6 millones

//Funcion para crear un nuevo huesped
THuesped NuevoHuesped(){
    MostrarHuesped();
    printf("\n------------ Cargando Nuevo Huesped -----------\n");
    THuesped huesped;

    int id_existe, dni_existe,tel_existe;

    do {
        printf("\nIngrese el ID del huesped: ");
        fflush(stdin); scanf("%d", &huesped.id);
        id_existe = VerificarID(&huesped.id);    //Se llama a la funcion VerificarID() para verificar que no se repita el ID ingresado
        if (id_existe){
            printf("\nEl ID ingresado ya existe. Ingrese otro ID.\n");
        }else if(huesped.id <= 0){
            printf("\nEl ID no puede ser 0 (cero) o menor. Intente nuevamente\n");
        }

    }while (id_existe || huesped.id <= 0);

    do {
        printf("\nIngrese el DNI del huesped: ");
        fflush(stdin); scanf("%d", &huesped.dni);
        dni_existe = VerificarDNI(&huesped.dni); //Se llama a la funcion VerificarDNI() para verificar que no se repita el DNI ingresado
        if (dni_existe){
            printf("\nEl DNI ingresado ya existe. Ingrese otro DNI.\n");
        }else if(huesped.dni <= MIN_DNI){
            printf("\nIngrese un DNI valido\n");
        }

    }while (dni_existe || huesped.dni <= MIN_DNI);

    printf("\nIngrese el nombre y apellido del huesped: ");
    fflush(stdin); gets(huesped.nombre);

    printf("\nIngrese la direccion del huesped: ");
    fflush(stdin); gets(huesped.direccion);

    do {
        printf("\nIngrese el numero de telefono del huesped: ");
        fflush(stdin); gets(huesped.telefono);
        tel_existe = VerificarTelefono(huesped.telefono); //Se llama a la funcion VerificarDNI() para verificar que no se repita el DNI ingresado
        if (tel_existe){
            printf("\nEl telefono ingresado ya existe. Ingrese otro telefono.\n");
        }
    }while (tel_existe);

    return huesped;
}

//Funcion para verificar que no se repita el ID de los huespedes
int VerificarID(int *id){
    FILE *huespedes = fopen("Huespedes.dat", "rb");
    THuesped huesped;
    int id_existe = 0;
    if (huespedes != NULL) {
        while (fread(&huesped, sizeof(THuesped), 1, huespedes)) {
            if (*id == huesped.id) {
                id_existe = 1;
                break;
            }
        }
        fclose(huespedes);
    } else {
        printf("\nNo se pudo abrir el archivo 'Huespedes', intente nuevamente\n");
    }
    fclose(huespedes);
    return id_existe;
}

//Funcion para verificar que no se repita el DNI de los huespedes
int VerificarDNI(int *dni){
    FILE *huespedes = fopen("Huespedes.dat", "rb");
    THuesped huesped;
    int dni_existe = 0;
    if (huespedes != NULL) {
        while (fread(&huesped, sizeof(THuesped), 1, huespedes)) {
            if (*dni == huesped.dni) {
                dni_existe = 1;
                break;
            }
        }
        fclose(huespedes);
    } else {
        printf("\nNo se pudo abrir el archivo 'Huespedes', intente nuevamente\n");
    }
    return dni_existe;
}

//Funcion para verificar que no se repitan los numeros telefonicos
int VerificarTelefono(char *telefono){
    FILE *huespedes = fopen("Huespedes.dat", "rb");
    THuesped huesped;
    int telefon_existe = 0;
    if (huespedes != NULL) {
        while (fread(&huesped, sizeof(THuesped), 1, huespedes)) {
            if (strcmp(telefono,huesped.telefono) == 0) {
                telefon_existe = 1;
                break;
            }
        }
        fclose(huespedes);
    } else {
        printf("\nNo se pudo abrir el archivo 'Huespedes', intente nuevamente\n");
    }
    return telefon_existe;
}

//Funcion para ver si un huesped tiene una reserva
int VerificarHuespedReserva(int *id){
    FILE *reservas = fopen("Reservas.dat", "rb");
    TReserva reserva;

    int verificar = 0;
    if (reservas != NULL) {
        while (fread(&reserva, sizeof(TReserva), 1, reservas)) {
            if (*id == reserva.id_cliente && reserva.estado == 1) {
                verificar = 1;
                break;
            }
        }fclose(reservas);
    } else {
        printf("\nNo se pudo abrir el archivo 'Reservas', intente nuevamente\n");
    }
    return verificar;
}

//Proceso para cargar un huesped en el archivo 'Huespedes.dat'
void CargarHuesped(THuesped huesped[],int *tam){
    FILE *huespedes = fopen("Huespedes.dat","ab");
    if(huespedes != NULL){
        huesped[*tam] = NuevoHuesped();
        fwrite(&huesped[*tam],sizeof(THuesped), 1, huespedes);
        (*tam)++;
        printf(LGREEN"\nHuesped cargado correctamente!\n" RESET);
        fclose(huespedes);
    }else{
        printf("\nError al abrir el archivo 'Huespedes', intente nuevamente\n");
    }
}

//Proceso para eliminar un huesped segun su ID
void EliminarHuesped(){
    FILE *huespedes = fopen("Huespedes.dat","rb");
    FILE *temp = fopen("tempHuesped","wb");
    THuesped huesped;

    int id,id_encontrado = 0;
    char op;

    if(huespedes != NULL){
        do{
            fseek(huespedes, 0, SEEK_END); // Ir al final del archivo
            if (ftell(huespedes) == 0) { // Si el archivo está vacío
                printf("\nNo hay huespedes para eliminar.\n"); //Si no hay huespedes para eliminar se vuelve al menu anterior
                fclose(huespedes);
                fclose(temp);
                remove("tempHuespedes");
                return;
            }
            fseek(huespedes, 0, SEEK_SET);

            MostrarHuesped();

            printf("\nIngrese el ID del huesped que quiera eliminar: ");
            fflush(stdin); scanf("%d",&id);
            id_encontrado = VerificarID(&id);
            if (!id_encontrado){
                printf("\nID no encontrado, intente nuevamente\n");
            }else if(VerificarHuespedReserva(&id)){                     //Se llama a la funcion VerificarHuespedReserva() para ver si el huesped actualmente tiene una reserva
                printf("\nNo se puede eliminar un huesped que actualmente tiene realizada una reserva, si desea hacerlo primero cancele la reserva\n\n");
                fclose(huespedes);
                fclose(temp);
                remove("tempHuespedes.dat");
                return;
            }
        }while (!id_encontrado && VerificarHuespedReserva(&id));

        printf("\nSeguro que quiere eliminar al huesped de ID = %d? 'S' para confirmar 'N' para salir\n",id);
        printf("\nIngrese una opcion: ");
        fflush(stdin); scanf("%c",&op);
        op = toupper(op);
        if(op != 'S'){
            fclose(huespedes);
            fclose(temp);
            remove("tempHuespedes.dat");
            return;
        }

        while(fread(&huesped,sizeof(THuesped), 1, huespedes)){
            if(id != huesped.id){
                fwrite(&huesped, sizeof(THuesped), 1, temp);
            }
        }
        system("cls");
        printf("\x1B[38;2;255;0;0m""\nHuesped eliminado correctamente\n" RESET);
    }else{
        printf("\nNo se pudo abrir el archivo, intente nuevamente\n");
    }
    fclose(huespedes);
    fclose(temp);
    remove("Huespedes.dat");
    rename("tempHuesped","Huespedes.dat");
}

//Proceso para listar todos los huespedes
void MostrarHuesped(){
    FILE *huespedes = fopen("Huespedes.dat","rb");
    THuesped huesped;
    if(huespedes != NULL){
        printf(LGREEN"\n\n----------------------------------- HUESPEDES -----------------------------------\n"RESET);
        printf("\n ID  "LGREEN"|"RESET" Nombre  y Apellido   "LGREEN"|"RESET"    DNI    "LGREEN"|"RESET"   Direccion             "LGREEN"|"RESET" Telefono    "LGREEN"|"RESET"\n");
        printf(LGREEN"---------------------------------------------------------------------------------\n"RESET);
        while(fread(&huesped,sizeof(THuesped), 1, huespedes)){
            printf(" %-3d "LGREEN"|"RESET" %-20s "LGREEN"|"RESET" %-9d "LGREEN"|"RESET" %-23s "LGREEN"|"RESET" %-11s "LGREEN"|"RESET"\n",huesped.id, huesped.nombre, huesped.dni, huesped.direccion, huesped.telefono);
        }
        fclose(huespedes);
    }else{
        printf("\nNo se pudo abrir el archivo, intente nuevamente\n");
    }
    fclose(huespedes);
}

//Proceso para modificar los datos de un huesped segun su ID
void ModificarHuesped(){
    FILE *huespedes = fopen("Huespedes.dat","rb");
    FILE *temp = fopen("tempHuesped","wb");
    THuesped huesped;
    int id,id_encontrado = 0;
    int op;
    char op2;

    if(huespedes != NULL){
        do{
            fseek(huespedes, 0, SEEK_END); // Ir al final del archivo
            if (ftell(huespedes) == 0) { // Si el archivo está vacío
                printf("\nNo hay huespedes para modificar.\n"); //Si no hay huespedes para modificar se vuelve al menu anterior
                fclose(huespedes);
                fclose(temp);
                remove("tempHuespedes");
                return;
            }
            fseek(huespedes, 0, SEEK_SET);

            MostrarHuesped();
            printf("\nIngrese el ID del huesped que quiera modificar: ");
            fflush(stdin); scanf("%d",&id);
            id_encontrado = VerificarID(&id);
            if (!id_encontrado){
                printf("\nID no encontrado, intente nuevamente\n");
                fclose(huespedes);
                fclose(temp);
                remove("tempHuespedes.dat");
                return;
                }
        }while (!id_encontrado);

         while (fread(&huesped, sizeof(THuesped), 1, huespedes)) {
            if (id == huesped.id) {
                system("cls");
                printf("\nHuesped encontrado:");
                printf("\nID: %d\nNombre: %s\nDNI: %d\nDireccion: %s\nTelefono: %s\n", huesped.id, huesped.nombre, huesped.dni, huesped.direccion, huesped.telefono);

                printf("\nSeleccione que desea modificar \n\n1) Nombre \n\n2) DNI\n\n3) Direccion\n\n4) Telefono\n\n0) Volver\n");

                printf("\nIngrese una opcion: ");
                fflush(stdin);
                scanf("%d", &op);
                system("cls");

                switch(op){
                    case 1:{
                            printf("\nEsta seguro que desea modificar el nombre del huesped? Ingrese 'S' para seguir o 'N' para salir: ");
                            fflush(stdin); scanf("%c",&op2);
                            op2 = toupper(op2);
                            if(op2 == 'S'){
                                printf("\nIngrese el nuevo nombre del huesped: ");
                                fflush(stdin); gets(huesped.nombre);
                                ActualizarNombreReserva(id,huesped.nombre);
                                system("cls");
                                printf(LGREEN"\nHuesped modificado correctamente!\n"RESET);
                            }else{
                                fclose(huespedes);
                                fclose(temp);
                                remove("tempHuespedes.dat");
                                return;
                            }
                        }break;
                    case 2:{
                            printf("\nEsta seguro que desea modificar el DNI del huesped? Ingrese 'S' para seguir o 'N' para salir: ");
                            fflush(stdin); scanf("%c",&op2);
                            op2 = toupper(op2);
                            if(op2 == 'S'){
                                do{
                                    printf("\nIngrese el DNI del huesped: ");
                                    fflush(stdin); scanf("%d", &huesped.dni);
                                    if (VerificarDNI(&huesped.dni)){                    //Se llama a la funcion VerificarDNI() para verificar que no se repita el DNI ingresado
                                        printf("\nEl DNI ingresado ya existe. Ingrese otro DNI.\n");
                                    }else if(huesped.dni <= MIN_DNI){
                                        printf("\nIngrese un DNI valido\n");
                                    }
                                }while(VerificarDNI(&huesped.dni)|| huesped.dni <= MIN_DNI);
                                ActualizarDNIReserva(id, huesped.dni);
                                system("cls");
                                printf(LGREEN"\nHuesped modificado correctamente!\n"RESET);
                            }else{
                                fclose(huespedes);
                                fclose(temp);
                                remove("tempHuespedes.dat");
                                return;
                            }
                        }break;
                    case 3:{
                            //Modifcar direccion del huesped
                            printf("\nEsta seguro que desea modificar la direccion del huesped? Ingrese 'S' para seguir o 'N' para salir: ");
                            fflush(stdin); scanf("%c",&op2);
                            op2 = toupper(op2);
                            if(op2 == 'S'){
                                printf("\nIngrese la nueva direccion: ");
                                fflush(stdin); gets(huesped.direccion);
                                system("cls");
                                printf(LGREEN"\nHuesped modificado correctamente!\n"RESET);
                            }else{
                                fclose(huespedes);
                                fclose(temp);
                                remove("tempHuespedes.dat");
                                return;
                            }
                        }break;
                    case 4:{
                            //Modificar telefono del huesped
                            printf("\nEsta seguro que desea modificar el telefono del huesped? Ingrese 'S' para seguir o 'N' para salir: ");
                            fflush(stdin); scanf("%c",&op2);
                            op2 = toupper(op2);
                            if(op2 == 'S'){
                                printf("\nIngrese el nuevo telefono: ");
                                fflush(stdin); gets(huesped.telefono);
                                system("cls");
                                printf(LGREEN"\nHuesped modificado correctamente!\n"RESET);
                            }else{
                                fclose(huespedes);
                                fclose(temp);
                                remove("tempHuespedes.dat");
                                return;
                            }
                        }break;
                    case 0:
                        system("cls");
                        break;
                    default:
                        printf("\nOpcion no valida\n");
                        break;
                }
            }
            fwrite(&huesped, sizeof(THuesped), 1, temp);
        }
    }else{
        printf("\nNo se pudo abrir el archivo, intente nuevamente\n");
    }
    fclose(huespedes);
    fclose(temp);
    remove("Huespedes.dat");
    rename("tempHuesped","Huespedes.dat");
}

//Procedimiento para actualizar el nombre del huesped en una reserva
void ActualizarNombreReserva(int id, char *nombre){
    FILE *reservas = fopen("Reservas.dat", "rb+");
    TReserva reserva;
    int reservas_actualizadas = 0;

    if (reservas != NULL) {
        while (fread(&reserva, sizeof(TReserva), 1, reservas)) {
            if (id == reserva.id_cliente) {
                strcpy(reserva.nombre_huesped, nombre);
                fseek(reservas, -sizeof(TReserva), SEEK_CUR);
                fwrite(&reserva, sizeof(TReserva), 1, reservas);
                fflush(reservas); // Asegurarse de que los cambios se escriban en el archivo inmediatamente
                reservas_actualizadas++;
            }
        }
        fclose(reservas);

    } else {
        printf("\nNo se pudo abrir el archivo 'Reservas', intente nuevamente\n");
    }
}

//Funcion para actualizar el DNI de una o mas reservas
void ActualizarDNIReserva(int id, int dni){
    FILE *reservas = fopen("Reservas.dat", "rb+");
    TReserva reserva;
    int reservas_actualizadas = 0;

    if (reservas != NULL) {
        while (fread(&reserva, sizeof(TReserva), 1, reservas)) {
            if (id == reserva.id_cliente) {
                reserva.dni_cliente = dni;
                fseek(reservas, -sizeof(TReserva), SEEK_CUR);
                fwrite(&reserva, sizeof(TReserva), 1, reservas);
                fflush(reservas); // Asegurarse de que los cambios se escriban en el archivo inmediatamente
                reservas_actualizadas++;
            }
        }
        fclose(reservas);
    } else {
        printf("\nNo se pudo abrir el archivo 'Reservas', intente nuevamente\n");
    }
}

//Menu para gestionar a los huespedes
void GestionarHuesped(){
    THuesped huesped[MAX_REGISTROS];
    int tam = 0;
    int op;
    char op2;
    do{
        printf(LGREEN "\n--------------------------------------\n" RESET);
        printf(LGREEN "|         Gestion de Huespedes       |" RESET);
        printf(LGREEN "\n--------------------------------------\n" RESET);
        printf("\n 1) Cargar nuevo Huesped\n");
        printf("\x1B[38;2;255;0;0m""\n 2) Eliminar Huesped segun ID\n" RESET);
        printf("\n 3) Mostrar Huespedes\n");
        printf("\x1B[38;2;255;255;0m""\n 4) Modificar Huesped\n"RESET);
        printf("\n 0) Volver al menu principal\n");
        printf("\n Ingrese una opcion: ");
        fflush(stdin); scanf("%d",&op);
        system("cls");
        switch(op){
            case 1:
                  printf("\nEsta seguro que desea cargar un huesped? Ingrese 'S' para seguir o 'N' para salir: ");
                  fflush(stdin); scanf("%c",&op2);
                  op2 = toupper(op2);
                  if(op2 == 'S'){
                      do{
                            CargarHuesped(huesped,&tam);
                            printf("\nDesea cargar otro huesped? 'N' para salir, 'S' para seguir: ");
                            fflush(stdin); scanf("%c",&op2);
                            op2 = toupper(op2);
                            system("cls");

                    }while(op2 != 'N' && tam <= MAX_REGISTROS);
                }
                break;
            case 2: EliminarHuesped();
                break;
            case 3: MostrarHuesped();
                break;
            case 4: ModificarHuesped();
                break;
            case 0:
                break;
            default: printf("\nIngrese una opcion valida\n");
                break;
        }
    }while(op != 0);
}
