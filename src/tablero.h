#ifndef TABLERO_H
#define TABLERO_H

#include <stddef.h>
#include <stdbool.h>

/**
 * Estados visibles de una celda.
 */
typedef enum
{
    CELDA_OCULTA = 0,
    CELDA_REVELADA = 1,
    CELDA_BANDERA = 2
} estado_celda_t;

/**
 * Datos por celda.
 */
typedef struct
{
    bool es_mina;
    unsigned char minas_alrededor;
    estado_celda_t estado;
} celda_t;

/**
 * Estructura del tablero.
 */
typedef struct
{
    size_t filas;
    size_t columnas;
    size_t cantidad_minas;
    celda_t **grilla;
} tablero_t;

/**
 * Crea un tablero vacío
 * Reserva memoria y deja todas las celdas ocultas, sin minas y con contador 0.
 * 
 * @param filas cantidad de filas (>0).
 * @param columnas cantidad de columnas (>0).
 * @param cantidad_minas cantidad total de minas a colocar luego (>=0).
 * @pre filas > 0, columnas > 0.
 * @returns Puntero a tablero_t o NULL si no hay memoria.
 * @post El llamador es dueño del puntero y debe llamar a tablero_destruir().
 */
tablero_t *tablero_crear(size_t filas, size_t columnas, size_t cantidad_minas);

/**
 * Libera toda la memoria del tablero.
 *
 * @param tablero puntero al tablero o NULL.
 * @pre ninguna.
 * @returns void.
 * @post tablero y su grilla quedan liberados.
 */
void tablero_destruir(tablero_t *tablero);

/**
 * Verifica si (fila, columna) está en rango.
 *
 * @param tablero puntero válido al tablero.
 * @param fila índice de fila.
 * @param columna índice de columna.
 * @pre tablero != NULL.
 * @returns true si está dentro, false si no.
 * @post ninguna.
 */
bool tablero_en_rango(const tablero_t *tablero, size_t fila, size_t columna);

/**
 * Coloca minas de forma aleatoria y reproducible por semilla.
 *
 * @param tablero Puntero al tablero.
 * @param semilla Valor para srand(); misma semilla => mismo patrón.
 * @pre tablero != NULL.
 * @returns 1 si ok, 0 si error (memoria o puntero inválido).
 * @post Se marcan exactamente tablero->cantidad_minas celdas como minas,
 *       o todas si cantidad_minas > filas*columnas.
 */
int tablero_colocar_minas(tablero_t *tablero, unsigned int semilla);

/**
 * Completa el número de minas vecinas (0..8) en cada celda no-mina.
 *
 * @param tablero Puntero al tablero.
 * @pre tablero != NULL.
 * @returns 1 si ok, 0 si error.
 * @post Para cada celda sin mina, se actualiza minas_alrededor con el conteo.
 */
int tablero_contar_vecinos(tablero_t *tablero);

/**
 * Revela la celda indicada. Si es 0, expande la zona.
 *
 * @param tablero Puntero al tablero.
 * @param fila Índice de fila.
 * @param columna Índice de columna.
 * @param encontro_mina puntero de salida: 1 si se reveló una mina, 0 si no.
 * @pre tablero != NULL, encontro_mina != NULL, (fila,columna) en rango.
 * @returns 1 si ok, 0 si error (puntero inválido / fuera de rango).
 * @post Si la celda no estaba revelada ni con bandera:
 *       - Si es mina: no cambia el estado visible y *encontr_mina=1.
 *       - Si no es mina y vale >0: se revela solo esa celda.
 *       - Si vale 0: se revelan ceros conectados + borde numérico.
 */
int tablero_revelar(tablero_t *tablero, size_t fila, size_t columna, int *encontro_mina);


#endif