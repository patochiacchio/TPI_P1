#ifndef ENTRADA_H
#define ENTRADA_H

#include <stddef.h>

/**
 * Tipo de acción del jugador.
 */
typedef enum
{
    ACCION_REVELAR = 0,  /* 'R' */
    ACCION_BANDERA = 1,  /* 'B' */
    ACCION_HINT    = 2,  /* 'H' */
    ACCION_SALIR   = 3   /* 'S' */
} accion_t;

/**
 * Lee una jugada desde stdin en una sola pasada.
 * Formato esperado: <accion> <fila> <columna>
 * - accion: R (revelar), B (bandera), H (hint), S (salir).
 * - fila, columna: enteros en [0, max_filas) y [0, max_columnas).
 *
 * @param accion   puntero de salida a la acción leída.
 * @param fila     puntero de salida a fila.
 * @param columna  puntero de salida a columna.
 * @param max_filas     cantidad total de filas del tablero.
 * @param max_columnas  cantidad total de columnas del tablero.
 *
 * @pre accion, fila y columna != NULL.
 * @returns 1 si entrada válida; 0 si formato/rangos inválidos.
 * @post No imprime nada salvo que se use desde main con mensajes propios.
 */
int entrada_leer_jugada(accion_t *accion, size_t *fila, size_t *columna, size_t max_filas, size_t max_columnas);

/**
 * Lee configuración del tablero desde stdin en una sola pasada.
 * Formato esperado: <filas> <columnas> <minas>
 * Valida:
 * - 5 <= filas   <= 30
 * - 5 <= columnas<= 30
 * - 1 <= minas   <  filas*columnas
 *
 * @param filas     puntero de salida a filas.
 * @param columnas  puntero de salida a columnas.
 * @param minas     puntero de salida a minas.
 * @returns 1 si válido; 0 si formato/rangos inválidos.
 * @post No imprime nada.
 */
int entrada_leer_configuracion(size_t *filas, size_t *columnas, size_t *minas);

#endif /* ENTRADA_H */
