/*
 * main.c — Sistema de Registro de Estudiantes (terminal)
 *
 * En main SOLO se ven llamadas a funciones: la logica del sistema
 * vive en src/ y la interfaz de pantalla tambien (src/terminal.c).
 *
 * Compilar: make          (ver Makefile)
 *           sin make: gcc main.c + todos los .c de src/
 *
 * (la version WASM no usa este archivo: ver
 *  ../registro-estudiantes-web/herramientas/build_wasm.sh)
 */

#include <stdio.h>

#include "include/estructuras.h"
#include "include/funciones.h"
#include "include/validacion.h"
#include "include/memoria.h"
#include "include/terminal.h"
#include "include/menu.h"


/*=========================================================
                         MAIN
=========================================================*/

int main(void)
{
    Estudiante *alumnos;

    int cantidad;


    /*-----------------------------------------------------
        INICIALIZAR PUNTEROS
    -----------------------------------------------------*/

    alumnos = NULL;


    /*-----------------------------------------------------
        CONFIGURAR TERMINAL
    -----------------------------------------------------*/

    configurarTerminal();


    /*-----------------------------------------------------
        CARGAR DATOS DEL DISCO
    -----------------------------------------------------*/

    cantidad =
        cargarEstudiantes(
            &alumnos
        );


    /*-----------------------------------------------------
        MENU PRINCIPAL
    -----------------------------------------------------*/

    ejecutarMenuPrincipal(
        &alumnos,
        &cantidad
    );


    /*-----------------------------------------------------
        LIBERAR MEMORIA
    -----------------------------------------------------*/

    liberarEstudiantes(
        &alumnos
    );


    return 0;
}
