# Registro de Estudiantes — Terminal

Proyecto en **C** organizado en módulos y cortado como el proyecto
**15-Modular-Boletin** del curso (programación estructurada): cabeceras en
`include/` (una por módulo, más `estructuras.h` y `constantes.h`) y las
implementaciones en `src/`, con un `main.c` que solo hace llamadas.
Corre en la terminal.

La misma lógica se compila también a **WebAssembly** para el navegador; esa
parte vive en una carpeta aparte:

```
/home/chris/studio/
├── c/registro de estudiantes_proyet/     ← ESTE PROYECTO (terminal)
├── registro-estudiantes-web/             ← parte web (api_web + dist + build_wasm)
```

| | Terminal | Navegador |
|---|---|---|
| Código | `main.c` + `src/funciones.c` + `src/terminal.c` | `../registro-estudiantes-web/api_web/api_web.c` + `src/funciones.c` |
| Compilador | `gcc` | `emcc` (Emscripten) |
| Interfaz | `gotoxy` / `getch` / colores ANSI | `../registro-estudiantes-web/dist/index.html` |
| Salida | `bin/registro_estudiantes` | `../registro-estudiantes-web/web/registro_web.js` + `.wasm` |

Los archivos de lógica (`src/funciones.c`, `src/validacion.c`, `src/memoria.c`
y las cabeceras de `include/`) **son los mismos en los dos casos**: no se
duplica la lógica.

> Salvo indicación, todos los comandos se ejecutan **desde la raíz de este
> proyecto** (`.../c/registro de estudiantes_proyet`).

---

## 1. Compilar para la terminal

```bash
gcc -Wall main.c src/funciones.c src/terminal.c -o bin/registro_estudiantes
./bin/registro_estudiantes
```

Con `make` (si está disponible):

```bash
make
make run
```

> Ojo: hay que pasar **todos** los `.c` del proyecto. Si falta `src/terminal.c`
> recibes `referencia a 'gotoxy' sin definir` y similares (son errores de enlace).

---

## 2. Compilar para WebAssembly

La parte web **no está dentro de este proyecto**: se movió a
`/home/chris/studio/registro-estudiantes-web/`.

```bash
# 1) activar Emscripten (solo la primera vez por terminal)
source /home/chris/emsdk/emsdk_env.sh

# 2) compilar (desde cualquier carpeta)
/home/chris/studio/registro-estudiantes-web/herramientas/build_wasm.sh
```

El script toma la lógica de `src/` (de este proyecto: `funciones.c`,
`validacion.c` y `memoria.c`) y la fachada de
`../registro-estudiantes-web/api_web/api_web.c`, y genera:

```
../registro-estudiantes-web/web/registro_web.js      (pegamento de Emscripten)
../registro-estudiantes-web/web/registro_web.wasm    (tu lógica en C)
```

Para que el navegador use los archivos recién compilados, cópialos sobre `dist/`:

```bash
cd /home/chris/studio/registro-estudiantes-web
cp web/registro_web.js web/registro_web.wasm dist/
```

### Verlo en el navegador

```bash
cd /home/chris/studio/registro-estudiantes-web/dist
python3 -m http.server 8080
```

Abre <http://localhost:8080/index.html>

> **No abras `index.html` con doble clic (`file://`).** El navegador bloquea la
> carga de `.wasm` por CORS en `file://` y verás *"No se pudo cargar
> registro_web.wasm"*. Siempre por HTTP.

Para parar el servidor: `Ctrl+C`, o desde otra terminal:
`pkill -f "http.server 8080"`

---

## 3. Cómo quedó organizado el código

