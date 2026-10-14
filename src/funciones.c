/*
 * funciones.c — Logica pura del Sistema de Registro de Estudiantes.
 *
 * Este archivo es uno de los .c de logica que entran al WebAssembly
 * (ver ../registro-estudiantes-web/herramientas/build_wasm.sh:
 *  api_web/api_web.c + src/funciones.c).
 *
 * NO contiene nada de terminal: sin getch, gotoxy, scanf ni colores
 * ANSI. La interfaz de terminal vive en terminal.c y la del navegador
 * en web/index.html.
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE  /* Necesario en Linux para que strcasestr funcione sin advertencias */
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "../include/funciones.h"
#include "../include/validacion.h"
#include "../include/memoria.h"
#include "../include/constantes.h"

/* ================================================================== */
/*  ORDENAMIENTO CON qsort()                                          */
/* ================================================================== */

/* Ordena por: area de conocimiento, aula, grupo y finalmente nombre */
int compararEstudiantes(const void *a, const void *b) {
    const Estudiante *e1 = (const Estudiante *)a;
    const Estudiante *e2 = (const Estudiante *)b;

    int resArea = strcasecmp(e1->areaConocimiento, e2->areaConocimiento);
    if (resArea != 0) return resArea;

    int resAula = strcasecmp(e1->aula, e2->aula);
    if (resAula != 0) return resAula;

    int resGrupo = strcasecmp(e1->grupo, e2->grupo);
    if (resGrupo != 0) return resGrupo;

    return strcasecmp(e1->nombre, e2->nombre);
}

void ordenarEstudiantes(Estudiante *alumnos, int cantidad) {
    if (alumnos != NULL && cantidad > 1) {
        qsort(alumnos, cantidad, sizeof(Estudiante), compararEstudiantes);
    }
}

/* ================================================================== */
/*  ARCHIVO BINARIO (.dat)                                            */
/* ================================================================== */

/* Guarda la cantidad y luego el arreglo completo en estudiantes.dat */
void guardarEstudiantes(const Estudiante *alumnos, int cantidad) {
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "wb");
    if (archivo == NULL) return;

    fwrite(&cantidad, sizeof(int), 1, archivo);
    if (cantidad > 0) {
        fwrite(alumnos, sizeof(Estudiante), cantidad, archivo);
    }
    fclose(archivo);
}

/* Carga estudiantes.dat en un arreglo dinamico (malloc).
 * Devuelve la cantidad de registros leidos (0 si el archivo no existe). */
int cargarEstudiantes(Estudiante **alumnos) {
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "rb");
    if (archivo == NULL) {
        *alumnos = NULL;
        return 0;
    }

    int cantidad = 0;
    if (fread(&cantidad, sizeof(int), 1, archivo) != 1 || cantidad <= 0) {
        fclose(archivo);
        *alumnos = NULL;
        return 0;
    }

    *alumnos = (Estudiante *) malloc(cantidad * sizeof(Estudiante));
    if (*alumnos == NULL) {
        fclose(archivo);
        return 0;
    }

    fread(*alumnos, sizeof(Estudiante), cantidad, archivo);
    fclose(archivo);

    ordenarEstudiantes(*alumnos, cantidad);
    return cantidad;
}

/* ================================================================== */
/*  LOGICA DE DATOS                                                   */
/* ================================================================== */

/* Promedio general del boletin */
void calcularPromedio(Boletin *boletin) {
    float suma = boletin->espanol + boletin->matematica +
                 boletin->cienciasNaturales + boletin->formacionCivica;
    boletin->promedio = suma / 4;
}

/* ================================================================== */
/*  ALTA DE UN ESTUDIANTE                                              */
/* ================================================================== */

/*
 * agregarEstudiante() — el flujo COMPLETO de registrar, en un solo lugar.
 *
 * Antes este mismo flujo estaba escrito dos veces: en main.c (terminal)
 * y en api_web.c (navegador). Ahora ambas interfaces llaman a esta
 * función y la regla vive una sola vez.
 *
 * Devuelve: 1 = agregado, 0 = datos invalidos, -1 = sin memoria.
 */
int agregarEstudiante(Estudiante **alumnos, int *cantidad, Estudiante *nuevo) {
    Estudiante *temporal;

    if (alumnos == NULL || cantidad == NULL || nuevo == NULL) return -1;

    if (!validarEstudiante(nuevo)) return 0;

    /* El promedio se calcula en el original para que la interfaz
     * pueda mostrarlo después de llamar a esta función. */
    calcularPromedio(&nuevo->boletin);

    temporal = ampliarEstudiantes(*alumnos, *cantidad);
    if (temporal == NULL) return -1;

    *alumnos = temporal;
    (*alumnos)[*cantidad] = *nuevo;
    (*cantidad)++;

    ordenarEstudiantes(*alumnos, *cantidad);
    guardarEstudiantes(*alumnos, *cantidad);

    return 1;
}

/* ================================================================== */
/*  BUSQUEDA                                                            */
/* ================================================================== */

/*
 * coincideNombre() — la REGLA de busqueda, definida una sola vez.
 *
 * La terminal y la version web iteran por su lado (una dibuja en
 * pantalla, otra genera JSON), pero ambas preguntan por aqui.
 *
 * 1 = hay coincidencia parcial, sin distinguir mayusculas.
 */
int coincideNombre(const Estudiante *alumno, const char *texto) {
    if (alumno == NULL || texto == NULL || texto[0] == '\0') return 0;

    return strcasestr(alumno->nombre, texto) != NULL;
}
