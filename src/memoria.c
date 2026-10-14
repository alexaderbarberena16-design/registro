/*
 * memoria.c — Reserva y liberacion de memoria dinamica del arreglo de
 * estudiantes.
 *
 * Aqui esta TODO el malloc/realloc/free del proyecto: antes estaba
 * repartido entre main.c, funciones.c y api_web.c.
 *
 * Sin terminal y sin logica de negocio.
 */

#include <stdlib.h>

#include "../include/memoria.h"

/* ================================================================== */
/*  AMPLIAR EL ARREGLO                                                 */
/* ================================================================== */

/* Devuelve el arreglo con espacio para (cantidad + 1) estudiantes.
 * Devuelve NULL si no hay memoria o si la cantidad es invalida.
 * El puntero original sigue siendo valido si esta funcion falla. */
Estudiante *ampliarEstudiantes(Estudiante *alumnos, int cantidad) {
    if (cantidad < 0) return NULL;

    return realloc(alumnos, (cantidad + 1) * sizeof(Estudiante));
}

/* ================================================================== */
/*  LIBERAR                                                            */
/* ================================================================== */

/* Libera el arreglo y deja el puntero en NULL (no apunta a memoria
 * liberada). Acepta puntero doble para poder modificarlo. */
void liberarEstudiantes(Estudiante **alumnos) {
    if (alumnos == NULL) return;

    free(*alumnos);
    *alumnos = NULL;
}
