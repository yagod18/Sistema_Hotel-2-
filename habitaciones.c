#include "habitaciones.h"
#include "huespedes.h"
#include "reservas.h"

//Funcion para crear una nueva habitacion
THabitacion NuevaHabitacion(){
    MostrarHabitacion();
    printf("\n------------ Cargando Nueva Habitacion -----------\n");
    THabitacion habitacion;

    int num_existe,i;

    do {
        printf("\nIngrese el numero de habitacion: ");
        fflush(stdin); scanf("%d", &habitacion.numero);
        num_existe = VerificarNumHabitacion(&habitacion.numero); //Se llama a la funcion VerificarNumHabitacion() para que no se repitan los numeros de habitacion
        if (num_existe){
            printf("\nEl numero de habitacion ingresado ya existe. Ingrese otro numero.\n");
        }else if(habitacion.numero <= 0){
            printf("\nEl Numero de habitacion no puede ser 0 (cero) o menor. Intente nuevamente\n");
        }

    }while (num_existe || habitacion.numero <= 0);

    do{
        printf("\nIngrese el tipo de habitacion ('Simple' 'Doble' o 'Suite'): ");
        fflush(stdin); gets(habitacion.tipo);
        for(i = 0; habitacion.tipo[i] != '\0'; i++){
            habitacion.tipo[i] = toupper(habitacion.tipo[i]);
        }
    }while(strcmp(habitacion.tipo,"SIMPLE") != 0 && strcmp(habitacion.tipo,"DOBLE") != 0 && strcmp(habitacion.tipo,"SUITE") != 0);


    do{
        printf("\nIngrese el precio por noche (Minimo = 100): ");
        fflush(stdin); scanf("%f",&habitacion.precio);
        if(habitacion.precio < 100){                     //Se establece un precio minimo de 100 por noche
            printf("\nIngrese un precio valido\n");
        }
    }while(habitacion.precio < 100);

    habitacion.estado = 0; //Se supone que al crear un nuevo registro de habitacion, la misma estara disponible
    return habitacion;
}

//Funcion para verificar que no se repita los numeros de las habitaciones
int VerificarNumHabitacion(int *num){
    FILE *habitaciones = fopen("Habitaciones.dat", "rb");
    THabitacion habitacion;
    int num_existe = 0;
    if (habitaciones != NULL) {
        while (fread(&habitacion, sizeof(THabitacion), 1, habitaciones)) {
            if (*num == habitacion.numero) {
                num_existe = 1;
                break;
            }
        }
        fclose(habitaciones);
    } else {
        printf("\nNo se pudo abrir el archivo 'Habitaciones', intente nuevamente\n");
    }
    fclose(habitaciones);
    return num_existe;
}

//Funcion para verificar si una habitacion esta ocupada
int VerificarEstado(int *num){
    FILE *habitaciones = fopen("Habitaciones.dat", "rb");
    THabitacion habitacion;

    int num_existe = 0;
    if (habitaciones != NULL) {
        while (fread(&habitacion, sizeof(THabitacion), 1, habitaciones)) {
            if (*num == habitacion.numero && habitacion.estado == 0) {
                num_existe = 1;
                break;
            }
        }
        fclose(habitaciones);
    } else {
        printf("\nNo se pudo abrir el archivo 'Habitaciones', intente nuevamente\n");
    }
    fclose(habitaciones);
    return num_existe;

}

//Proceso para cargar una nueva habitacion al archivo "Habitaciones.dat"
void CargarHabitacion(THabitacion habitacion[],int *tam2){
    FILE *habitaciones = fopen("Habitaciones.dat","ab");
    if(habitaciones != NULL){
        habitacion[*tam2] = NuevaHabitacion();
        fwrite(&habitacion[*tam2],sizeof(THabitacion), 1, habitaciones);
        (*tam2)++;
        printf(LGREEN"\nHabitacion cargado correctamente!\n"RESET);
        fclose(habitaciones);
    }else{
        printf("\nError al abrir el archivo 'Habitaciones', intente nuevamente\n");
    }
    fclose(habitaciones);
}

