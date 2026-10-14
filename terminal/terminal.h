#ifndef TERMINAL_H
#define TERMINAL_H

/*
 * terminal.h — Funciones de la interfaz de terminal.
 *
 * SOLO para la version de consola (gcc). NO entra al WebAssembly:
 * en el navegador la interfaz es web/index.html.
 */

#include "../nucleo/estudiantes.h"

/* ------------------------------------------------------------------ */
/*  Colores ANSI (Foreground)                                         */
/* ------------------------------------------------------------------ */
#define NEGRO     30
#define AZUL      34
#define VERDE     32
#define CELESTE   36
#define ROJO      31
#define MAGENTA   35
#define AMARILLO  33
#define GRIS      37
#define BLANCO    97

/* ------------------------------------------------------------------ */
/*  Prototipos — utilidades de pantalla                               */
/* ------------------------------------------------------------------ */
void configurarTerminal(void);
int  getch(void);
void gotoxy(int x, int y);
void backgroundColor(int colorTexto, int colorFondo);
void titulo(const char *texto);
void limpiarPantalla(void);
void limpiarBuffer(void);

/* ------------------------------------------------------------------ */
/*  Prototipos — validacion de entrada                                */
/* ------------------------------------------------------------------ */
int   leerEntero(int x, int y, const char *label, int min, int max);
float leerFlotante(int x, int y, const char *label, float min, float max);
void  leerTexto(int x, int y, const char *label, char *destino, int maxLen);

/* ------------------------------------------------------------------ */
/*  Prototipos — formularios, impresion y busqueda                    */
/* ------------------------------------------------------------------ */
void llenarInformacionAlumno(Estudiante *alumno, int numero);
void llenarInformacionBoletin(Estudiante *alumno);
void imprimirInformacion(const Estudiante *alumnos, int cantidad);
void buscarEstudiante(const Estudiante *alumnos, int cantidad);

#endif /* TERMINAL_H */
