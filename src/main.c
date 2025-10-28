#include <stdio.h>
#include <stdlib.h>
#include "tablero.h"
#include "render.h"

/**
 * Crea un tablero vacío 9x9 con 10 minas
 * y lo imprime oculto, para validar memoria + render.
 */
int main(void)
{
    int rv = EXIT_SUCCESS;
    tablero_t *tablero = NULL;

    /* Reserva e inicializa la estructura base */
    tablero = tablero_crear(9, 9, 10);
    
    if (tablero == NULL)
    {
        fprintf(stderr, "Error: sin memoria para tablero.\n");
        rv = EXIT_FAILURE;
    }

    /* se colocan minas con semilla fija para debug */
    if (rv == EXIT_SUCCESS)
    {
        int ok_minas = tablero_colocar_minas(tablero, 1234u);
        if (ok_minas == 0)
        {
            fprintf(stderr, "Error: no se pudieron colocar las minas.\n");
            rv = EXIT_FAILURE;
        }
    }

    /* conteo rápido para chequear que la cantidad coincide */
    if (rv == EXIT_SUCCESS)
    {
        size_t fila = 0;
        size_t columna = 0;
        size_t minas_colocadas = 0;

        while (fila < tablero->filas)
        {
            columna = 0;
            while (columna < tablero->columnas)
            {
                if (tablero->grilla[fila][columna].es_mina == true)
                {
                    minas_colocadas = minas_colocadas + 1;
                }
                columna = columna + 1;
            }
            fila = fila + 1;
        }

        fprintf(stdout, "Minas colocadas: %zu\n", minas_colocadas);
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
