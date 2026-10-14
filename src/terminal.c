/*
 * terminal.c — Utilidades de pantalla de la version terminal.
 *
 * Contiene TODO lo que depende de la consola: getch(), gotoxy(),
 * colores ANSI y la lectura de entrada (scanf) con validacion.
 *
 * Los formularios estan en estudiante.c y boletin.c, y el listado
 * y la busqueda en reporte.c.
 *
 * NO entra al WebAssembly (ver ../registro-estudiantes-web/herramientas/build_wasm.sh).
 * En el navegador, esta interfaz la reemplaza dist/index.html.
 */

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "../include/terminal.h"
#include "../include/constantes.h"

/* ================================================================== */
/*  CONFIGURACION                                                      */
/* ================================================================== */

/* Redimensiona la ventana de la terminal (antes vivia en main.c) */
void configurarTerminal(void) {
    printf("\033[8;35;105t");
}

/* ================================================================== */
/*  UTILIDADES DE PANTALLA                                            */
/* ================================================================== */

/* getch() para Linux: lee una tecla sin pulsar Enter */
int getch(void) {
    struct termios oldattr, newattr;
    int ch;

    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

    return ch;
}

/* Coloca el cursor en la posicion (x, y) */
void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
}

/* Color de texto sobre fondo de color */
void backgroundColor(int colorTexto, int colorFondo) {
    int fondoFinal = colorFondo + 10;
    if (colorFondo == BLANCO) fondoFinal = 107;

    printf("\033[%d;%dm", colorTexto, fondoFinal);
}

/* Barra de titulo superior */
void titulo(const char *texto) {
    backgroundColor(BLANCO, AZUL);
    gotoxy(0, 1);
    printf("                                                                                                     m ");
    gotoxy(35, 1);
    printf("%s", texto);
    printf("\033[0m");
}

/* Borra la pantalla completa */
void limpiarPantalla(void) {
    printf("\033[H\033[J");
}

/* Descarta el '\n' sobrante en el buffer de stdin */
void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* ================================================================== */
/*  VALIDACIONES DE ENTRADA                                           */
/* ================================================================== */

/* Lee un entero dentro del rango [min, max]; reintenta si es invalido */
int leerEntero(int x, int y, const char *label, int min, int max) {
    int valor;

    while (1) {
        gotoxy(x, y);
        printf("%s", label);
        printf("                      ");
        gotoxy(x + strlen(label), y);

        if (scanf("%d", &valor) == 1 && valor >= min && valor <= max) {
            limpiarBuffer(); /* Limpia el 'Enter' sobrante al leer con exito */
            gotoxy(x, y + 1);
            printf("                                                            ");
            return valor;
        }

        limpiarBuffer(); /* Limpia si hubo un error de escritura (letras en vez de numeros) */
        gotoxy(x, y + 1);
        backgroundColor(ROJO, NEGRO);
        printf("ERROR: Ingrese un valor entero valido (%d a %d)", min, max);
        printf("\033[0m");
    }
}

/* Lee un flotante dentro del rango [min, max]; reintenta si es invalido */
float leerFlotante(int x, int y, const char *label, float min, float max) {
    float valor;

    while (1) {
        gotoxy(x, y);
        printf("%s", label);
        printf("                      ");
        gotoxy(x + strlen(label), y);

        if (scanf("%f", &valor) == 1 && valor >= min && valor <= max) {
            limpiarBuffer(); /* Limpia el 'Enter' sobrante al leer con exito */
            gotoxy(x, y + 1);
            printf("                                                            ");
            return valor;
        }

        limpiarBuffer();
        gotoxy(x, y + 1);
        backgroundColor(ROJO, NEGRO);
        printf("ERROR: Ingrese una nota valida (%.1f a %.1f)", min, max);
        printf("\033[0m");
    }
}

/* Lee una cadena no vacia de hasta maxLen-1 caracteres (evita desbordar
 * el buffer destino, que puede ser de 20 o de 50 bytes) */
void leerTexto(int x, int y, const char *label, char *destino, int maxLen) {
    char formato[32];
    snprintf(formato, sizeof(formato), " %%%d[^\n]", maxLen - 1);

    while (1) {
        gotoxy(x, y);
        printf("%s", label);
        printf("                                    ");
        gotoxy(x + strlen(label), y);

        if (scanf(formato, destino) == 1 && strlen(destino) > 0) {
            limpiarBuffer(); /* Limpia el 'Enter' sobrante al leer con exito */
            gotoxy(x, y + 1);
            printf("                                                            ");
            return;
        }

        limpiarBuffer();
        gotoxy(x, y + 1);
        backgroundColor(ROJO, NEGRO);
        printf("ERROR: Entrada de texto invalida.");
        printf("\033[0m");
    }
}
