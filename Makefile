#=====================================================================
#  Makefile — Registro de Estudiantes (versión terminal)
#
#  Uso (desde la raíz del proyecto):
#    make          compila en bin/registro_estudiantes
#    make run      compila y ejecuta
#    make clean    borra los archivos que genera este Makefile
#    make wasm     compila la versión WebAssembly (requiere emsdk)
#
#  No hace falta -Iinclude: cada .c incluye sus cabeceras con rutas
#  relativas al propio archivo.
#=====================================================================

CC       = gcc
CFLAGS   = -std=gnu11 -Wall -Wextra -g
BIN      = bin/registro_estudiantes

SRC      = main.c \
           nucleo/funciones.c \
           nucleo/validacion.c \
           nucleo/memoria.c \
           terminal/terminal.c \
           terminal/menu.c

OBJ      = $(SRC:%.c=bin/%.o)
DEP      = $(OBJ:.o=.d)

WASM     = ../registro-estudiantes-web/herramientas/build_wasm.sh


.PHONY: all run clean wasm


all: $(BIN)


$(BIN): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(OBJ) -o $@


bin/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@


run: $(BIN)
	./$(BIN)


wasm:
	./$(WASM)


# bin/ es 100% generado por este Makefile (README: "binarios y objetos
# compilados (no son código fuente)"), así que se borra entero. Con eso
# también desaparecen restos de builds viejas que este archivo no generó
# (headers precompilados, .o de fuentes que ya no existen, etc.).
clean:
	rm -rf bin


# Los headers cuentan como dependencia: si cambia un .h se recompila
# lo que corresponda.
-include $(DEP)
