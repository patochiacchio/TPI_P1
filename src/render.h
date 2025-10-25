#ifndef RENDER_H
#define RENDER_H

#include "tablero.h"

/**
 * Imprime el tablero en ASCII.
 * - Celdas ocultas: '.'
 * - Banderas:       'F'
 * - Reveladas:      dígito 0..8 (por ahora no revelamos nada en esta etapa)
 *
 * @param tablero puntero válido.
 * @pre tablero != NULL.
 * @returns void.
 * @post Tablero impreso por stdout.
 */
void render_imprimir(const tablero_t *tablero);

#endif /* RENDER_H */
