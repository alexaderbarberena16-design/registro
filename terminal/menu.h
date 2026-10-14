#ifndef MENU_H
#define MENU_H

/*
 * menu.h — Menú principal de la versión de terminal.
 *
 * SOLO lo usa main.c. El resto del programa (nucleo/, terminal.c)
 * no debe incluir este archivo: el menú es la capa más alta.
 */

#include "../nucleo/estudiantes.h"

/* Bucle principal: muestra el menú y ejecuta la opción elegida
 * hasta que el usuario elige 4 (Salir). */
void ejecutarMenuPrincipal(Estudiante **alumnos, int *cantidad);

#endif /* MENU_H */
