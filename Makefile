#=====================================================================
#  Makefile — Registro de Estudiantes (versión terminal)
#
#  Uso (desde la raíz del proyecto):
#    make          compila en ../registro-no-codigo/bin/registro_estudiantes
#    make run      compila y ejecuta
#    make clean    borra los archivos que genera este Makefile
#    make wasm     compila la versión WebAssembly (requiere emsdk)
#
#  Todo lo que NO es código (binario, .o y estudiantes.dat) se genera
#  FUERA del proyecto, en ../registro-no-codigo/, para que la carpeta
#  del proyecto contenga únicamente código fuente.
#
#  No hace falta -Iinclude: cada .c incluye sus cabeceras con rutas
#  relativas al propio archivo.
#=====================================================================

# Carpeta hermana donde se deja lo generado (fuera del proyecto)
GENERADO = ../registro-no-codigo

CC       = gcc
CFLAGS   = -std=gnu11 -Wall -Wextra -g -DRUTA_DATOS='"$(GENERADO)/estudiantes.dat"'
BIN      = $(GENERADO)/bin/registro_estudiantes

SRC      = main.c \
           src/funciones.c \
           src/validacion.c \
           src/memoria.c \
           src/terminal.c \
           src/estudiante.c \
           src/boletin.c \
           src/reporte.c \
           src/menu.c

OBJ      = $(SRC:%.c=$(GENERADO)/bin/%.o)
DEP      = $(OBJ:.o=.d)

WASM     = ../registro-estudiantes-web/herramientas/build_wasm.sh


.PHONY: all run clean wasm


all: $(BIN)


$(BIN): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(OBJ) -o $@


$(GENERADO)/bin/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@


run: $(BIN)
	$(BIN)


wasm:
	./$(WASM)


# $(GENERADO)/ es 100% generado por este Makefile (binario, objetos y
# dependencias), así que se borra entero. Con eso también desaparecen
# restos de builds viejas que este archivo no generó.
# estudiantes.dat NO se toca: son los datos del usuario.
clean:
	rm -rf $(GENERADO)/bin


# Los headers cuentan como dependencia: si cambia un .h se recompila
# lo que corresponda.
-include $(DEP)
