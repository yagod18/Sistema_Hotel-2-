#include "habitaciones.h"
#include "huespedes.h"
#include "reservas.h"


//Funcion para crear una nueva reserva
TReserva NuevaReserva(){

    FILE *reservas = fopen("Reservas.dat", "rb");

    TReserva reserva;
    MostrarHabitacion();
    MostrarHuesped();

    time_t now;
    struct tm *fechaActual;
    char fechaStr[11];

    // Obtener la fecha actual
    now = time(NULL);
    fechaActual = localtime(&now);

    // Guardar la fecha actual en la estructura reserva
    reserva.aa_in = fechaActual->tm_year + 1900;
    reserva.mm_in = fechaActual->tm_mon + 1;
    reserva.dd_in = fechaActual->tm_mday;
    struct tm fechaCheckIn = { 0 };

    int id_reserva,id_existe,num_habitacion;
    char *nombre_huesped;

    printf("\n\n------------ Cargando Nueva Reserva -----------\n");
    do{
        printf("\nIngrese el ID de la reserva: ");
        fflush(stdin); scanf("%d", &reserva.id_reserva);
        id_reserva = VerificarIDReserva(&reserva.id_reserva); //Se llama a la funcion VerificarIDReserva() para que no se repitan los ID ingresados
        if (id_reserva){
            printf("\nEl ID de reserva ingresado ya existe. Ingrese otro ID.\n");
        }else if(reserva.id_reserva <= 0){
            printf("\nEl ID de reserva no puede ser 0 (cero) o menor. Intente nuevamente\n");
        }
    }while (id_reserva || reserva.id_reserva <= 0);

    FILE *huespedes = fopen("Huespedes.dat", "rb");

    fseek(huespedes, 0, SEEK_END); // Ir al final del archivo
    if (ftell(huespedes) == 0) { // Si el archivo est� vac�o
        printf("\nNo hay huespedes para hacer la reserva, por favor cargue primero un huesped.\n");//Si no existen huespedes se pide primero cargar uno para poder hacer al menos una reserva
        fclose(huespedes);
        reserva.id_reserva = 0;
        return reserva;
    }
    fseek(huespedes, 0, SEEK_SET);


    do {
        printf("\nIngrese el ID del huesped: ");
        scanf("%d", &reserva.id_cliente);
        id_existe = VerificarID(&reserva.id_cliente);
        if (!id_existe){
            printf("\nID no encontrado. Ingrese otro ID.\n");
        }

        nombre_huesped = ObtenerNombreHuesped(&reserva.id_cliente);
        if (nombre_huesped != NULL) {
                strcpy(reserva.nombre_huesped, nombre_huesped);  //Se guarda el nombre del huepsed
        }
        reserva.dni_cliente = ObtenerDNI(&reserva.id_cliente); //Se guarda el DNI del huesped

    }while (!id_existe);

    FILE *habitaciones = fopen("Habitaciones.dat", "rb");

    fseek(habitaciones, 0, SEEK_END); // Ir al final del archivo
    if (ftell(habitaciones) == 0) { // Si el archivo est� vac�o
        printf("\nNo hay habitaciones para reservar, por favor cargue primero una habitacion.\n");//Si no existen habitaciones se pide primero cargar uno para poder hacer al menos una reserva
        fclose(habitaciones);
        reserva.id_reserva = 0;
        return reserva;
    }
    fseek(habitaciones, 0, SEEK_SET);

    do {
        printf("\nIngrese el numero de habitacion: ");
        scanf("%d", &reserva.num_habitacion);
        num_habitacion = VerificarNumHabitacion(&reserva.num_habitacion);
        if (!num_habitacion){
            printf("\nEl numero de habitacion no existe. Ingrese otro numero.\n");
        }
        reserva.precio_por_noche = ObtenerPrecioPorNoche(&reserva.num_habitacion); //Se almacena el precio por noche de la habitacion

    }while (!num_habitacion);

    MostrarReservasPorHabitacion(reserva.num_habitacion);

    do{
        printf("\n\nIngrese la fecha de Check In (dd/mm/aa): ");
        fflush(stdin); scanf(" %10[^\n]", fechaStr);  // Leer la fecha como cadena
        fflush(stdin); sscanf(fechaStr, "%d/%d/%d", &reserva.dd_in, &reserva.mm_in, &reserva.aa_in);

        if(!ValidarFecha(reserva.dd_in,reserva.mm_in,reserva.aa_in)){
            printf("\nIngrese una fecha valida\n");
        }else{

            fechaCheckIn.tm_year = reserva.aa_in - 1900;
            fechaCheckIn.tm_mon = reserva.mm_in - 1;
            fechaCheckIn.tm_mday = reserva.dd_in;
            time_t checkInTime = mktime(&fechaCheckIn);

            if (difftime(checkInTime, now) <= 0) {
                printf("\nLa fecha de Check In debe ser mayor que la fecha actual.\n");
            }
        }
    }while(!ValidarFecha(reserva.dd_in, reserva.mm_in, reserva.aa_in) || difftime(mktime(&fechaCheckIn), now) <= 0);

    do{
        printf("\nIngrese la fecha de Check Out (dd/mm/aa): ");
        fflush(stdin); scanf(" %10[^\n]", fechaStr);  // Leer la fecha como cadena
        fflush(stdin); sscanf(fechaStr, "%d/%d/%d", &reserva.dd_out, &reserva.mm_out, &reserva.aa_out);

        if(!ValidarFecha(reserva.dd_out,reserva.mm_out,reserva.aa_out)){
            system("cls");
            printf("\nIngrese una fecha valida\n");
        }else if (CompararFechas(reserva.dd_in, reserva.mm_in, reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out) >= 0){
            system("cls");
            printf("\nLa fecha de Check Out debe ser mayor que la fecha de Check In\n");
        }

    }while(!ValidarFecha(reserva.dd_out, reserva.mm_out, reserva.aa_out) || CompararFechas(reserva.dd_in, reserva.mm_in, reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out) >= 0);

    if (!VerificarDisponibilidad(reservas, reserva.num_habitacion, reserva.dd_in, reserva.mm_in, reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out)){
            system("cls");
            printf("\nla habitacion no esta disponible para el rango de fechas proporcionado.\n");
            reserva.id_reserva = 0;
            return reserva;
    }

    fclose(habitaciones);
    fclose(huespedes);
    fclose(reservas);

    reserva.cantidad_noches = CalcularDiferenciaDias(reserva.dd_in, reserva.mm_in, reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out); //Se calcula la cantidad de noches segun las fechas y se guarda en reservas.cantidad_noches
    reserva.precio_total = reserva.precio_por_noche * reserva.cantidad_noches; //Se calcula el precio total de la reserva
    reserva.estado = 1;
    return reserva;
}

