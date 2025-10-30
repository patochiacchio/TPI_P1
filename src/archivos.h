#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include "tablero.h"

/**
 * Guarda el estado completo del tablero en un archivo de texto.
 *
 * @param tablero Puntero al tablero actual.
 * @param nombre_archivo  Nombre del archivo destino.
 * @returns 1 si éxito, 0 si error.
 */
int archivos_guardar_partida(const tablero_t *tablero, const char *nombre_archivo);

/**
 * Carga una partida desde un archivo de texto previamente guardado.
 *
 * @param nombre_archivo  Nombre del archivo fuente.
 * @returns Puntero a tablero_t nuevo (debe liberarse con tablero_destruir),
 *          o NULL si hubo error.
 */
tablero_t *archivos_cargar_partida(const char *nombre_archivo);

#endif /* ARCHIVOS_H */
