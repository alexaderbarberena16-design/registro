/*
 * main.c — Sistema de Registro de Estudiantes (terminal)
 *
 * En main SOLO se ven llamadas a funciones: la lógica del sistema
 * vive en nucleo/ y la interfaz de pantalla en terminal/.
 *
 * Compilar: make          (ver Makefile)
 *           sin make: gcc main.c + todos los .c de nucleo/ y terminal/
 *
 * (la version WASM no usa este archivo: ver
 *  ../registro-estudiantes-web/herramientas/build_wasm.sh)
 */

#include "nucleo/estudiantes.h"
#include "terminal/terminal.h"
#include "terminal/menu.h"

/* ================================================================== */
/*  MAIN — solo llamadas                                               */
/* ================================================================== */

int main(void) {
    Estudiante *alumnos = NULL;
    int cantidad = 0;

    configurarTerminal();
    cantidad = cargarEstudiantes(&alumnos);
    ejecutarMenuPrincipal(&alumnos, &cantidad);
    liberarEstudiantes(&alumnos);

    return 0;
}
