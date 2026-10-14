/*
 * memoria.c — Reserva y liberación de memoria dinámica del arreglo de
 * estudiantes.
 *
 * Aquí está TODO el malloc/realloc/free del proyecto: antes estaba
 * repartido entre main.c, funciones.c y api_web.c.
 *
 * Sin terminal y sin lógica de negocio.
 */

#include "estudiantes.h"

/* ================================================================== */
/*  AMPLIAR EL ARREGLO                                                 */
/* ================================================================== */

/* Devuelve el arreglo con espacio para (cantidad + 1) estudiantes.
 * Devuelve NULL si no hay memoria o si la cantidad es inválida.
 * El puntero original sigue siendo válido si esta función falla. */
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
