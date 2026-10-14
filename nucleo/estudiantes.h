#ifndef ESTUDIANTES_H
#define ESTUDIANTES_H

/*
 * estudiantes.h — Sistema de Registro de Estudiantes
 *
 * Cabecera COMPARTIDA entre la terminal y el WebAssembly.
 * Contiene unicamente las estructuras, las constantes del negocio
 * y la logica pura del sistema.
 *
 * OJO: nada de terminal aqui (sin gotoxy, getch, scanf ni colores ANSI).
 * Eso vive en terminal.h, que NO entra al wasm.
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE  /* Necesario en Linux para que strcasestr funcione sin advertencias */
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

/* Nombre del archivo binario donde se guardan los datos */
#define NOMBRE_ARCHIVO "estudiantes.dat"

/* ------------------------------------------------------------------ */
/*  Estructuras                                                        */
/* ------------------------------------------------------------------ */

typedef struct {
    float espanol;
    float matematica;
    float cienciasNaturales;
    float formacionCivica;
    float promedio;
} Boletin;

typedef struct {
    int   codigo;
    char  nombre[50];
    int   edad;
    char  areaConocimiento[50];
    char  aula[20];
    char  grupo[20];
    Boletin boletin;
} Estudiante;

/* ------------------------------------------------------------------ */
/*  Constantes del negocio — DEFINIDAS UNA SOLA VEZ                    */
/*  (antes estaban escritas a mano en terminal.c y en api_web.c)       */
/* ------------------------------------------------------------------ */

/* Notas */
#define NOTA_MINIMA      0.0f
#define NOTA_MAXIMA    100.0f
#define NOTA_APROBACION  70.0f

/* Código del estudiante */
#define CODIGO_MINIMO    1
#define CODIGO_MAXIMO    999999

/* Edad */
#define EDAD_MINIMA      3
#define EDAD_MAXIMA      100

/* ------------------------------------------------------------------ */
/*  Prototipos — logica pura (funciones.c) — ESTA SÍ entra al wasm     */
/* ------------------------------------------------------------------ */

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
int agregarEstudiante(Estudiante **alumnos, int *cantidad, Estudiante *nuevo);

/* Busqueda por nombre: 1 si hay coincidencia parcial
 * (no distingue mayusculas/minusculas) */
int coincideNombre(const Estudiante *alumno, const char *texto);

/* ------------------------------------------------------------------ */
/*  Prototipos — validacion (validacion.c) — entra al wasm             */
/* ------------------------------------------------------------------ */

int validarNota(float nota);
int validarEstudiante(const Estudiante *alumno);

/* ------------------------------------------------------------------ */
/*  Prototipos — memoria (memoria.c) — entra al wasm                   */
/* ------------------------------------------------------------------ */

/* Devuelve el arreglo con espacio para (cantidad + 1) estudiantes,
 * o NULL si no hay memoria. */
Estudiante *ampliarEstudiantes(Estudiante *alumnos, int cantidad);

/* Libera el arreglo y deja el puntero en NULL. */
void liberarEstudiantes(Estudiante **alumnos);

#endif /* ESTUDIANTES_H */
