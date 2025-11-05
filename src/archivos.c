#include <stdio.h>
#include <stdlib.h>
#include "archivos.h"

int archivos_guardar_partida(const tablero_t *tablero, const char *nombre_archivo)
{
    int ok = 1;
    FILE *archivo_salida = NULL;

    if (tablero == NULL || nombre_archivo == NULL)
    {
        ok = 0;
    }

    if (ok == 1)
    {
        archivo_salida = fopen(nombre_archivo, "w"); /* modo escritura (borra si existe) */
        if (archivo_salida == NULL)
        {
            ok = 0;
        }
    }

    if (ok == 1)
    {
        /* Guardamos dimensiones y cantidad de minas en la primera línea */
        if (fprintf(archivo_salida, "%zu %zu %zu\n", tablero->filas, tablero->columnas, tablero->cantidad_minas) < 0)
        {
            ok = 0;
        }
    }

    if (ok == 1)
    {
        size_t fila = 0;
        while (fila < tablero->filas)
        {
            size_t columna = 0;
            while (columna < tablero->columnas)
            {
                const celda_t *celda = &tablero->grilla[fila][columna];

                int valor_mina = 0;

                if (celda->es_mina)
                {
                    valor_mina = 1;
                }
                else
                {
                    valor_mina = 0;
                }
                /*  valor_mina, minas_alrededor (0–8), estado (0/1/2) */
                if (fprintf(archivo_salida, "%d %d %d\n", valor_mina, celda->minas_alrededor, celda->estado) < 0)
                {
                    ok = 0;
                }
                columna = columna + 1;
            }
            fila = fila + 1;
        }
    }

    if (archivo_salida != NULL)
    {
        fclose(archivo_salida);
    }

    return ok;
}

tablero_t *archivos_cargar_partida(const char *nombre_archivo)
{
    tablero_t *tablero = NULL;
    FILE *archivo_entrada = NULL;
    size_t filas = 0;
    size_t columnas = 0;
    size_t minas = 0;
    int ok = 1;

    if (nombre_archivo == NULL)
    {
        ok = 0;
    }

    if (ok == 1)
    {
        archivo_entrada = fopen(nombre_archivo, "r"); /* modo lectura */
        if (archivo_entrada == NULL)
        {
            ok = 0;
        }
    }

    if (ok == 1)
    {
        if (fscanf(archivo_entrada, "%zu %zu %zu", &filas, &columnas, &minas) != 3)
        {
            ok = 0;
        }
    }

    if (ok == 1)
    {
        tablero = tablero_crear(filas, columnas, minas);
        if (tablero == NULL)
        {
            ok = 0;
        }
    }

    if (ok == 1)
    {
        size_t fila = 0;
        while (fila < filas && ok == 1)
        {
            size_t columna = 0;
            while (columna < columnas && ok == 1)
            {
                int mina = 0;
                int vecinos = 0;
                int estado = 0;

                if (fscanf(archivo_entrada, "%d %d %d", &mina, &vecinos, &estado) != 3)
                {
                    ok = 0;
                }
                else
                {
                    tablero->grilla[fila][columna].es_mina = (mina == 1);
                    tablero->grilla[fila][columna].minas_alrededor = (unsigned char)vecinos;
                    tablero->grilla[fila][columna].estado = (estado_celda_t)estado;
                }
                columna = columna + 1;
            }
            fila = fila + 1;
        }
    }

    if (archivo_entrada != NULL)
    {
        fclose(archivo_entrada);
    }

    if (ok == 0 && tablero != NULL)
    {
        tablero_destruir(tablero);
        tablero = NULL;
    }

    return tablero;
}