//Funcion para calcular los dias entre dos fechas
int CalcularDiferenciaDias(int dd1, int mm1, int aa1, int dd2, int mm2, int aa2){

    int diasPorMes[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (EsBisiesto(aa1)) {
        diasPorMes[1] = 29;
    }

    int dias = 0;
    while (dd1 != dd2 || mm1 != mm2 || aa1 != aa2) {
        dd1++;
        if (dd1 > diasPorMes[mm1 - 1]) {
            dd1 = 1;
            mm1++;
            if (mm1 > 12) {
                mm1 = 1;
                aa1++;
                if (EsBisiesto(aa1)) {
                    diasPorMes[1] = 29;
                } else {
                    diasPorMes[1] = 28;
                }
            }
        }
        dias++;
    }
    return dias;
}

//Funcion para verificar si un a�o es bisiesto
int EsBisiesto(int aa){
    if ((aa % 4 == 0 && aa % 100 != 0) || (aa % 400 == 0)) {
        return 1;
    }
    return 0;
}

// Funci�n para comparar dos fechas
int CompararFechas(int dd1, int mm1, int aa1, int dd2, int mm2, int aa2){

    if (aa1 < aa2) return -1;
    if (aa1 > aa2) return 1;
    if (mm1 < mm2) return -1;
    if (mm1 > mm2) return 1;
    if (dd1 < dd2) return -1;
    if (dd1 > dd2) return 1;

    return 0;
}

//Funcion para verificar fechas
int ValidarFecha(int dd, int mm, int aa){
    if (aa < 2024 || aa > 2030) {
        return 0;
    }
    if (mm < 1 || mm > 12) {
        return 0;
    }
    if (dd < 1 || dd > 31) {
        return 0;
    }
    if (mm == 4 || mm == 6 || mm == 9 || mm == 11) {
        if (dd > 30) {
            return 0;
        }
    }
    return 1;
}

//Funcion para obtener el DNI de un huesped segun su ID
int ObtenerDNI(int *id){
    FILE *huespedes = fopen("Huespedes.dat", "rb");
    THuesped huesped;
    int dni_huesped = 0;

    if (huespedes != NULL) {
        while (fread(&huesped, sizeof(THuesped), 1, huespedes)) {
            if (*id == huesped.id) {
                dni_huesped = huesped.dni;
                return dni_huesped;
            }
        }
        fclose(huespedes);
    } else {
        printf("\nNo se pudo abrir el archivo 'Huespedes', intente nuevamente\n");
    }
    return dni_huesped;
}

//Funcion para verificar que no se repitan los ID de las reservas
int VerificarIDReserva(int *id){
    FILE *reservas = fopen("Reservas.dat", "rb");
    TReserva reserva;
    int id_existe = 0;
    if (reservas != NULL) {
        while (fread(&reserva, sizeof(TReserva), 1, reservas)) {
            if (*id == reserva.id_reserva) {
                id_existe = 1;
                break;
            }
        }fclose(reservas);
    } else {
        printf("\nNo se pudo abrir el archivo 'Reservas', intente nuevamente\n");
    }
    return id_existe;
}

//Funcion para obtener el precio por noche de una habitacion
float ObtenerPrecioPorNoche(int *num_habitacion){
    FILE *habitaciones = fopen("Habitaciones.dat", "rb");
    THabitacion habitacion;
    float precio_por_noche = 0;
    if (habitaciones != NULL) {
        while (fread(&habitacion, sizeof(THabitacion), 1, habitaciones)) {
            if (*num_habitacion == habitacion.numero) {
                precio_por_noche = habitacion.precio;
                fclose(habitaciones);
                break;
            }
        }
        fclose(habitaciones);
    } else {
        printf("\nNo se pudo abrir el archivo 'Habitaciones', intente nuevamente\n");
    }
    return precio_por_noche;
}

//Proceso para cargar una reserva en el archivo "Reservas.dat"
void CargarReserva(TReserva reserva[], int *tam3){
    FILE *reservas = fopen("Reservas.dat", "ab");
    if (reservas != NULL) {
        reserva[*tam3] = NuevaReserva();
        if (reserva[*tam3].id_reserva != 0){
            fwrite(&reserva[*tam3], sizeof(TReserva), 1, reservas);
            ActualizarEstadoHabitacion(reserva[*tam3].num_habitacion, 1);
            (*tam3)++;
            printf(LGREEN"\nReserva cargada correctamente!\n"RESET);
            fclose(reservas);
        }else{
            printf("\nError al cargar la reserva. Revise los mensajes anteriores.\n");
        }
    }else{
        printf("\nError al abrir el archivo 'Reservas', intente nuevamente\n");
    }
    fclose(reservas);
}

//Proceso para listar todas las reservas
void MostrarReservas(){
    FILE *reservas = fopen("Reservas.dat","rb");
    TReserva reserva;
    if(reservas != NULL){
        printf(CYAN"\n\n---------------------------------------------------------------- Reservas ---------------------------------------------------------------------\n"RESET);
        printf("\n ID Reserva "CYAN"|"RESET" Nro. habitacion "CYAN"|"RESET" ID huesped "CYAN"|"RESET"    DNI    "CYAN"|"RESET"    Nombre Huesped    "CYAN"|"RESET"   Check In  "CYAN"|"RESET"   Check Out   "CYAN"|"RESET" Precio por noche "CYAN"|"RESET" Precio total "CYAN"|"RESET"\n");
        printf(CYAN"-----------------------------------------------------------------------------------------------------------------------------------------------"RESET);
        while(fread(&reserva,sizeof(TReserva), 1, reservas)){
            printf(" \n %-10d "CYAN"|"RESET" %-15d "CYAN"|"RESET" %-10d "CYAN"|"RESET" %-9d "CYAN"|"RESET" %-20s "CYAN"|"RESET"  %02d/%02d/%04d "CYAN"|"RESET"   %02d/%02d/%04d  "CYAN"|"RESET" %-16.2f "CYAN"|"RESET" %-12.2f "CYAN"|"RESET"",reserva.id_reserva, reserva.num_habitacion, reserva.id_cliente, reserva.dni_cliente, reserva.nombre_huesped, reserva.dd_in, reserva.mm_in, reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out, reserva.precio_por_noche, reserva.precio_total);
        }
        printf("\n");
        fclose(reservas);
    }else{
        printf("\nNo se pudo abrir el archivo, intente nuevamente\n");
    }
}

//Procedimiento para mostrar todas las reservas asociadas a una sola habitacion
void MostrarReservasPorHabitacion(int num_habitacion) {
    FILE *reservas = fopen("Reservas.dat","rb");
    TReserva reserva;

    if(reservas != NULL){
        printf(CYAN"\n\n---------------------------------------------------- Reservas para la habitacion %d ------------------------------------------------------\n"RESET, num_habitacion);
        printf("\n ID Reserva "CYAN"|"RESET" Nro. habitacion "CYAN"|"RESET" ID huesped "CYAN"|"RESET"    DNI    "CYAN"|"RESET" Nombre Huesped "CYAN"|"RESET"   Check In  "CYAN"|"RESET"   Check Out   "CYAN"|"RESET" Precio por noche "CYAN"|"RESET" Precio total "CYAN"|"RESET"\n");
        printf(CYAN"-----------------------------------------------------------------------------------------------------------------------------------------"RESET);
        while (fread(&reserva, sizeof(TReserva), 1, reservas)) {
            if (reserva.num_habitacion == num_habitacion) {
                 printf(" \n %-10d "CYAN"|"RESET" %-15d "CYAN"|"RESET" %-10d "CYAN"|"RESET" %-9d "CYAN"|"RESET" %-14s "CYAN"|"RESET"  %02d/%02d/%04d "CYAN"|"RESET"   %02d/%02d/%04d  "CYAN"|"RESET" %-16.2f "CYAN"|"RESET" %-12.2f "CYAN"|"RESET"",reserva.id_reserva, reserva.num_habitacion, reserva.id_cliente, reserva.dni_cliente, reserva.nombre_huesped, reserva.dd_in, reserva.mm_in, reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out, reserva.precio_por_noche, reserva.precio_total);
            }

        }

    }else{
        printf("\nNo se pudo abrir el archivo, intente nuevamente\n");
    }
    fclose(reservas);
}

//Proceso para mostrar todas las habitaciones disponibles
void MostrarHabitacionesDisponibles(){
    FILE *habitaciones = fopen("Habitaciones.dat","rb");
    FILE *reservas = fopen("Reservas.dat","rb");
    THabitacion habitacion;

    if(habitaciones != NULL && reservas != NULL){
        printf(CYAN"\n\n------------------- Habitaciones ---------------------\n"RESET);
        printf("\n Numero "CYAN"|"RESET" Tipo    "CYAN"|"RESET" Precio por noche "CYAN"|"RESET" Estado actual "CYAN"|"RESET"\n");
        printf(CYAN"------------------------------------------------------\n"RESET);

        time_t now;
        struct tm *fechaActual;
        now = time(NULL);
        fechaActual = localtime(&now);

        int dd_actual = fechaActual->tm_mday;
        int mm_actual = fechaActual->tm_mon + 1;
        int aa_actual = fechaActual->tm_year + 1900;

       while (fread(&habitacion, sizeof(THabitacion), 1, habitaciones)) {
            if (VerificarDisponibilidad(reservas, habitacion.numero, dd_actual, mm_actual, aa_actual, dd_actual, mm_actual, aa_actual)) {
                printf(" %-6d "CYAN"|"RESET" %-7s "CYAN"|"RESET" %-16.2f "CYAN"|"RESET" %-13s "CYAN"|"RESET"\n", habitacion.numero, habitacion.tipo, habitacion.precio, "Desocupado");
            }
        }

        fclose(habitaciones);
        fclose(reservas);
    } else {
        printf("\nNo se pudo abrir el archivo, intente nuevamente\n");
    }
}

//Proceso para mostrar todas las habitaciones ocupadas
void MostrarHabitacionesOcupadas(){
    FILE *habitaciones = fopen("Habitaciones.dat","rb");
    FILE *reservas = fopen("Reservas.dat","rb");
    THabitacion habitacion;

    if(habitaciones != NULL && reservas != NULL){
        printf(CYAN"\n\n------------------- Habitaciones ---------------------\n"RESET);
        printf("\n Numero "CYAN"|"RESET" Tipo    "CYAN"|"RESET" Precio por noche "CYAN"|"RESET" Estado actual "CYAN"|"RESET"\n");
        printf(CYAN"------------------------------------------------------\n"RESET);

        time_t now;
        struct tm *fechaActual;
        now = time(NULL);
        fechaActual = localtime(&now);

        int dd_actual = fechaActual->tm_mday;
        int mm_actual = fechaActual->tm_mon + 1;
        int aa_actual = fechaActual->tm_year + 1900;

       while (fread(&habitacion, sizeof(THabitacion), 1, habitaciones)) {
            if (!VerificarDisponibilidad(reservas, habitacion.numero, dd_actual, mm_actual, aa_actual, dd_actual, mm_actual, aa_actual)) {
                printf(" %-6d "CYAN"|"RESET" %-7s "CYAN"|"RESET" %-16.2f "CYAN"|"RESET" %-13s "CYAN"|"RESET"\n", habitacion.numero, habitacion.tipo, habitacion.precio, "Ocupado");
            }
        }

        fclose(habitaciones);
        fclose(reservas);
    } else {
        printf("\nNo se pudo abrir el archivo, intente nuevamente\n");
    }
}

//Procedimiento para mostrar habtiaciones ocupadas segun dos fechas
void MostrarHabitacionesDesocupadasSegunFechas(){
    FILE *habitaciones = fopen("Habitaciones.dat","rb");
    FILE *reservas = fopen("Reservas.dat", "rb");

    THabitacion habitacion;

    int dd_in,mm_in,aa_in;
    int dd_out,mm_out,aa_out;

    time_t now;
    struct tm *fechaActual;
    char fechaStr[11];

    // Obtener la fecha actual
    now = time(NULL);
    fechaActual = localtime(&now);

    // Guardar la fecha actual
    aa_in = fechaActual->tm_year + 1900;
    mm_in = fechaActual->tm_mon + 1;
    dd_in = fechaActual->tm_mday;
    struct tm fechaCheckIn = { 0 };

    do{
        printf("\nIngrese la fecha de Check In (dd/mm/aa): ");
        fflush(stdin); scanf(" %10[^\n]", fechaStr);  // Leer la fecha como cadena
        fflush(stdin); sscanf(fechaStr, "%d/%d/%d", &dd_in, &mm_in, &aa_in);

        if(!ValidarFecha(dd_in,mm_in,aa_in)){
            printf("\nIngrese una fecha valida\n");
        }else{

            fechaCheckIn.tm_year = aa_in - 1900;
            fechaCheckIn.tm_mon = mm_in - 1;
            fechaCheckIn.tm_mday = dd_in;
            time_t checkInTime = mktime(&fechaCheckIn);

            if (difftime(checkInTime, now) <= 0) {
                printf("\nLa fecha de Check In debe ser mayor que la fecha actual.\n");
            }
        }
    }while(!ValidarFecha(dd_in, mm_in, aa_in) || difftime(mktime(&fechaCheckIn), now) <= 0);

    do{
        printf("\nIngrese la fecha de Check Out (dd/mm/aa): ");
        fflush(stdin); scanf(" %10[^\n]", fechaStr);  // Leer la fecha como cadena
        fflush(stdin); sscanf(fechaStr, "%d/%d/%d", &dd_out, &mm_out, &aa_out);

        if(!ValidarFecha(dd_out,mm_out,aa_out)){
            system("cls");
            printf("\nIngrese una fecha valida\n");
        }else if (CompararFechas(dd_in, mm_in, aa_in, dd_out, mm_out, aa_out) >= 0){
            system("cls");
            printf("\nLa fecha de Check Out debe ser mayor que la fecha de Check In\n");
        }

    }while(!ValidarFecha(dd_out, mm_out, aa_out) || CompararFechas(dd_in, mm_in, aa_in, dd_out, mm_out, aa_out) >= 0);

    printf(CYAN"\n------ Habitaciones Dsponibles ------- \n"RESET);
    printf("\n Numero "CYAN"|"RESET" Tipo    "CYAN"|"RESET" Precio por noche "CYAN"|"RESET"\n");
    printf(CYAN"--------------------------------------\n"RESET);

    // Array para almacenar el estado de ocupaci�n de cada habitaci�n
    int *habitacionesDesocupadas;
    int numHabitaciones = 0;

    // Contar el n�mero de habitaciones
    while (fread(&habitacion, sizeof(THabitacion), 1, habitaciones)) {
        numHabitaciones++;
    }

    // Alojar memoria para el array de estado de habitaciones
    habitacionesDesocupadas = (int *)malloc(numHabitaciones * sizeof(int));

    // Inicializar el array de estado de habitaciones
    for (int i = 0; i < numHabitaciones; i++) {
        habitacionesDesocupadas[i] = 0; // 0 indica desocupado, 1 indica ocupado
    }

    // Verificar disponibilidad
    VerificarDisponibilidadPorFechas(reservas, habitacionesDesocupadas, numHabitaciones, dd_in, mm_in, aa_in, dd_out, mm_out, aa_out);

    // Verificar disponibilidad
    if (!VerificarDisponibilidadPorFechas(reservas, habitacionesDesocupadas, numHabitaciones, dd_in, mm_in, aa_in, dd_out, mm_out, aa_out)) {
        printf("\nNo hay habitaciones disponibles para el rango de fechas proporcionado.\n");
    } else {
        // Mostrar las habitaciones desocupadas seg�n el array de estado
        rewind(habitaciones); // Volver al inicio del archivo de habitaciones
        while (fread(&habitacion, sizeof(THabitacion), 1, habitaciones)) {
            if (habitacionesDesocupadas[habitacion.numero - 1] == 0) {
                printf(" %-6d "CYAN"|"RESET" %-7s "CYAN"|"RESET" %-16.2f "CYAN"|"RESET"\n", habitacion.numero, habitacion.tipo, habitacion.precio);
            }
        }
    }


    free(habitacionesDesocupadas); // Liberar memoria del array de estado

    fclose(habitaciones);
    fclose(reservas);
}

//Funcion para verificar la disponibildad de las habitacones segun dos fechas
int VerificarDisponibilidadPorFechas(FILE *reservas, int *habitacionesDesocupadas, int numHabitaciones, int dd_in, int mm_in, int aa_in, int dd_out, int mm_out, int aa_out){

    TReserva reserva;
    rewind(reservas); // Volver al inicio del archivo de reservas

    while (fread(&reserva, sizeof(TReserva), 1, reservas)) {
        // Verificar si la reserva afecta al rango de fechas solicitado
        if (!(reserva.aa_out < aa_in || reserva.aa_in > aa_out ||
              (reserva.aa_out == aa_in && reserva.mm_out < mm_in) ||
              (reserva.aa_in == aa_out && reserva.mm_in > mm_out) ||
              (reserva.aa_out == aa_in && reserva.mm_out == mm_in && reserva.dd_out < dd_in) ||
              (reserva.aa_in == aa_out && reserva.mm_in == mm_out && reserva.dd_in > dd_out))) {
            // Marcar la habitaci�n como ocupada
            habitacionesDesocupadas[reserva.num_habitacion - 1] = 1;
        }
    }
    for (int i = 0; i < numHabitaciones; i++) {
        if (habitacionesDesocupadas[i] == 0) {
            return 1; // Hay al menos una habitaci�n disponible
        }
    }
    return 0; // No hay habitaciones disponibles
}

//Verificar disponibilidad de una sola habitacion segun dos fechas
int VerificarDisponibilidad(FILE *reservas, int numHabitacion, int dd_in, int mm_in, int aa_in, int dd_out, int mm_out, int aa_out){

    TReserva reserva;
    rewind(reservas); // Volver al inicio del archivo de reservas

    while (fread(&reserva, sizeof(TReserva), 1, reservas)) {

        if (reserva.num_habitacion == numHabitacion){
        // Verificar si la reserva afecta al rango de fechas solicitado
             if (!((aa_out < reserva.aa_in) || (aa_in > reserva.aa_out) ||
                  (aa_out == reserva.aa_in && mm_out < reserva.mm_in) ||
                  (aa_in == reserva.aa_out && mm_in > reserva.mm_out) ||
                  (aa_out == reserva.aa_in && mm_out == reserva.mm_in && dd_out < reserva.dd_in) ||
                  (aa_in == reserva.aa_out && mm_in == reserva.mm_out && dd_in > reserva.dd_out))) {
                return 0; // La habitaci�n est� ocupada en las fechas solicitadas
            }
            }
        }

    return 1; // La habitaci�n est� disponible en las fechas solicitadas
}

//Verificar disponibilidad de una sola habitacion segun dos fechas para modificar una reserva
int VerificarDisponibilidadReserva(FILE *reservas, int numHabitacion, int dd_in, int mm_in, int aa_in, int dd_out, int mm_out, int aa_out, int id_reserva){
    TReserva reserva;
    rewind(reservas); // Volver al inicio del archivo de reservas

    while (fread(&reserva, sizeof(TReserva), 1, reservas)) {
        // Omitir la reserva que estamos modificando
        if (reserva.id_reserva == id_reserva) {
            continue;
        }
        if (reserva.num_habitacion == numHabitacion){
        // Verificar si la reserva afecta al rango de fechas solicitado
            if ((CompararFechas(dd_in, mm_in, aa_in, reserva.dd_in, reserva.mm_in, reserva.aa_in) >= 0 &&
                 CompararFechas(dd_in, mm_in, aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out) <= 0) ||
                (CompararFechas(dd_out, mm_out, aa_out, reserva.dd_in, reserva.mm_in, reserva.aa_in) >= 0 &&
                 CompararFechas(dd_out, mm_out, aa_out, reserva.dd_out, reserva.mm_out, reserva.aa_out) <= 0) ||
                (CompararFechas(reserva.dd_in, reserva.mm_in, reserva.aa_in, dd_in, mm_in, aa_in) >= 0 &&
                 CompararFechas(reserva.dd_out, reserva.mm_out, reserva.aa_out, dd_out, mm_out, aa_out) <= 0)) {
                return 0; // No disponible
                }
            }
        }

    return 1; // La habitaci�n est� disponible en las fechas solicitadas
}

//Proceso para cancelar una reserva segun su ID
void CancelarReserva(){
    FILE *reservas = fopen("Reservas.dat","rb");
    FILE *temp = fopen("tempReservas","wb");
    TReserva reserva;

    int num,num_encontrado = 0;
    char op;

    if(reservas != NULL){

        fseek(reservas, 0, SEEK_END); // Ir al final del archivo
        if (ftell(reservas) == 0) { // Si el archivo est� vac�o
            printf("\nNo hay reservas para cancelar.\n");//Si no hay reservas para cancelar se vuelve al menu anterior
            fclose(reservas);
            fclose(temp);
            remove("tempReservas");
            return;
        }
        fseek(reservas, 0, SEEK_SET);

        do{
            MostrarReservas();

            printf("\n\nIngrese el numero de reserva que desea cancelar: ");
            fflush(stdin); scanf("%d",&num);
            num_encontrado = VerificarIDReserva(&num);
            if (!num_encontrado){
                printf("\nNumero no encontrado, intente nuevamente\n");
            }
        }while (!num_encontrado);

        printf("\nSeguro que quiere cancelar la reserva con ID %d? 'S' para confirmar 'N' para salir\n", num);
        printf("\nIngrese una opcion: ");
        fflush(stdin); scanf(" %c", &op);
        op = toupper(op);
        if(op != 'S'){
            fclose(reservas);
            fclose(temp);
            remove("tempHabitaciones");
            return;
        }
        system("cls");
        while(fread(&reserva, sizeof(TReserva), 1, reservas)){
            if(num != reserva.id_reserva){
                fwrite(&reserva, sizeof(TReserva), 1, temp);
            }else{
                 ActualizarEstadoHabitacion(reserva.num_habitacion,0);
            }
        }
        system("cls");
        printf("\x1B[38;2;255;0;0m""\nReserva cancelada correctamente!\n"RESET);
    }else{
        printf("\nNo se pudo abrir el archivo, intente nuevamente\n");
    }
    fclose(reservas);
    fclose(temp);
    remove("Reservas.dat");
    rename("tempReservas","Reservas.dat");
}

//Funcion para obtener el nombre del huesped segun su ID
char* ObtenerNombreHuesped(int *id) {
    FILE *huespedes = fopen("Huespedes.dat", "rb");
    THuesped huesped;
    static char nombreHuesped[100] = {'\0'};

    if (huespedes != NULL) {
        while (fread(&huesped, sizeof(THuesped), 1, huespedes)) {
            if (*id == huesped.id) {
                strcpy(nombreHuesped, huesped.nombre);
                fclose(huespedes);
                return nombreHuesped;
            }
        }
        fclose(huespedes);
    } else {
        printf("\nNo se pudo abrir el archivo 'Huespedes', intente nuevamente\n");
    }
    return NULL;
}

//Proceso para modificar una reserva segun su ID
void ModificarReserva(){

    FILE *reservas = fopen("Reservas.dat","rb");
    FILE *temp = fopen("tempReservas","wb");
    TReserva reserva;

    float precio_nuevo_por_noche;
    int num_habitacion,nueva_habitacion;
    int id_existe,id;
    int op;
    char op2;

    time_t now;
    struct tm *fechaActual;
    char fechaStr[11];
    time_t checkInTime;

    // Obtener la fecha actual
    now = time(NULL);
    fechaActual = localtime(&now);

    // Guardar la fecha actual en la estructura reserva
    reserva.aa_in = fechaActual->tm_year + 1900;
    reserva.mm_in = fechaActual->tm_mon + 1;
    reserva.dd_in = fechaActual->tm_mday;
    struct tm fechaCheckIn = { 0 };

    if(reservas != NULL){

        fseek(reservas, 0, SEEK_END); // Ir al final del archivo
        if (ftell(reservas) == 0) { // Si el archivo est� vac�o
            printf("\nNo hay reservas para modificar.\n");//Si no hay reservas para modificar se vuelve al menu anterior
            fclose(reservas);
            fclose(temp);
            remove("tempReservas");
            return;
        }
        fseek(reservas, 0, SEEK_SET);

        do{
            MostrarReservas();
            printf("\n\nIngrese el numero de reserva que quiera modificar: ");
            fflush(stdin); scanf("%d",&id);
            id_existe = VerificarIDReserva(&id);
            system("cls");
            if (!id_existe){
                printf("\nNumero de reserva no encontrado, intente nuevamente\n");
                fclose(reservas);
                fclose(temp);
                remove("tempReservas");
                return;
                }
        }while(!id_existe);

         while (fread(&reserva, sizeof(TReserva), 1, reservas)) {
            if (id == reserva.id_reserva) {
                printf("\nReserva encontrada:");
                printf("\n\nID Reserva: %d\nNumero de habitacion: %d\nID de huesped: %d\nCheck In: %02d/%02d/%04d\nCheck Out: %02d/%02d/%04d\nPrecio por noche: %.2f\nPrecio total: %.2f\n",reserva.id_reserva, reserva.num_habitacion, reserva.id_cliente, reserva.dd_in, reserva.mm_in, reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out ,reserva.precio_por_noche, reserva.precio_total);

                printf("\nSeleccione que desea modificar:");
                printf("\n\n1) Numero de habitacion");
                printf("\n\n2) Fecha de Check In");
                printf("\n\n3) Fecha de Check Out");
                printf("\n\n0) Volver");
                printf("\n\nIngrese una opcion: ");
                fflush(stdin);
                scanf("%d", &op);
                switch(op){
                    case 1:{
                            printf("\nEsta seguro que desea modificar el numero de habitacion? Ingrese 'S' para seguir o 'N' para salir: ");
                            fflush(stdin); scanf("%c",&op2);
                            op2 = toupper(op2);
                            if(op2 == 'S'){

                                FILE *habitaciones = fopen("Habitaciones.dat","rb");
                                THabitacion habitacion;

                                printf(CYAN"\n------ Habitaciones Dsponibles entre el %02d/%02d/%04d y %02d/%02d/%04d ------- \n"RESET,reserva.dd_in, reserva.mm_in, reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out);
                                printf("\n Numero "CYAN"|"RESET" Tipo    "CYAN"|"RESET" Precio por noche "CYAN"|"RESET"\n");
                                printf(CYAN"--------------------------------------\n"RESET);

                                // Array para almacenar el estado de ocupaci�n de cada habitaci�n
                                int *habitacionesDesocupadas;
                                int numHabitaciones = 0;

                                // Contar el n�mero de habitaciones
                                while (fread(&habitacion, sizeof(THabitacion), 1, habitaciones)) {
                                    numHabitaciones++;
                                }

                                // Alojar memoria para el array de estado de habitaciones
                                habitacionesDesocupadas = (int *)malloc(numHabitaciones * sizeof(int));

                                // Inicializar el array de estado de habitaciones
                                for (int i = 0; i < numHabitaciones; i++) {
                                    habitacionesDesocupadas[i] = 0; // 0 indica desocupado, 1 indica ocupado
                                }

                                // Verificar disponibilidad
                                VerificarDisponibilidadPorFechas(reservas, habitacionesDesocupadas, numHabitaciones, reserva.dd_in, reserva.mm_in, reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out);

                                // Verificar disponibilidad
                                if (!VerificarDisponibilidadPorFechas(reservas, habitacionesDesocupadas, numHabitaciones, reserva.dd_in, reserva.mm_in, reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out)) {
                                    printf("\nNo hay habitaciones disponibles para el rango de fechas proporcionado.\n");

                                } else {
                                    // Mostrar las habitaciones desocupadas seg�n el array de estado
                                    rewind(habitaciones); // Volver al inicio del archivo de habitaciones
                                    while (fread(&habitacion, sizeof(THabitacion), 1, habitaciones)) {
                                        if (habitacionesDesocupadas[habitacion.numero - 1] == 0) {
                                            printf(" %-6d "CYAN"|"RESET" %-7s "CYAN"|"RESET" %-16.2f "CYAN"|"RESET"\n", habitacion.numero, habitacion.tipo, habitacion.precio);
                                        }
                                    }
                                }

                                free(habitacionesDesocupadas);
                                fclose(habitaciones);

                                printf("\n\nIngrese el nuevo numero de habitacion: ");
                                num_habitacion = reserva.num_habitacion; // Se guarda el numero de habitacion antes de modificarlo para luego actualizar su estado
                                fflush(stdin); scanf("%d", &nueva_habitacion);


                                if (VerificarNumHabitacion(&nueva_habitacion)) {
                                    // Verificar si la nueva habitaci�n est� disponible para las fechas de la reserva
                                    if (VerificarDisponibilidadReserva(reservas, nueva_habitacion, reserva.dd_in, reserva.mm_in, reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out, reserva.id_reserva)) {
                                        // Guardar el precio por noche de la nueva habitaci�n
                                        precio_nuevo_por_noche = ObtenerPrecioPorNoche(&nueva_habitacion);

                                        // Actualizar la reserva con la nueva habitaci�n y el nuevo precio por noche
                                        num_habitacion = reserva.num_habitacion;
                                        reserva.num_habitacion = nueva_habitacion;
                                        reserva.precio_por_noche = precio_nuevo_por_noche;
                                        reserva.precio_total = reserva.precio_por_noche * reserva.cantidad_noches;

                                        // Actualizar el estado de las habitaciones
                                        ActualizarEstadoHabitacion(reserva.num_habitacion, 1); // Ocupar la nueva habitaci�n
                                        if(VerificarDisponibilidad(reservas, num_habitacion, reserva.dd_in, reserva.mm_in, reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out)){
                                            ActualizarEstadoHabitacion(num_habitacion, 0); // Liberar la habitaci�n anterior
                                        }

                                        printf(LGREEN"\nReserva modificada correctamente!\n"RESET);
                                    } else {
                                        printf("\nNumero de habitacion no disponible para las fechas seleccionadas.\n");
                                        return;
                                    }
                                } else {
                                    printf("\nNumero de habitacion no valido o no existe.\n");
                                }
                            } else {
                                fclose(reservas);
                                fclose(temp);
                                remove("tempReservas");
                                return;
                            }
                        }break;
                    case 2:{
                            printf("\nEsta seguro que desea modificar la fecha de Check In de la reserva? Ingrese 'S' para seguir o 'N' para salir: ");
                            fflush(stdin); scanf("%c",&op2);
                            op2 = toupper(op2);
                            if(op2 == 'S'){
                                do{
                                    MostrarReservasPorHabitacion(reserva.num_habitacion);
                                    printf("\n\nIngrese la fecha de Check In (dd/mm/aa): ");
                                    fflush(stdin); scanf(" %10[^\n]", fechaStr);  // Leer la fecha como cadena
                                    fflush(stdin); sscanf(fechaStr, "%d/%d/%d", &reserva.dd_in, &reserva.mm_in, &reserva.aa_in);

                                    if(!ValidarFecha(reserva.dd_in,reserva.mm_in,reserva.aa_in)){
                                        printf("\nIngrese una fecha valida\n");
                                    }else{

                                        fechaCheckIn.tm_year = reserva.aa_in - 1900;
                                        fechaCheckIn.tm_mon = reserva.mm_in - 1;
                                        fechaCheckIn.tm_mday = reserva.dd_in;

                                        checkInTime = mktime(&fechaCheckIn);

                                       if (difftime (checkInTime, now) <= 0) {
                                                printf("\nLa fecha de Check In debe ser mayor que la fecha actual.\n");

                                        }else if (CompararFechas(reserva.dd_in, reserva.mm_in, reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out) >= 0){
                                            system("cls");
                                            printf("\nLa fecha de Check In no puede ser mayor que la fecha de Check Out\n");
                                        }
                                    }
                                }while(!ValidarFecha(reserva.dd_in, reserva.mm_in, reserva.aa_in) || difftime (checkInTime, now) <= 0 || CompararFechas(reserva.dd_in, reserva.mm_in, reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out) >= 0);

                                if (!VerificarDisponibilidadReserva(reservas, reserva.num_habitacion, reserva.dd_in, reserva.mm_in, reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out, reserva.id_reserva)){
                                        system("cls");
                                        printf("\nla habitacion no esta disponible para el rango de fechas proporcionado.\n");
                                        return ;
                                }

                                reserva.cantidad_noches = CalcularDiferenciaDias(reserva.dd_in, reserva.mm_in, reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out);
                                reserva.precio_total = reserva.precio_por_noche * reserva.cantidad_noches; //Se actualiza  el precio total de la reserva

                                printf(LGREEN"\nReserva modificada correctamente!\n"RESET);

                            }else{
                                fclose(reservas);
                                fclose(temp);
                                remove("tempReservas");
                                return;
                            }
                        }break;
                    case 3:{
                            printf("\nEsta seguro que desea modificar la fecha Check Out de la reserva? Ingrese 'S' para seguir o 'N' para salir: ");
                            fflush(stdin); scanf("%c",&op2);
                            op2 = toupper(op2);
                            if(op2 == 'S'){
                                do{
                                    printf("\n\nIngrese la fecha de Check Out (dd/mm/aa): ");
                                    fflush(stdin); scanf(" %10[^\n]", fechaStr);  // Leer la fecha como cadena
                                    fflush(stdin); sscanf(fechaStr, "%d/%d/%d", &reserva.dd_out, &reserva.mm_out, &reserva.aa_out);

                                    if(!ValidarFecha(reserva.dd_out, reserva.mm_out, reserva.aa_out)){
                                        system("cls");
                                        printf("\nIngrese una fecha valida\n");
                                    }else if (CompararFechas(reserva.dd_in,reserva.mm_in,reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out) >= 0){
                                        system("cls");
                                        printf("\nLa fecha de Check Out debe ser mayor que la fecha de Check In\n");
                                    }

                                }while(!ValidarFecha(reserva.dd_out, reserva.mm_out, reserva.aa_out) || CompararFechas(reserva.dd_in,reserva.mm_in,reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out) >= 0);

                                reserva.cantidad_noches = CalcularDiferenciaDias(reserva.dd_in,reserva.mm_in,reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out);
                                reserva.precio_total = reserva.precio_por_noche * reserva.cantidad_noches; //Se actualiza  el precio total de la reserva

                                printf(LGREEN"\nReserva modificada correctamente!\n"RESET);

                            }else{
                                fclose(reservas);
                                fclose(temp);
                                remove("tempReservas");
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
            fwrite(&reserva, sizeof(TReserva), 1, temp);
        }
    }else{
        printf("\nNo se pudo abrir el archivo, intente nuevamente\n");
    }
    fclose(reservas);
    fclose(temp);
    remove("Reservas.dat");
    rename("tempReservas","Reservas.dat");
}

//Procedimiento para buscar reserva segun el DNI de un huesped
void BuscarReservaPorDNI(){
    FILE *reservas = fopen("Reservas.dat","rb");
    TReserva reserva;
    int dni;

    if(reservas != NULL){
         do{
             printf("\nIngrese el DNI para buscar su reserva: ");
             fflush(stdin); scanf("%d",&dni);
             if(!VerificarDNI(&dni)){
                 printf("\nEl DNI ingresado no esta asociado a ninguna reserva. Intente nuevamente\n");
                 fclose(reservas);
                 return;
            }else{
                system("cls");
                printf("\nReserva encontrada\n");
                printf(CYAN"\n\n--------------------------------------------------------------- Reservas ----------------------------------------------------------------\n"RESET);
                printf("\n ID Reserva "CYAN"|"RESET" Nro. habitacion "CYAN"|"RESET" ID huesped "CYAN"|"RESET"    DNI    "CYAN"|"RESET" Nombre Huesped "CYAN"|"RESET"   Check In  "CYAN"|"RESET"   Check Out   "CYAN"|"RESET" Precio por noche "CYAN"|"RESET" Precio total "CYAN"|"RESET"\n");
                printf(CYAN"-----------------------------------------------------------------------------------------------------------------------------------------"RESET);
                while(fread(&reserva,sizeof(TReserva), 1, reservas)){
                    if(reserva.dni_cliente == dni){
                        printf(" \n %-10d "CYAN"|"RESET" %-15d "CYAN"|"RESET" %-10d "CYAN"|"RESET" %-9d "CYAN"|"RESET" %-14s "CYAN"|"RESET"  %02d/%02d/%04d "CYAN"|"RESET"  %02d/%02d/%04d   "CYAN"|"RESET" %-16.2f "CYAN"|"RESET" %-12.2f "CYAN"|"RESET"",reserva.id_reserva, reserva.num_habitacion, reserva.id_cliente, reserva.dni_cliente, reserva.nombre_huesped, reserva.dd_in, reserva.mm_in, reserva.aa_in, reserva.dd_out, reserva.mm_out, reserva.aa_out, reserva.precio_por_noche, reserva.precio_total);
                    }
                }
                fclose(reservas);
            }
        }while(!VerificarDNI(&dni));
    }else{
        printf("\nNo se pudo abrir el archivo, intente nuevamente\n");
    }

}

//Menu para gestionar las reservas
void GestionarReservas(){
    TReserva reserva[MAX_REGISTROS];
    int tam3 = 0;
    int op;
    int num_buscar;
    char op2;
    do{
        printf(CYAN"\n--------------------------------------\n"RESET);
        printf(CYAN"|         Gestion de Reservas        |"RESET);
        printf(CYAN"\n--------------------------------------\n"RESET);
        printf("\n 1) Cargar nueva reserva\n");
        printf("\x1B[38;2;255;0;0m""\n 2) Cancelar reserva segun su ID\n"RESET);
        printf("\n 3) Mostrar reservas\n");
        printf("\x1B[38;2;255;255;0m""\n 4) Modificar reservas\n"RESET);
        printf("\n 5) Mostrar habitaciones disponibles\n");
        printf("\n 6) Mostrar habitaciones ocupadas\n");
        printf("\n 7) Buscar reserva por DNI\n");
        printf("\n 8) Buscar disponibilidad segun dos fechas\n");
        printf("\n 9) Ver todas las reservas de una habitacion \n");
        printf("\n 0) Volver al menu principal\n");
        printf("\n Ingrese una opcion: ");
        fflush(stdin); scanf("%d",&op);
        system("cls");
        switch(op){
            case 1:
                printf("\nEsta seguro que desea cargar una reserva? Ingrese 'S' para seguir o 'N' para salir: ");
                fflush(stdin); scanf("%c",&op2);
                op2 = toupper(op2);
                if(op2 == 'S'){
                    do{
                        CargarReserva(reserva,&tam3);
                        if (reserva[tam3 - 1].id_reserva != 0) { // Verificar si la reserva se carg� correctamente
                            printf("\nDesea cargar otra reserva? 'N' para salir, 'S' para seguir: ");
                            fflush(stdin); scanf(" %c", &op2);
                            op2 = toupper(op2);
                            system("cls");
                        } else {
                            op2 = 'N'; // Salir del ciclo si hubo un error en la reserva
                        }
                    }while(op2 != 'N' && tam3 <= MAX_REGISTROS);
                }
                break;
            case 2: CancelarReserva();
                break;
            case 3: MostrarReservas();
                break;
            case 4: ModificarReserva();
                break;
            case 5: MostrarHabitacionesDisponibles();
                break;
            case 6: MostrarHabitacionesOcupadas();
                break;
            case 7: BuscarReservaPorDNI();
                break;
            case 8: MostrarHabitacionesDesocupadasSegunFechas();
                break;
            case 9:{
                    printf("\nIngrese el numero de la habitacion para ver sus reservas: ");
                    fflush(stdin); scanf("%d",&num_buscar);
                    if(VerificarNumHabitacion(&num_buscar)){
                        MostrarReservasPorHabitacion(num_buscar);
                    }else{
                        printf("\nEl numero de habitacion ingresado no existe. Intente nuevamente\n");
                    }
                }break;
            case 0:
                break;
            default: printf("\nIngrese una opcion valida\n");
                break;
        }
    }while(op != 0);
}

