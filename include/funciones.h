#ifndef FUNCIONES_H
#define FUNCIONES_H


#include "estructuras.h"


/*---------------------------------------------------------
    PROTOTIPOS
---------------------------------------------------------*/

/* Ordenamiento con qsort() */
int  compararEstudiantes(const void *a, const void *b);

void ordenarEstudiantes(Estudiante *alumnos, int cantidad);

/* Archivo binario .dat */
void guardarEstudiantes(const Estudiante *alumnos, int cantidad);

int  cargarEstudiantes(Estudiante **alumnos);

/* Logica de datos */
void calcularPromedio(Boletin *boletin);

/* Alta completa de un estudiante:
 * valida los datos, amplia la memoria, calcula el promedio,
 * ordena la lista y guarda en disco.
 * Devuelve: 1 = agregado, 0 = datos invalidos, -1 = sin memoria. */
int  agregarEstudiante(Estudiante **alumnos, int *cantidad, Estudiante *nuevo);

/* Busqueda por nombre: 1 si hay coincidencia parcial
 * (no distingue mayusculas/minusculas) */
int  coincideNombre(const Estudiante *alumno, const char *texto);


#endif