//Proceso para eliminar una habitacion segun su numero
void EliminarHabitacion(){
    FILE *habitaciones = fopen("Habitaciones.dat","rb");
    FILE *temp = fopen("tempHabitaciones","wb");
    THabitacion habitacion;

    int estado;
    int num,num_encontrado = 0;
    char op;

    if(habitaciones != NULL){
        do{
            fseek(habitaciones, 0, SEEK_END); // Ir al final del archivo
            if (ftell(habitaciones) == 0) { // Si el archivo está vacío
                printf("\nNo hay habitaciones para eliminar.\n"); //Si no hay habitaciones para eliminar se vuelve al menu anterior
                fclose(habitaciones);
                fclose(temp);
                remove("tempHabitaciones");
                return;
            }
            fseek(habitaciones, 0, SEEK_SET);

            MostrarHabitacion();

            printf("\nIngrese el numero de habitacion que quiere eliminar: ");
            fflush(stdin); scanf("%d",&num);
            num_encontrado = VerificarNumHabitacion(&num);
            estado = VerificarEstado(&num);
            if (!num_encontrado){
                printf("\nNumero no encontrado, intente nuevamente\n");
            }else if(!estado){
                printf("\nNo se puede eliminar una habitacion actualmente asociada a una reserva, si desea hacerlo primero cancele la reserva\n");
                fclose(habitaciones);
                fclose(temp);
                remove("tempHabitaciones");
                return;
            }
        }while (!num_encontrado || !estado);

        printf("\nSeguro que quiere eliminar la habitacion numero %d? 'S' para confirmar 'N' para salir\n", num);
        printf("\nIngrese una opcion: ");
        fflush(stdin); scanf(" %c", &op);
        op = toupper(op);
        if(op != 'S'){
            fclose(habitaciones);
            fclose(temp);
            remove("tempHabitaciones");
            return;
        }

        while(fread(&habitacion, sizeof(THabitacion), 1, habitaciones)){
            if(num != habitacion.numero){
                fwrite(&habitacion, sizeof(THabitacion), 1, temp);
            }
        }
        system("cls");
        printf("\x1B[38;2;255;0;0m""\nHabitacion eliminada correctamente!\n"RESET);
    }else{
        printf("\nNo se pudo abrir el archivo, intente nuevamente\n");
    }
    fclose(habitaciones);
    fclose(temp);
    remove("Habitaciones.dat");
    rename("tempHabitaciones","Habitaciones.dat");
}

//Proceso para listar todas las habitaciones
void MostrarHabitacion(){
    FILE *habitaciones = fopen("Habitaciones.dat","rb");
    FILE *reservas = fopen("Reservas.dat", "rb");

    THabitacion habitacion;

    if(habitaciones != NULL && reservas != NULL){
        printf(ORANGE"\n\n------------------- Habitaciones ---------------------\n"RESET);
        printf("\n Numero "ORANGE"|"RESET" Tipo    "ORANGE"|"RESET" Precio por noche "ORANGE"|"RESET" Estado actual "ORANGE"|"RESET"\n");
        printf(ORANGE"------------------------------------------------------\n"RESET);

        time_t now;
        struct tm *fechaActual;
        now = time(NULL);
        fechaActual = localtime(&now);

        int dd_actual = fechaActual->tm_mday;
        int mm_actual = fechaActual->tm_mon + 1;
        int aa_actual = fechaActual->tm_year + 1900;

       while (fread(&habitacion, sizeof(THabitacion), 1, habitaciones)) {
            if (!VerificarDisponibilidad(reservas, habitacion.numero, dd_actual, mm_actual, aa_actual, dd_actual, mm_actual, aa_actual)) {
                printf(" %-6d "ORANGE"|"RESET" %-7s "ORANGE"|"RESET" %-16.2f "ORANGE"|"RESET" %-13s "ORANGE"|"RESET"\n", habitacion.numero, habitacion.tipo, habitacion.precio, "Ocupado");
            } else {
                printf(" %-6d "ORANGE"|"RESET" %-7s "ORANGE"|"RESET" %-16.2f "ORANGE"|"RESET" %-13s "ORANGE"|"RESET"\n", habitacion.numero, habitacion.tipo, habitacion.precio, "Desocupado");
            }
        }

        fclose(habitaciones);
        fclose(reservas);
    } else {
        printf("\nNo se pudo abrir el archivo, intente nuevamente\n");
    }
}

