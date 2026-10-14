#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H


/*---------------------------------------------------------
    ESTRUCTURA BOLETIN
---------------------------------------------------------*/

typedef struct
{
    float espanol;

    float matematica;

    float cienciasNaturales;

    float formacionCivica;

    float promedio;

} Boletin;


/*---------------------------------------------------------
    ESTRUCTURA ESTUDIANTE
---------------------------------------------------------*/

typedef struct
{
    int codigo;

    char nombre[50];

    int edad;

    char areaConocimiento[50];

    char aula[20];

    char grupo[20];

    Boletin boletin;

} Estudiante;


#endif
