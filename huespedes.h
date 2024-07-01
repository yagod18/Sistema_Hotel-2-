#ifndef HUESPEDES_H_INCLUDED
#define HUESPEDES_H_INCLUDED
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "colors.h"
#define MAX_REGISTROS 100 //Cantidad maxima de registros

typedef struct Huesped{
    int id; //ID para identificar al huesped
    int dni; //DNI del huesped
    char nombre[100]; //Nombre y apellido del huesped
    char direccion[100]; //Direccion del huesped
    char telefono[20]; //Numero de telefono del huesped
}THuesped;

THuesped NuevoHuesped();

int VerificarID(int *id);
int VerificarDNI(int *dni);
int ObtenerDNI(int *id);
int VerificarHuespedReserva(int *id);
int VerificarTelefono(char *telefono);

void ActualizarNombreReserva(int id, char *nombre);
void ActualizarDNIReserva(int id, int dni);

void GestionarHuesped();
void CargarHuesped(THuesped huesped[],int *tam);
void EliminarHuesped();
void MostrarHuesped();
void ModificarHuesped();

#endif // HUESPEDES_H_INCLUDED