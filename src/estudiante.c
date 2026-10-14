/*
 * estudiante.c — Formulario de datos del estudiante (version terminal).
 *
 * Pantalla que pide los datos personales: codigo, nombre, edad,
 * area de conocimiento, aula y grupo. En el proyecto del curso los
 * formularios viven en su propio modulo (estudiante.c / boletin.c),
 * separados de la pantalla (terminal.c) y del reporte (reporte.c).
 *
 * NO entra al WebAssembly.
 */

#include <stdio.h>

#include "../include/estudiante.h"
#include "../include/terminal.h"
#include "../include/constantes.h"

/* ================================================================== */
/*  FORMULARIO DE DATOS PERSONALES                                    */
/* ================================================================== */

/* Pide codigo, nombre, edad, area, aula y grupo */
void llenarInformacionAlumno(Estudiante *alumno, int numero) {
    gotoxy(5, 4);
    backgroundColor(AMARILLO, NEGRO);
    printf("INFORMACION DEL ESTUDIANTE #%d", numero);
    printf("\033[0m");

    alumno->codigo = leerEntero(5, 6, "Codigo: ", CODIGO_MINIMO, CODIGO_MAXIMO);
    leerTexto(5, 8, "Nombre: ", alumno->nombre, 50);
    alumno->edad = leerEntero(5, 10, "Edad: ", EDAD_MINIMA, EDAD_MAXIMA);

    leerTexto(5, 12, "Area de Conocimiento: ", alumno->areaConocimiento, 50);
    leerTexto(5, 14, "Aula Asignada: ", alumno->aula, 20);
    leerTexto(5, 16, "Grupo: ", alumno->grupo, 20);
}