//Proceso para modificar una habitacion segun su numero
void ModificarHabitacion(){
    FILE *habitaciones = fopen("Habitaciones.dat","rb");
    FILE *temp = fopen("tempHabitaciones","wb");
    THabitacion habitacion;

    int num,num_encontrado = 0;
    int op,i;
    char op2;
    float nuevo_precio;

    if(habitaciones != NULL){
        do{
            fseek(habitaciones, 0, SEEK_END); // Ir al final del archivo
            if (ftell(habitaciones) == 0) { // Si el archivo está vacío
                printf("\nNo hay habitaciones para modificar.\n"); //Si no hay habitaciones para modificar se vuelver al menu anteriior
                fclose(habitaciones);
                fclose(temp);
                remove("tempHabitaciones");
                return;
            }
            fseek(habitaciones, 0, SEEK_SET);

            MostrarHabitacion();
            printf("\nIngrese el numero de habitacion que quiera modificar: ");
            fflush(stdin); scanf("%d",&num);
            num_encontrado = VerificarNumHabitacion(&num);
            if (!num_encontrado){
                printf("\nNumero de habitacion no encontrado, intente nuevamente\n");
                }
        }while (!num_encontrado);

         while (fread(&habitacion, sizeof(THabitacion), 1, habitaciones)) {
            if (num == habitacion.numero) {
                system("cls");
                printf("\nHabitacion encontrada:");
                printf("\nNumero: %d\nTipo: %s\nPrecio: %.2f\n",habitacion.numero, habitacion.tipo, habitacion.precio);

                printf("\nSeleccione que desea modificar\n");
                printf("\n1) Tipo\n");
                printf("\n2) Precio\n");
                printf("\n0) Volver\n");
                printf("\nIngrese una opcion: ");
                fflush(stdin);
                scanf("%d", &op);
                system("cls");

                switch(op){
                    case 1:{
                            //Se actualiza el nuevo tipo de la habitacion
                            printf("\nEsta seguro que desea modificar el tipo de habitacion? Ingrese 'S' para seguir o 'N' para salir: ");
                            fflush(stdin); scanf("%c",&op2);
                            op2 = toupper(op2);
                            if(op2 == 'S'){
                                do{
                                    printf("\nIngrese el nuevo tipo de habitacion ('Simple' 'Doble' o 'Suite'): ");
                                    fflush(stdin); gets(habitacion.tipo);
                                    for(i = 0; habitacion.tipo[i] != '\0'; i++){
                                        habitacion.tipo[i] = toupper(habitacion.tipo[i]);
                                    }
                                }while(strcmp(habitacion.tipo,"SIMPLE") != 0 && strcmp(habitacion.tipo,"DOBLE") != 0 && strcmp(habitacion.tipo,"SUITE") != 0);
                                    printf(LGREEN"\nHabitacion modificada correctamente!\n"RESET);
                            }else{
                                fclose(habitaciones);
                                fclose(temp);
                                remove("tempHabitaciones");
                                return;
                            }
                        }break;
                    case 2:{
                            //Se actualiza el precio por noche de la habitacion, y si la misma esta reservada se actualiza en el archivo de "Reservas.dat" tambien
                            printf("\nEsta seguro que desea modificar el precio de la habitacion? Ingrese 'S' para seguir o 'N' para salir: ");
                            fflush(stdin); scanf("%c",&op2);
                            op2 = toupper(op2);
                            if(op2 == 'S'){
                                printf("\nIngrese el nuevo precio por noche de la habitacion: ");
                                fflush(stdin); scanf("%f", &nuevo_precio);
                                habitacion.precio = nuevo_precio;
                                if (!VerificarEstado(&num)){
                                    ActualizarPrecioReservas(&num, &nuevo_precio); //Aqui se actualiza el precio por noche y el total en el archivo "Reservas.dat"
                                }
                                printf(LGREEN"\nHabitacion modificada correctamente!\n"RESET);
                            }else{
                                fclose(habitaciones);
                                fclose(temp);
                                remove("tempHabitaciones");
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
            fwrite(&habitacion, sizeof(THabitacion), 1, temp);
        }
    }else{
        printf("\nNo se pudo abrir el archivo, intente nuevamente\n");
    }
    fclose(habitaciones);
    fclose(temp);
    remove("Habitaciones.dat");
    rename("tempHabitaciones","Habitaciones.dat");
}

//Proceso para actualizar el estado de una habitacion (ocupado o desocupado)
void ActualizarEstadoHabitacion(int num, int estado) {
    FILE *habitaciones = fopen("Habitaciones.dat", "rb+");
    THabitacion habitacion;
    if (habitaciones != NULL) {
        while (fread(&habitacion, sizeof(THabitacion), 1, habitaciones)) {
            if (habitacion.numero == num) {
                habitacion.estado = estado;
                fseek(habitaciones, -sizeof(THabitacion), SEEK_CUR);
                fwrite(&habitacion, sizeof(THabitacion), 1, habitaciones);
                fclose(habitaciones);
                break;
            }
        }
        fclose(habitaciones);
    } else {
        printf("\nNo se pudo abrir el archivo 'Habitaciones', intente nuevamente\n");
    }
    fclose(habitaciones);
}

//Proceso para actualizar el precio por noche y total de una reserva si se modifica el precio de la habitacion en ModificarHabitacion()
void ActualizarPrecioReservas(int *num_habitacion, float *nuevo_precio) {
    FILE *reservas = fopen("Reservas.dat", "rb+");
    TReserva reserva;

    int reservas_actualizadas = 0;

    if (reservas != NULL) {
        while (fread(&reserva, sizeof(TReserva), 1, reservas)) {
            if (*num_habitacion == reserva.num_habitacion) {
                reserva.precio_por_noche = *nuevo_precio;
                reserva.precio_total = reserva.precio_por_noche * reserva.cantidad_noches;

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

//Menu para gestionar las habitaciones
void GestionarHabitaciones(){
    THabitacion habitacion[MAX_REGISTROS];
    int tam2 = 0;
    int op;
    char op2;
    do{
        printf(ORANGE"\n--------------------------------------\n"RESET);
        printf(ORANGE"|       Gestion de Habitaciones      |"RESET);
        printf(ORANGE"\n--------------------------------------\n"RESET);
        printf("\n 1) Cargar nueva Habitacion\n");
        printf("\x1B[38;2;255;0;0m""\n 2) Eliminar habitacion segun su numero\n" RESET);
        printf("\n 3) Mostrar habitaciones\n");
        printf("\x1B[38;2;255;255;0m""\n 4) Modificar habitacion\n"RESET);
        printf("\n 0) Volver al menu principal\n");
        printf("\n Ingrese una opcion: ");
        fflush(stdin); scanf("%d",&op);
        system("cls");
        switch(op){
            case 1:
                printf("\nEsta seguro que desea cargar una habitacion? Ingrese 'S' para seguir o 'N' para salir: ");
                fflush(stdin); scanf("%c",&op2);
                op2 = toupper(op2);
                if(op2 == 'S'){
                    do{
                        CargarHabitacion(habitacion,&tam2);
                        printf("\nDesea cargar otra habitacion? 'N' para salir, 'S' para seguir: ");
                        fflush(stdin); scanf("%c",&op2);
                        op2 = toupper(op2);
                        system("cls");
                    }while(op2 != 'N' && tam2 <= MAX_REGISTROS);
                }
                break;
            case 2: EliminarHabitacion();
                break;
            case 3: MostrarHabitacion();
                break;
            case 4: ModificarHabitacion();
                break;
            case 0:
                break;
            default: printf("\nIngrese una opcion valida\n");
                break;
        }
    }while(op != 0);
}

