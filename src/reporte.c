/*
 * reporte.c — Listado y busqueda en pantalla (version terminal).
 *
 * Imprime la tabla con todos los estudiantes y muestra el detalle
 * de los que coinciden con una busqueda por nombre. La REGLA de
 * busqueda (coincideNombre) no vive aqui: es logica pura y esta en
 * funciones.c, compartida con la version WebAssembly.
 *
 * NO entra al WebAssembly.
 */

#include <stdio.h>

#include "../include/reporte.h"
#include "../include/terminal.h"
#include "../include/funciones.h"
#include "../include/constantes.h"

/* ================================================================== */
/*  LISTADO GENERAL                                                   */
/* ================================================================== */

/* Listado general de todos los estudiantes registrados */
void imprimirInformacion(const Estudiante *alumnos, int cantidad) {
    int i, fila;

    limpiarPantalla();
    titulo("LISTADO DE ESTUDIANTES REGISTRADOS");

    if (cantidad == 0) {
        gotoxy(5, 5);
        backgroundColor(AMARILLO, NEGRO);
        printf("No hay estudiantes registrados todavia.");
        printf("\033[0m");
        gotoxy(5, 8);
        printf("Presiona una tecla para regresar...");
        getch(); /* Funciona perfecto porque no hay "Enter" fantasma */
        return;
    }

    backgroundColor(NEGRO, GRIS);
    gotoxy(1, 4);  printf("COD");
    gotoxy(7, 4);  printf("NOMBRE");
    gotoxy(26, 4); printf("EDAD");
    gotoxy(31, 4); printf("AREA");
    gotoxy(46, 4); printf("AULA");
    gotoxy(54, 4); printf("GRUPO");
    gotoxy(62, 4); printf("ESP");
    gotoxy(69, 4); printf("MAT");
    gotoxy(76, 4); printf("CIEN");
    gotoxy(83, 4); printf("CIV");
    gotoxy(90, 4); printf("PROM");
    printf("\033[0m");

    gotoxy(1, 5);
    printf("--------------------------------------------------------------------------------------------------");

    fila = 6;
    for (i = 0; i < cantidad; i++) {
        gotoxy(1, fila);  printf("%-5d", alumnos[i].codigo);
        gotoxy(7, fila);  printf("%-18.18s", alumnos[i].nombre);
        gotoxy(26, fila); printf("%-3d", alumnos[i].edad);
        gotoxy(31, fila); printf("%-14.14s", alumnos[i].areaConocimiento);
        gotoxy(46, fila); printf("%-7.7s", alumnos[i].aula);
        gotoxy(54, fila); printf("%-7.7s", alumnos[i].grupo);
        gotoxy(62, fila); printf("%-5.1f", alumnos[i].boletin.espanol);
        gotoxy(69, fila); printf("%-5.1f", alumnos[i].boletin.matematica);
        gotoxy(76, fila); printf("%-5.1f", alumnos[i].boletin.cienciasNaturales);
        gotoxy(83, fila); printf("%-5.1f", alumnos[i].boletin.formacionCivica);

        if (alumnos[i].boletin.promedio >= NOTA_APROBACION) {
            backgroundColor(VERDE, NEGRO);
        } else {
            backgroundColor(ROJO, NEGRO);
        }

        gotoxy(90, fila);
        printf("%-6.2f", alumnos[i].boletin.promedio);
        printf("\033[0m");

        fila++;
    }

    gotoxy(1, fila + 1);
    printf("--------------------------------------------------------------------------------------------------");
    gotoxy(1, fila + 2);
    printf("Total de estudiantes almacenados: %d", cantidad);

    gotoxy(1, fila + 4);
    printf("Presiona una tecla para regresar al menu...");
    getch();
}

/* ================================================================== */
/*  BUSQUEDA POR NOMBRE                                               */
/* ================================================================== */

/* Busqueda parcial por nombre (no distingue mayusculas/minusculas) */
void buscarEstudiante(const Estudiante *alumnos, int cantidad) {
    char nombreBusqueda[50];
    int i, encontrados = 0;

    limpiarPantalla();
    titulo("BUSQUEDA DE ESTUDIANTES");

    if (cantidad == 0) {
        gotoxy(5, 5);
        backgroundColor(ROJO, NEGRO);
        printf("ADVERTENCIA: No hay registro de ese estudiante (Base de datos vacia).");
        printf("\033[0m");
        gotoxy(5, 8);
        printf("Presiona una tecla para continuar...");
        getch();
        return;
    }

    leerTexto(5, 4, "Ingrese el nombre del estudiante a buscar: ", nombreBusqueda, 50);

    for (i = 0; i < cantidad; i++) {
        if (coincideNombre(&alumnos[i], nombreBusqueda)) {
            encontrados++;

            limpiarPantalla();
            titulo("ESTUDIANTE ENCONTRADO");

            gotoxy(5, 4);
            backgroundColor(VERDE, NEGRO);
            printf("RESULTADO DE LA BUSQUEDA");
            printf("\033[0m");

            gotoxy(5, 6);  printf("Codigo              : %d", alumnos[i].codigo);
            gotoxy(5, 7);  printf("Nombre              : %s", alumnos[i].nombre);
            gotoxy(5, 8);  printf("Edad                : %d anos", alumnos[i].edad);
            gotoxy(5, 9);  printf("Area Conocimiento   : %s", alumnos[i].areaConocimiento);
            gotoxy(5, 10); printf("Aula Asignada       : %s", alumnos[i].aula);
            gotoxy(5, 11); printf("Grupo               : %s", alumnos[i].grupo);

            gotoxy(55, 4);
            backgroundColor(CELESTE, NEGRO);
            printf("NOTAS DEL BOLETIN");
            printf("\033[0m");

            gotoxy(55, 6);  printf("Espanol            : %.2f", alumnos[i].boletin.espanol);
            gotoxy(55, 7);  printf("Matematica         : %.2f", alumnos[i].boletin.matematica);
            gotoxy(55, 8);  printf("Ciencias Naturales : %.2f", alumnos[i].boletin.cienciasNaturales);
            gotoxy(55, 9);  printf("Formacion Civica   : %.2f", alumnos[i].boletin.formacionCivica);
            gotoxy(55, 11); printf("Promedio General   : %.2f", alumnos[i].boletin.promedio);

            gotoxy(5, 14);
            printf("Presiona una tecla para continuar la busqueda...");
            getch();
        }
    }

    if (encontrados == 0) {
        gotoxy(5, 7);
        backgroundColor(ROJO, NEGRO);
        printf("ADVERTENCIA: No hay registro de ese estudiante.");
        printf("\033[0m");
        gotoxy(5, 10);
        printf("Presiona una tecla para regresar al menu...");
        getch();
    }
}
