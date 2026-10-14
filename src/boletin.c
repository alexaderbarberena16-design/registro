/*
 * boletin.c — Formulario del boletin de notas (version terminal).
 *
 * Pantalla que pide las notas: espanol, matematica, ciencias
 * naturales y formacion civica. El calculo del promedio NO vive
 * aqui: eso es logica pura y esta en funciones.c (calcularPromedio),
 * que es el que tambien compila la version WebAssembly.
 *
 * NO entra al WebAssembly.
 */

#include <stdio.h>

#include "../include/boletin.h"
#include "../include/terminal.h"
#include "../include/constantes.h"

/* ================================================================== */
/*  FORMULARIO DEL BOLETIN                                            */
/* ================================================================== */

/* Pide las cuatro notas del boletin */
void llenarInformacionBoletin(Estudiante *alumno) {
    gotoxy(55, 4);
    backgroundColor(CELESTE, NEGRO);
    printf("BOLETIN DE NOTAS");
    printf("\033[0m");

    gotoxy(55, 6);
    printf("Estudiante: %s", alumno->nombre);

    alumno->boletin.espanol = leerFlotante(55, 8, "Espanol (0-100): ", NOTA_MINIMA, NOTA_MAXIMA);
    alumno->boletin.matematica = leerFlotante(55, 10, "Matematica (0-100): ", NOTA_MINIMA, NOTA_MAXIMA);
    alumno->boletin.cienciasNaturales = leerFlotante(55, 12, "Ciencias Naturales (0-100): ", NOTA_MINIMA, NOTA_MAXIMA);
    alumno->boletin.formacionCivica = leerFlotante(55, 14, "Formacion Civica (0-100): ", NOTA_MINIMA, NOTA_MAXIMA);
}
