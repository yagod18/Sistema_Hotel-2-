#ifndef REGISTROS_H_INCLUDED
#define REGISTROS_H_INCLUDED
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "colors.h"
#define MAX_REGISTROS 100 //Cantidad maxima de registros

typedef struct Reserva{
    int id_reserva; //ID para identificar la reserva
    int id_cliente; //ID cliente para relacionar la reserva con un huesped
    int dni_cliente; //DNI cliente para guardar el dato en la reserva
    int num_habitacion; //Numero de habitacion para relacionar la reserva con una habitacio
    char nombre_huesped[100]; //Nombre del huesped
    int dd_in,mm_in,aa_in; //Fecha de check in
    int dd_out,mm_out,aa_out; //Fecha de check out
    int cantidad_noches; //Cantidad de noches segun las fechas
    float precio_por_noche; //Precio por noche de la habitacion
    float precio_total; //Precio total de la estadia
    int estado; // 0 si esta desocupado, 1 si esta ocupado
}TReserva;


TReserva NuevaReserva();
int VerificarIDReserva(int *id);

float ObtenerPrecioPorNoche(int *num_habitacion);
char* ObtenerNombreHuesped(int *id);

int ValidarFecha(int dd, int mm, int aa);
int CompararFechas(int dd1, int mm1, int aa1, int dd2, int mm2, int aa2);
int EsBisiesto(int aa);
int CalcularDiferenciaDias(int dd1, int mm1, int aa1, int dd2, int mm2, int aa2);

int VerificarDisponibilidadPorFechas(FILE *reservas, int *habitacionesDesocupadas, int numHabitaciones, int dd_in, int mm_in, int aa_in, int dd_out, int mm_out, int aa_out);
int VerificarDisponibilidad(FILE *reservas, int numHabitacion, int dd_in, int mm_in, int aa_in, int dd_out, int mm_out, int aa_out);
int VerificarDisponibilidadReserva(FILE *reservas, int numHabitacion, int dd_in, int mm_in, int aa_in, int dd_out, int mm_out, int aa_out, int id_reserva);

void GestionarReservas();
void CargarReserva(TReserva reserva[], int *tam3);
void CancelarReserva();
void MostrarReservas();
void MostrarHabitacionesDisponibles();
void MostrarHabitacionesOcupadas();
void ModificarReserva();
void BuscarReservaPorDNI();
void MostrarHabitacionesDesocupadasSegunFechas();
void MostrarReservasPorHabitacion(int num_habitacion);


#endif // REGISTROS_H_INCLUDED
