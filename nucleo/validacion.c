/*
 * validacion.c — Reglas de negocio sobre los datos del estudiante.
 *
 * Es la ÚNICA definición de los rangos válidos: tanto la terminal como
 * la versión WebAssembly validan con estas funciones (no cada una por
 * su lado).
 *
 * Sin terminal, sin archivos y sin memoria dinámica.
 */

#include "estudiantes.h"

/* ================================================================== */
/*  NOTAS                                                              */
/* ================================================================== */

/* 1 = la nota está dentro del rango permitido */
int validarNota(float nota) {
    return (nota >= NOTA_MINIMA && nota <= NOTA_MAXIMA);
}

/* ================================================================== */
/*  ESTUDIANTE COMPLETO                                                */
/* ================================================================== */

/* 1 = todos los campos del registro son válidos */
int validarEstudiante(const Estudiante *alumno) {
    if (alumno == NULL) return 0;

    if (alumno->codigo < CODIGO_MINIMO || alumno->codigo > CODIGO_MAXIMO) return 0;
    if (alumno->nombre[0] == '\0') return 0;
    if (alumno->edad < EDAD_MINIMA || alumno->edad > EDAD_MAXIMA) return 0;

    if (alumno->areaConocimiento[0] == '\0') return 0;
    if (alumno->aula[0] == '\0') return 0;
    if (alumno->grupo[0] == '\0') return 0;

    if (!validarNota(alumno->boletin.espanol)) return 0;
    if (!validarNota(alumno->boletin.matematica)) return 0;
    if (!validarNota(alumno->boletin.cienciasNaturales)) return 0;
    if (!validarNota(alumno->boletin.formacionCivica)) return 0;

    return 1;
}
