#ifndef VALIDACION_H
#define VALIDACION_H


#include "estructuras.h"


/*---------------------------------------------------------
    PROTOTIPOS
---------------------------------------------------------*/

/* 1 = la nota esta dentro del rango permitido */
int  validarNota(float nota);

/* 1 = todos los campos del registro son validos */
int  validarEstudiante(const Estudiante *alumno);


#endif
