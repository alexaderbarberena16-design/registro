#ifndef MEMORIA_H
#define MEMORIA_H


#include "estructuras.h"


/*---------------------------------------------------------
    PROTOTIPOS
---------------------------------------------------------*/

/* Devuelve el arreglo con espacio para (cantidad + 1) estudiantes,
 * o NULL si no hay memoria. */
Estudiante *ampliarEstudiantes(Estudiante *alumnos, int cantidad);

/* Libera el arreglo y deja el puntero en NULL. */
void        liberarEstudiantes(Estudiante **alumnos);


#endif
