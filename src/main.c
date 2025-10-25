#include <stdio.h>
#include <stdlib.h>
#include "tablero.h"
#include "render.h"

/**
 * Crea un tablero vacío 9x9 con 10 minas (todavía no colocadas)
 * y lo imprime oculto, para validar memoria + render.
 */
int main(void)
{
    int rv = EXIT_SUCCESS;
    tablero_t *tablero = NULL;

    /* Reserva e inicializa la estructura base (todo oculto, sin minas) */
    tablero = tablero_crear(9, 9, 10);
    if (tablero == NULL)
    {
        fprintf(stderr, "Error: sin memoria para tablero.\n");
        rv = EXIT_FAILURE;
    }

    /* Render mínimo: índices + celdas ocultas con '.' */
    if (rv == EXIT_SUCCESS)
    {
        puts("========== Buscaminas ==========");
        render_imprimir(tablero);
    }

    /* Liberación de memoria */
    if (tablero != NULL)
    {
        tablero_destruir(tablero);
        tablero = NULL;
    }

    return rv;
}
