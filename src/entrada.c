#include <stdio.h>
#include <ctype.h> /* para usar isspace y toupper */
#include <stdlib.h>
#include "entrada.h"

/* Parseo en una sola pasada; devuelve 1 si OK, 0 si inválido. */
int entrada_leer_jugada(accion_t *accion, size_t *fila, size_t *columna, size_t max_filas, size_t max_columnas)
{
    int ok = 1;
    char linea[128];
    char a = '\0';
    char *p = NULL;
    char *endptr = NULL;
    long fila_tmp = 0;
    long columna_tmp = 0;

    if (accion == NULL || fila == NULL || columna == NULL)
    {
        ok = 0;
    }

    if (ok == 1)
    {
        /* pido una línea */
        if (fgets(linea, (int)sizeof(linea), stdin) == NULL)
        {
            ok = 0;
        }
    }

    if (ok == 1)
    {
        /* salto espacios iniciales y tomo la acción (MAYUS) */
        p = linea;
        while (*p != '\0' && isspace((unsigned char)*p) != 0)
        {
            p = p + 1;
        }
        if (*p == '\0')
        {
            ok = 0;
        }
        else
        {
            a = (char)toupper((unsigned char)*p); /* tambien era opcion cambiar las acciones a minuscula pero seguimos la regla de constantes = SNAKE_CASE*/
            if (a == 'R')
            {
                *accion = ACCION_REVELAR;
            }
            else if (a == 'B')
            {
                *accion = ACCION_BANDERA;
            }
            else if (a == 'H')
            {
                *accion = ACCION_HINT;
            }
            else if (a == 'S')
            {
                *accion = ACCION_SALIR;
            }
            else
            {
                ok = 0;
            }
            p = p + 1;
        }
    }

    /* fila */
    if (ok == 1)
    {
        while (*p != '\0' && isspace((unsigned char)*p) != 0)
        {
            p = p + 1;
        }
        if (*p == '\0')
        {
            ok = 0;
        }
        else
        {
            fila_tmp = strtol(p, &endptr, 10);
            if (endptr == p || fila_tmp < 0 || (long)max_filas <= fila_tmp)
            {
                ok = 0;
            }
            else
            {
                p = endptr;
            }
        }
    }

    /* columna */
    if (ok == 1)
    {
        while (*p != '\0' && isspace((unsigned char)*p) != 0)
        {
            p = p + 1;
        }
        if (*p == '\0')
        {
            ok = 0;
        }
        else
        {
            columna_tmp = strtol(p, &endptr, 10);
            if (endptr == p || columna_tmp < 0 || (long)max_columnas <= columna_tmp)
            {
                ok = 0;
            }
            else
            {
                p = endptr;
            }
        }
    }

    /* si todo OK, asigno los out-params */
    if (ok == 1)
    {
        *fila = (size_t)fila_tmp;
        *columna = (size_t)columna_tmp;
    }

    return ok;
}
