/*
 * menu.c — Menu principal de la version de terminal.
 *
 * Aca vive el dibujo del menu y el flujo de cada opcion. NO contiene
 * logica de negocio: lo unico que hace es pedir datos a terminal.c
 * y delegar en agregarEstudiante() (src/).
 *
 * Este archivo NO entra al WebAssembly.
 */

#include <stdio.h>

#include "../include/menu.h"
#include "../include/terminal.h"
#include "../include/funciones.h"
#include "../include/constantes.h"

/* ================================================================== */
/*  PANTALLAS                                                          */
/* ================================================================== */

/* Dibuja el menu principal y devuelve la opcion elegida (1-4) */
static int mostrarMenuPrincipal(int cantidad) {
    limpiarPantalla();
    titulo("SISTEMA DE CONTROL Y REGISTRO ACADEMICO");

    gotoxy(35, 4);
    backgroundColor(AZUL, NEGRO);
    printf("--- MENU PRINCIPAL ---");
    printf("\033[0m");

    gotoxy(35, 6);  printf("1. Registrar nuevo estudiante");
    gotoxy(35, 8);  printf("2. Buscar estudiante registrado");
    gotoxy(35, 10); printf("3. Mostrar lista general de estudiantes");
    gotoxy(35, 12); printf("4. Salir");

    gotoxy(35, 14);
    backgroundColor(GRIS, NEGRO);
    printf("[Registros en disco: %d]", cantidad);
    printf("\033[0m");

    return leerEntero(35, 16, "Seleccione una opcion (1-4): ", 1, 4);
}

/* Despedida al salir del sistema */
static void salirDelSistema(void) {
    limpiarPantalla();
    gotoxy(35, 10);
    backgroundColor(VERDE, NEGRO);
    printf("¡Datos guardados! Gracias por utilizar el sistema.");
    printf("\033[0m");
    gotoxy(35, 12);
    getch();
    limpiarPantalla();
}

/* ================================================================== */
/*  OPCION 1 — REGISTRAR                                               */
/* ================================================================== */

/* Pide los datos por pantalla y delega el alta en agregarEstudiante().
 * Devuelve: 1 = registrado, 0 = datos invalidos, -1 = sin memoria. */
static int registrarNuevoEstudiante(Estudiante **alumnos, int *cantidad) {
    Estudiante nuevo;
    int resultado;

    limpiarPantalla();
    titulo("REGISTRO DE ESTUDIANTE");

    llenarInformacionAlumno(&nuevo, *cantidad + 1);
    llenarInformacionBoletin(&nuevo);

    /* Validacion + memoria + promedio + orden + guardado: todo en src/ */
    resultado = agregarEstudiante(alumnos, cantidad, &nuevo);

    if (resultado == 1) {
        gotoxy(55, 16);
        printf("Promedio Calculado : %.2f", nuevo.boletin.promedio);
    }

    gotoxy(5, 19);

    if (resultado == 1) {
        backgroundColor(VERDE, NEGRO);
        printf("Estudiante registrado, ordenado y guardado en disco con exito.");
    } else if (resultado == 0) {
        backgroundColor(ROJO, NEGRO);
        printf("ERROR: datos invalidos (revisa los rangos permitidos).");
    } else {
        backgroundColor(ROJO, NEGRO);
        printf("ERROR CRITICO: No hay suficiente memoria disponible.");
    }

    printf("\033[0m");

    gotoxy(5, 21);
    printf("Presiona una tecla para continuar...");
    getch();

    return resultado;
}

/* ================================================================== */
/*  BUCLE PRINCIPAL                                                    */
/* ================================================================== */

/* Muestra el menu y ejecuta la opcion elegida hasta que salen */
void ejecutarMenuPrincipal(Estudiante **alumnos, int *cantidad) {
    int opcion = 0;

    while (opcion != 4) {
        opcion = mostrarMenuPrincipal(*cantidad);

        switch (opcion) {
            case 1: registrarNuevoEstudiante(alumnos, cantidad); break;
            case 2: buscarEstudiante(*alumnos, *cantidad);      break;
            case 3: imprimirInformacion(*alumnos, *cantidad);   break;
            case 4: salirDelSistema();                          break;
        }
    }
}
