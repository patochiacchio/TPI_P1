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

int tablero_contar_vecinos(tablero_t *tablero)
{
    int ok = 1;

    if (tablero == NULL)
    {
        ok = 0;
    }

    if (ok == 1)
    {
        size_t fila = 0;

        while (fila < tablero->filas)
        {
            size_t columna = 0;

            while (columna < tablero->columnas)
            {
                /* solo se cuenta si NO es mina */
                if (tablero->grilla[fila][columna].es_mina == false)
                {
                    unsigned char conteo = 0;
                    int df[8] = {-1, -1, -1,  0, 0,  1, 1, 1}; /* preguntar si conviene usar ptrdiff_t o con int estamos bien (funciona con int) */
                    int dc[8] = {-1,  0,  1, -1, 1, -1, 0, 1};
                    int k = 0; /* i = filas, j = columnas, k = otras_cosas */

                    /* se recorren los 8 vecinos con offsets */
                    while (k < 8)
                    {
                        int fila_vecina = (int)fila + df[k];
                        int columna_vecina = (int)columna + dc[k];

                        if (fila_vecina >= 0 && columna_vecina >= 0 && fila_vecina < (int)tablero->filas && columna_vecina < (int)tablero->columnas)
                        {
                            if (tablero->grilla[(size_t)fila_vecina][(size_t)columna_vecina].es_mina == true)
                            {
                                conteo = (unsigned char)(conteo + 1);
                            }
                        }
                        k = k + 1;
                    }

                    tablero->grilla[fila][columna].minas_alrededor = conteo;
                }

                columna = columna + 1;
            }

            fila = fila + 1;
        }
    }

    return ok;
}

int tablero_revelar(tablero_t *tablero, size_t fila, size_t columna, int *encontro_mina)
{
    int ok = 1;

    if (tablero == NULL || encontro_mina == NULL)
    {
        ok = 0;
    }
    if (ok == 1)
    {
        if (tablero_en_rango(tablero, fila, columna) == false)
        {
            ok = 0;
        }
    }

    if (ok == 1)
    {
        celda_t *celda = &tablero->grilla[fila][columna];
        *encontro_mina = 0; /* por defecto, no cayó en mina */

        if (celda->estado == CELDA_REVELADA || celda->estado == CELDA_BANDERA)
        {
            /* no hago nada si ya estaba revelada o marcada con bandera */   
        }
        else if (celda->es_mina == true)
        {
            *encontro_mina = 1;
        }
        else
        {
            /* caso no-mina */
            if (celda->minas_alrededor > 0u)
            {
                celda->estado = CELDA_REVELADA; /* número > 0: se revela esta */
            }
            else
            {
                /* número == 0: flood-fill con LIFO de pila de pendientes */
                typedef struct { size_t fila; size_t columna; } posicion_t;
                posicion_t *pendientes = NULL;
                size_t capacidad = 0;
                size_t cantidad = 0;

                /* revelar origen y apilar/desapilar si corresponde */
                celda->estado = CELDA_REVELADA;

                if (ok == 1)
                {
                    if (cantidad == capacidad)
                    {
                        size_t nueva;
                        if (capacidad == 0u)
                        {
                            nueva = 64u;
                        }
                        else
                        {
                            nueva = capacidad * 2u;
                        }

                        posicion_t *tmp = (posicion_t *)realloc(pendientes, nueva * sizeof(*pendientes));
                        if (tmp == NULL)
                        {
                            ok = 0;
                        }
                        else
                        {
                            pendientes = tmp;
                            capacidad = nueva;
                        }
                    }
                    if (ok == 1)
                    {
                        pendientes[cantidad].fila = fila;
                        pendientes[cantidad].columna = columna;
                        cantidad = cantidad + 1u;
                    }
                }

                /* offsets de los 8 vecinos */
                if (ok == 1)
                {
                    int desplazamiento_fila[8]    = {-1,-1,-1, 0,0, 1,1,1};
                    int desplazamiento_columna[8] = {-1, 0, 1,-1,1,-1,0,1};

                    /* expandir mientras haya pila de pendientes y no haya fallo de memoria */
                    while (ok == 1 && cantidad > 0u)
                    {
                        /* (LIFO) */
                        cantidad = cantidad - 1u;
                        size_t f_actual = pendientes[cantidad].fila;
                        size_t c_actual = pendientes[cantidad].columna;

                        /* revisar 8 vecinos del actual */
                        {
                            int indice_vecino = 0;
                            while (indice_vecino < 8)
                            {
                                int f_vec = (int)f_actual + desplazamiento_fila[indice_vecino];
                                int c_vec = (int)c_actual + desplazamiento_columna[indice_vecino];

                                if (f_vec >= 0 && c_vec >= 0 &&
                                    f_vec < (int)tablero->filas &&
                                    c_vec < (int)tablero->columnas)
                                {
                                    size_t fi = (size_t)f_vec;
                                    size_t ci = (size_t)c_vec;
                                    celda_t *v = &tablero->grilla[fi][ci];

                                    /* solo toco celdas ocultas y no-mina; banderas se respetan */
                                    if (v->estado == CELDA_OCULTA && v->es_mina == false)
                                    {
                                        v->estado = CELDA_REVELADA;

                                        /* si también es 0, la agrego para seguir expandiendo */
                                        if (v->minas_alrededor == 0u)
                                        {
                                            if (ok == 1)
                                            {
                                                if (cantidad == capacidad)
                                                {
                                                    size_t nueva;
                                                    if (capacidad == 0u)
                                                    {
                                                        nueva = 64u;
                                                    }
                                                    else
                                                    {
                                                        nueva = capacidad * 2u;
                                                    }

                                                    posicion_t *tmp = (posicion_t *)realloc(pendientes, nueva * sizeof(*pendientes));
                                                    if (tmp == NULL)
                                                    {
                                                        ok = 0; /* sin memoria para seguir expandiendo */
                                                    }
                                                    else
                                                    {
                                                        pendientes = tmp;
                                                        capacidad = nueva;
                                                    }
                                                }
                                                if (ok == 1)
                                                {
                                                    pendientes[cantidad].fila = fi;
                                                    pendientes[cantidad].columna = ci;
                                                    cantidad = cantidad + 1u;
                                                }
                                            }
                                        }
                                    }
                                }
                                indice_vecino = indice_vecino + 1;
                            }
                        }
                    }
                }

                /* limpieza local del vector dinámico */
                if (pendientes != NULL)
                {
                    free(pendientes);
                    pendientes = NULL;
                }
            }
        }
    }

    return ok;
}
