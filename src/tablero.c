#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "tablero.h"

/* Solo visibles en este módulo: administración de la matriz dinámica. */
static celda_t **reservar_grilla(size_t filas, size_t columnas);
static void liberar_grilla(celda_t **grilla, size_t filas);

tablero_t *tablero_crear(size_t filas, size_t columnas, size_t cantidad_minas)
{
    tablero_t *tablero = NULL;
    int ok = 1;
    size_t i = 0;
    size_t j = 0;

    /* Reserva del struct principal */
    tablero = (tablero_t *)calloc(1, sizeof(*tablero));
    if (tablero == NULL)
    {
        ok = 0;
    }

    /* Data + reserva de la matriz de celdas */
    if (ok == 1)
    {
        tablero->filas = filas;
        tablero->columnas = columnas;
        tablero->cantidad_minas = cantidad_minas;
        tablero->grilla = reservar_grilla(filas, columnas);

        if (tablero->grilla == NULL)
        {
            ok = 0;
        }
    }

    /* Inicialización explícita */
    if (ok == 1)
    {
        i = 0;
        while (i < filas)
        {
            j = 0;
            while (j < columnas)
            {
                tablero->grilla[i][j].es_mina = false;
                tablero->grilla[i][j].minas_alrededor = 0;
                tablero->grilla[i][j].estado = CELDA_OCULTA;
                j = j + 1;
            }
            i = i + 1;
        }
    }

    /* Ctrl+Z limpio ante falla de cualquier paso */
    if (ok == 0)
    {
        if (tablero != NULL)
        {
            if (tablero->grilla != NULL)
            {
                liberar_grilla(tablero->grilla, filas);
                tablero->grilla = NULL;
            }
            free(tablero);
            tablero = NULL;
        }
    }

    return tablero;
}

void tablero_destruir(tablero_t *tablero)
{
    if (tablero != NULL)
    {
        if (tablero->grilla != NULL)
        {
            liberar_grilla(tablero->grilla, tablero->filas);
            tablero->grilla = NULL;
        }
        free(tablero);
    }
}

bool tablero_en_rango(const tablero_t *tablero, size_t fila, size_t columna)
{
    bool en_rango = false;

    if (tablero != NULL)
    {
        if (fila < tablero->filas && columna < tablero->columnas)
        {
            en_rango = true;
        }
    }
    return en_rango;
}

static celda_t **reservar_grilla(size_t filas, size_t columnas)
{
    celda_t **grilla = NULL;
    size_t i = 0;
    int ok = 1;

    grilla = (celda_t **)calloc(filas, sizeof(*grilla));
    if (grilla == NULL)
    {
        ok = 0;
    }

    i = 0;
    while (ok == 1 && i < filas)
    {
        grilla[i] = (celda_t *)calloc(columnas, sizeof(*grilla[i]));
        if (grilla[i] == NULL)
        {
            ok = 0;
        }
        i = i + 1;
    }

    if (ok == 0 && grilla != NULL)
    {
        /* Libera solo lo que se alcanzó a reservar */
        size_t k = 0;
        while (k < i)
        {
            free(grilla[k]);
            k = k + 1;
        }
        free(grilla);
        grilla = NULL;
    }

    return grilla;
}

static void liberar_grilla(celda_t **grilla, size_t filas)
{
    size_t i = 0;

    if (grilla == NULL)
    {
        return;
    }

    while (i < filas)
    {
        free(grilla[i]);
        i = i + 1;
    }
    free(grilla);
}

int tablero_colocar_minas(tablero_t *tablero, unsigned int semilla)
{
    int ok = 1;
    size_t total = 0;
    size_t a_colocar = 0;
    size_t i = 0;
    size_t k = 0;
    size_t *posiciones = NULL;

    if (tablero == NULL)
    {
        ok = 0;
    }

    /* calcular total de celdas y reservar vector 0..total-1 */
    if (ok == 1)
    {
        total = tablero->filas * tablero->columnas;
        a_colocar = tablero->cantidad_minas;
        if (a_colocar > total)
        {
            a_colocar = total;
        }

        posiciones = (size_t *)calloc(total, sizeof(*posiciones));
        if (posiciones == NULL)
        {
            ok = 0;
        }
    }

    if (ok == 1)
    {
        i = 0;
        while (i < total)
        {
            posiciones[i] = i;
            i = i + 1;
        }

        srand(semilla);
        if (total > 0)
        {
            k = total - 1;
            while (k > 0)
            {
                size_t j = (size_t)(rand() % (k + 1));
                size_t tmp = posiciones[k];
                posiciones[k] = posiciones[j];
                posiciones[j] = tmp;
                k = k - 1;
            }
        }

        i = 0;
        while (i < a_colocar)
        {
            size_t idx = posiciones[i];
            size_t fila = idx / tablero->columnas;
            size_t columna = idx % tablero->columnas;
            tablero->grilla[fila][columna].es_mina = true;
            i = i + 1;
        }
    }

    /* limpieza local */
    if (posiciones != NULL)
    {
        free(posiciones);
        posiciones = NULL;
    }

    return ok;
}
