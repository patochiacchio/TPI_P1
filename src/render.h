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

/**
 * Imprime el tablero al final de la partida (modo 'derrota').
 * Muestra:
 * - Minas no descubiertas:    "💣"
 * - Banderas correctas:       "🚩"
 * - Banderas incorrectas:     "❌"
 * - Celdas reveladas:         '0..8'
 *
 * @param tablero puntero válido (solo lectura).
 * @pre tablero != NULL.
 * @returns void.
 * @post Tablero final impreso por stdout.
 */
void render_imprimir_final(const tablero_t *tablero);

/**
 * Muestra la pantalla final de victoria o derrota.
 *
 * @param tablero Puntero al tablero final (solo lectura).
 * @param estado  El estado final (JUEGO_GANADO o JUEGO_PERDIDO).
 * @pre tablero != NULL.
 * @returns void.
 * @post Imprime el tablero y el mensaje final.
 */
void render_partida_terminada(const tablero_t *tablero, estado_juego_t estado);

#endif /* RENDER_H */
