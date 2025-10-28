#include <stdio.h>
#include <stdlib.h>
#include "tablero.h"
#include "render.h"
#include "entrada.h"

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

    // /* conteo rápido para chequear que la cantidad coincide */
    // if (rv == EXIT_SUCCESS)
    // {
    //     size_t fila = 0;
    //     size_t columna = 0;
    //     size_t minas_colocadas = 0;

    //     while (fila < tablero->filas)
    //     {
    //         columna = 0;
    //         while (columna < tablero->columnas)
    //         {
    //             if (tablero->grilla[fila][columna].es_mina == true)
    //             {
    //                 minas_colocadas = minas_colocadas + 1;
    //             }
    //             columna = columna + 1;
    //         }
    //         fila = fila + 1;
    //     }

    //     fprintf(stdout, "Minas colocadas: %zu\n", minas_colocadas);
    // }

    /* se cuentan los vecinos (0..8) en celdas no-mina */
    if (rv == EXIT_SUCCESS)
    {
        int ok_vecinos = tablero_contar_vecinos(tablero);
        if (ok_vecinos == 0)
        {
            fprintf(stderr, "Error: no se pudieron contar los vecinos.\n");
            rv = EXIT_FAILURE;
        }
    }

    // /* TEST para revelar hasta 3 celdas no-mina para ver números (temporal) */
    // if (rv == EXIT_SUCCESS)
    // {
    //     size_t fila = 0;
    //     size_t columna = 0;
    //     size_t mostradas = 0;

    //     while (fila < tablero->filas && mostradas < 3)
    //     {
    //         columna = 0;
    //         while (columna < tablero->columnas && mostradas < 3)
    //         {
    //             if (tablero->grilla[fila][columna].es_mina == false)
    //             {
    //                 tablero->grilla[fila][columna].estado = CELDA_REVELADA;
    //                 mostradas = mostradas + 1;
    //             }
    //             columna = columna + 1;
    //         }
    //         fila = fila + 1;
    //     }
    // }

    /* Render mínimo: índices + celdas ocultas con '.' */
    if (rv == EXIT_SUCCESS)
    {
        puts("========== Buscaminas ==========");
        render_imprimir(tablero);
    }

    /* Entrada de datos con validación de jugada */
    if (rv == EXIT_SUCCESS)
    {
        puts("Ingrese jugada (R/B/H/S) fila columna, ej: R 3 4");
        {
            accion_t accion = ACCION_REVELAR;
            size_t fila = 0;
            size_t columna = 0;
            int ok_entrada = 0;

            ok_entrada = entrada_leer_jugada(&accion, &fila, &columna, tablero->filas, tablero->columnas);

            if (ok_entrada == 1)
            {
                const char *accion_txt = "REVELAR";
                if (accion == ACCION_BANDERA) { accion_txt = "BANDERA"; }
                else if (accion == ACCION_HINT) { accion_txt = "HINT"; }
                else if (accion == ACCION_SALIR) { accion_txt = "SALIR"; }

                printf("OK: %s (%zu, %zu)\n", accion_txt, fila, columna);
            }
            else
            {
                printf("Entrada inválida.\n");
            }
        }
    }

    /* Liberación de memoria */
    if (tablero != NULL)
    {
        tablero_destruir(tablero);
        tablero = NULL;
    }

    return rv;
}
