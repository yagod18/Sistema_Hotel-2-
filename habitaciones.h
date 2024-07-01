#ifndef HABITACIONES_H_INCLUDED
#define HABITACIONES_H_INCLUDED
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "colors.h"
#define MAX_REGISTROS 100 //Cantidad maxima de registros

typedef struct Habitacion{
    int numero; //Numero de la habitacion
    char tipo[50]; //Tipo de habitacion (Simple, Doble, Suite)
    float precio; //Precio por noche
    int estado; //0 si esta desocupado, 1 si esta ocupado
}THabitacion;

THabitacion NuevaHabitacion();
int VerificarNumHabitacion(int *num);
int VerificarEstado(int *num);

void GestionarHabitaciones();
void CargarHabitacion(THabitacion habitacion[],int *tam2);
void EliminarHabitacion();
void MostrarHabitacion();
void ModificarHabitacion();
void ActualizarEstadoHabitacion(int num, int estado);
void ActualizarPrecioReservas(int *num_habitacion, float *nuevo_precio);

#endif // HABITACIONES_H_INCLUDED