```
c/registro de estudiantes_proyet/
├── main.c                ← punto de entrada; en main SOLO hay llamadas (NO entra al wasm)
├── Makefile              ← make / make run / make clean / make wasm
├── README.md             ← este documento
├── include/              ← cabeceras: una por módulo (como el proyecto del curso)
│   ├── estructuras.h     ← typedefs Boletin y Estudiante (compartido)
│   ├── constantes.h      ← colores ANSI y CONSTANTES del negocio (compartido)
│   ├── funciones.h       ← prototipos de funciones.c
│   ├── validacion.h      ← prototipos de validacion.c
│   ├── memoria.h         ← prototipos de memoria.c
│   ├── terminal.h        ← prototipos de terminal.c (pantalla y entrada)
│   └── menu.h            ← prototipo del menú principal
├── src/                  ← implementaciones (un .c por módulo)
│   ├── funciones.c       ← promedio, ordenamiento, .dat y agregarEstudiante()
│   ├── validacion.c      ← validarNota() / validarEstudiante() (los rangos, UNA sola vez)
│   ├── memoria.c         ← ampliarEstudiantes() / liberarEstudiantes()
│   ├── terminal.c        ← getch, gotoxy, scanf, formularios, listado y búsqueda
│   └── menu.c            ← menú principal y flujo de cada opción
├── bin/                  ← binarios y objetos compilados (no son código fuente)
│   └── registro_estudiantes
└── estudiantes.dat       ← datos guardados en disco
```

Fuera de este proyecto (parte web):

```
studio/registro-estudiantes-web/
├── api_web/api_web.c         ← fachada JSON que consume el navegador (SÍ entra al wasm)
├── herramientas/build_wasm.sh
└── dist/                     ← index.html + registro_web.js + registro_web.wasm (listo para desplegar)
```

> **Regla de organización:** en `main.c` solo se ven llamadas; cada `.c`
> incluye primero **su propia cabecera** de `include/` y después las demás
> que necesita. Los módulos que no tocan la consola (`funciones`,
> `validacion`, `memoria`) son los que se compilan para el wasm: el menú y
> la terminal (`src/terminal.c`, `src/menu.c`) se quedan fuera.

### ¿Por qué `api_web.c` y no directamente `main.c`?

Porque `main.c` y `src/terminal.c` dependen de la terminal (y por eso **no**
entran al wasm; los módulos de lógica ya no contienen nada de consola):

| Función | Por qué no sirve en el navegador |
|---|---|
| `getch()` | usa `termios` para leer una tecla de stdin |
| `gotoxy()` | imprime `\033[y;xH`, código ANSI de consola |
| `backgroundColor()` | imprime `\033[3X;4Xm` |
| `leerEntero/Flotante/Texto()` | leen de `stdin` con `scanf()` |
| `limpiarPantalla()` | imprime `\033[H\033[J` |
| `imprimirInformacion()` | combina todo lo anterior |

En su lugar, `api_web/api_web.c` envuelve **solo la lógica**:

| Función en C | Qué hace el navegador con ella |
|---|---|
| `calcularPromedio()` | calcula el promedio (¡no lo hace JavaScript!) |
| `ordenarEstudiantes()` / `compararEstudiantes()` | ordena con `qsort` |
| `guardarEstudiantes()` / `cargarEstudiantes()` | escribe/lee `estudiantes.dat` en la FS de Emscripten |
| `webAgregar()` | registra + valida rangos + calcula + ordena + guarda |
| `webListado()` | devuelve el array como JSON |
| `webBuscar()` | búsqueda parcial por nombre, sin distinguir mayúsculas |
| `webEliminar()`, `webVaciar()`, `webCantidad()` | mantenimiento |
| `webPromedio()` | promedio suelto, para la vista previa del formulario |

---

## 4. Reglas del puente C ↔ JavaScript

En `dist/index.html`:

```js
// DEFINIR Module ANTES de cargar registro_web.js
var Module = {
    onRuntimeInitialized: function () { /* ...aquí ya hay cwrap... */ }
};
```

```html
<script src="registro_web.js"></script>   <!-- ← después -->
```

Si defines `Module` *después* del pegamento, pierdes `cwrap`/`ccall` y aparece
`Module.cwrap is not a function`.

Los enlaces se crean con la firma de Emscripten:

