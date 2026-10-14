#ifndef TERMINAL_H
#define TERMINAL_H


#include "estructuras.h"


/*---------------------------------------------------------
    PROTOTIPOS — utilidades de pantalla
---------------------------------------------------------*/

void configurarTerminal(void);

int  getch(void);

void gotoxy(int x, int y);

void backgroundColor(int colorTexto, int colorFondo);

void titulo(const char *texto);

void limpiarPantalla(void);

void limpiarBuffer(void);


/*---------------------------------------------------------
    PROTOTIPOS — validacion de entrada
---------------------------------------------------------*/

int   leerEntero(int x, int y, const char *label, int min, int max);

float leerFlotante(int x, int y, const char *label, float min, float max);

void  leerTexto(int x, int y, const char *label, char *destino, int maxLen);


/*---------------------------------------------------------
    PROTOTIPOS — formularios, impresion y busqueda
---------------------------------------------------------*/

void llenarInformacionAlumno(Estudiante *alumno, int numero);

void llenarInformacionBoletin(Estudiante *alumno);

void imprimirInformacion(const Estudiante *alumnos, int cantidad);

void buscarEstudiante(const Estudiante *alumnos, int cantidad);


#endif
