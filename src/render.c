#include <stdio.h>   // printf
#include "render.h"

/* Render mínimo: solo muestra el estado visible de cada celda.
 * No hay lógica de juego acá.
 */
void render_imprimir(const tablero_t *tablero)
{
    size_t f = 0;
    size_t c = 0;

    if (tablero == NULL)
    {
        return;
    }

    /* Encabezado de columnas (0..n-1) */
    printf("   ");
    c = 0;
    while (c < tablero->columnas)
    {
        printf("%2zu ", c);
        c = c + 1;
    }
    printf("\n");

    /* Separador */
    printf("   ");
    c = 0;
    while (c < tablero->columnas)
    {
        printf("---");
        c = c + 1;
    }
    printf("\n");

    /* Filas */
    f = 0;
    while (f < tablero->filas)
    {
        printf("%2zu|", f);
        c = 0;
        while (c < tablero->columnas)
        {
            char ch = '.'; /* Oculto */
            if (tablero->grilla[f][c].estado == CELDA_BANDERA)
            {
                ch = 'F';
            }
            else if (tablero->grilla[f][c].estado == CELDA_REVELADA)
            {
                unsigned char n = tablero->grilla[f][c].minas_alrededor;
                ch = (char)('0' + n); /* Revela número */
            }
            printf(" %c ", ch);
            c = c + 1;
        }
        printf("\n");
        f = f + 1;
    }
}
