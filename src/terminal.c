/*
 * terminal.c — Interfaz de terminal del Sistema de Registro de Estudiantes.
 *
 * Contiene TODO lo que depende de la consola: getch(), gotoxy(),
 * colores ANSI, scanf() para la entrada, formularios, listado y
 * busqueda en pantalla.
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
#include "../include/funciones.h"

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

/* ================================================================== */
/*  FORMULARIOS                                                       */
/* ================================================================== */

/* Formulario de datos personales */
void llenarInformacionAlumno(Estudiante *alumno, int numero) {
    gotoxy(5, 4);
    backgroundColor(AMARILLO, NEGRO);
    printf("INFORMACION DEL ESTUDIANTE #%d", numero);
    printf("\033[0m");

    alumno->codigo = leerEntero(5, 6, "Codigo: ", CODIGO_MINIMO, CODIGO_MAXIMO);
    leerTexto(5, 8, "Nombre: ", alumno->nombre, 50);
    alumno->edad = leerEntero(5, 10, "Edad: ", EDAD_MINIMA, EDAD_MAXIMA);

    leerTexto(5, 12, "Area de Conocimiento: ", alumno->areaConocimiento, 50);
    leerTexto(5, 14, "Aula Asignada: ", alumno->aula, 20);
    leerTexto(5, 16, "Grupo: ", alumno->grupo, 20);
}

/* Formulario del boletin de notas */
void llenarInformacionBoletin(Estudiante *alumno) {
    gotoxy(55, 4);
    backgroundColor(CELESTE, NEGRO);
    printf("BOLETIN DE NOTAS");
    printf("\033[0m");

    gotoxy(55, 6);
    printf("Estudiante: %s", alumno->nombre);

    alumno->boletin.espanol = leerFlotante(55, 8, "Espanol (0-100): ", NOTA_MINIMA, NOTA_MAXIMA);
    alumno->boletin.matematica = leerFlotante(55, 10, "Matematica (0-100): ", NOTA_MINIMA, NOTA_MAXIMA);
    alumno->boletin.cienciasNaturales = leerFlotante(55, 12, "Ciencias Naturales (0-100): ", NOTA_MINIMA, NOTA_MAXIMA);
    alumno->boletin.formacionCivica = leerFlotante(55, 14, "Formacion Civica (0-100): ", NOTA_MINIMA, NOTA_MAXIMA);
}

/* ================================================================== */
/*  IMPRESION Y BUSQUEDA                                              */
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
