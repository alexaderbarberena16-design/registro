#ifndef REPORTE_H
#define REPORTE_H


#include "estructuras.h"


/*---------------------------------------------------------
    PROTOTIPOS
---------------------------------------------------------*/

/* Listado general de todos los estudiantes registrados */
void imprimirInformacion(const Estudiante *alumnos, int cantidad);

/* Busqueda parcial por nombre (no distingue mayusculas/minusculas) */
void buscarEstudiante(const Estudiante *alumnos, int cantidad);


#endif