```js
const f = Module.cwrap('nombreEnC', tipoRetorno, [tiposDeParametros]);
//  'number' → int / float          'string' → char*
//  'null'   → void
```

Toda función que quieras llamar desde JS **debe** llevar
`EMSCRIPTEN_KEEPALIVE` en el C **y** aparecer en `-sEXPORTED_FUNCTIONS`
(de lo contrario el linker la elimina por ser "código muerto").

---

## 5. Persistencia de datos

La memoria del `.wasm` desaparece al recargar la página. Por eso el `index.html`
guarda el listado en `localStorage` (`registro_estudiantes_v1`) y lo restaura
llamando a `webAgregar()` uno por uno al arrancar.

`estudiantes.dat` se escribe igual que siempre, pero dentro del sistema de
archivos en memoria (MEMFS) de Emscripten: sirve para que `guardarEstudiantes`
y `cargarEstudiantes` funcionen sin cambiar, pero **no** sobrevive al recargar.

Para borrar los datos guardados en el navegador: botón *Vaciar listado*, o en la
consola del navegador `localStorage.clear()`.

---

## 6. Si cambias el código C

```bash
# terminal (recomendado)
make

# terminal (a mano)
gcc -Wall main.c src/funciones.c src/validacion.c src/memoria.c \
    src/terminal.c src/menu.c -o bin/registro_estudiantes

# wasm (necesita source /home/chris/emsdk/emsdk_env.sh primero)
make wasm
cp ../registro-estudiantes-web/web/registro_web.js \
   ../registro-estudiantes-web/web/registro_web.wasm \
   ../registro-estudiantes-web/dist/
```

Y recarga la página con **Ctrl+Shift+R** (hard refresh) para que no quede el
`.wasm` en caché.

---

## 7. Solución de problemas

| Síntoma | Causa | Solución |
|---|---|---|
| `emcc: orden no encontrado` | SDK sin activar | `source /home/chris/emsdk/emsdk_env.sh` |
| *"El módulo WebAssembly aún no está listo"* | `window.x` y `let x` son **dos variables distintas** (el `let` sombrea a `window`) | declarar `let wasmListo` y asignarlo **con el mismo identificador**, nunca `window.wasmListo` |
| `JSON.parse` recibe código de una función | faltaba la **segunda llamada**: `cwrap()` devuelve una función, no el resultado | `const f = Module.cwrap(...); f();` — guardar la función y luego invocarla |
| `No se pudo cargar registro_web.wasm` | abierto con `file://` o sin compilar | servidor HTTP + `build_wasm.sh` |
| `Module.cwrap is not a function` | `Module` definido tarde | definirlo **antes** del `<script src>` |
| `undefined reference to 'gotoxy'` | faltan archivos al enlazar | `make` (o pasa **todos** los `.c`: los cinco de `src/` más `main.c`) |
| `estructuras.h: No such file or directory` (wasm) | el script no encontró el proyecto terminal | revisa la variable `PROYECTO` de `build_wasm.sh` |
| Página vieja tras recompilar | caché del navegador | `Ctrl+Shift+R` |
| Puerto ocupado | otro proceso en 8080 | `python3 -m http.server 8081` |

### Los dos errores que más dan la brasa

**1. `window.x` ≠ `let x`**

```js
// script #1
window.wasmListo = true;   // ← propiedad del objeto global

// script #2
let wasmListo = false;     // ← binding léxico, SOBREA a la línea de arriba

// función que lee la variable
if (!wasmListo) ...   // ← siempre false, aunque el módulo ya cargó
```

Dos casilleros con el mismo nombre. Se lee el `let`, que nadie toca.
**Solución:** declarar y asignar **el mismo** identificador en todos lados.

**2. `cwrap()` no ejecuta, devuelve una función**

```js
const fMal  = Module.cwrap('webListado', 'string', []);
const json  = fMal;        // ❌ es una función
const json2 = fMal();      // ✅ esto ejecuta la función en C

// atajo correcto (se invoca al llamar):
const f = () => Module.cwrap('webListado', 'string', [])();
```
