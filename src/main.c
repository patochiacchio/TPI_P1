#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "tablero.h"
#include "render.h"
#include "entrada.h"
#include "archivos.h"

/**
 * Crea un tablero vacío 9x9 con 10 minas
 * y lo imprime oculto, para validar memoria + render.
 */
int main(void)
{
    SetConsoleOutputCP(CP_UTF8);
    int rv = EXIT_SUCCESS;
    tablero_t *tablero = NULL;

    // /* Reserva e inicializa la estructura base */
    // tablero = tablero_crear(9, 9, 10);
    
    /* Intentar cargar partida guardada (opcional) */
    puts("Desea cargar una partida guardada? (s/n)");
    char opcion_cargar = (char)getchar();
    while (getchar() != '\n'); /* limpia buffer */

    if (opcion_cargar == 's' || opcion_cargar == 'S')
    {
        tablero = archivos_cargar_partida("partida_guardada.txt");
        if (tablero != NULL)
        {
            puts("Partida cargada correctamente.");
            render_imprimir(tablero);
        }
        else
        {
            puts("No se pudo cargar la partida. Se creará una nueva.");
        }
    }
    else
    {
        /* Configuración dinámica (filas columnas minas) */
        size_t filas = 0u;
        size_t columnas = 0u;
        size_t minas = 0u;
        int configurado = 0;

        puts("====== CONFIGURAR TABLERO ======");
        puts("Mínimo de filas y columnas: 5");
        puts("Máximo de filas y columnas: 30");
        puts("== CONFIGURACIONES RECOMENDADAS ==");
        puts("Principiante: 8 8 10");
        puts("Intermedio: 16 16 40");
        puts("Experto: 30 16 99");
        puts("Introduzca las dimensiones del tablero (filas columnas minas). Ej: 9 9 10");



        while (configurado == 0)
        {
            int lectura_ok = entrada_leer_configuracion(&filas, &columnas, &minas);
            if (lectura_ok == 1)
            {
                configurado = 1;
            }
            else
            {
                puts("Valores inválidos. Intente de nuevo (filas columnas minas):");
            }
        }

        /* crear tablero con los valores ingresados */
        if (tablero == NULL)
        {
            tablero = tablero_crear(filas, columnas, minas);
        }

        if (tablero == NULL)
        {
            fprintf(stderr, "Error: sin memoria para tablero.\n");
            rv = EXIT_FAILURE;
        }

        /* se colocan minas con semilla VARIABLE para minas aleatorias (CON DEBUG) */
        if (rv == EXIT_SUCCESS)
        {
            unsigned int semilla = (unsigned int)time(NULL) ^ (unsigned int)clock();
            
            // printf("Semilla usada: %u\n", semilla);  // debug
            
            if (tablero_colocar_minas(tablero, semilla) == 0)
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
    }
    
    /* Entrada de datos con validación de jugada */
    if (rv == EXIT_SUCCESS)
    {
        estado_juego_t estado = JUEGO_EN_CURSO;
        int salir = 0;

        while (salir == 0 && estado == JUEGO_EN_CURSO)
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
                    /* Acciones de jugadas (R/B/H/S) */
                    if (accion == ACCION_REVELAR)
                    {
                        int encontro_mina = 0;
                        if (tablero_revelar(tablero, fila, columna, &encontro_mina) == 1)
                        {
                            estado = tablero_estado(tablero, encontro_mina);
                            if (encontro_mina == 1)
                            {
                                puts("Pisaste una mina"); /* mejorar esto */
                            }
                            // /* Prueba estados */

                            // estado_juego_t estado = tablero_estado(tablero, encontro_mina);
                            // if (estado == JUEGO_GANADO)
                            // {
                            //     puts("¡Ganaste!");
                            // }
                            // else if (estado == JUEGO_PERDIDO)
                            // {
                            //     puts("Perdiste");
                            // }
                            render_imprimir(tablero);
                        }
                        else
                        {
                            puts("No se pudo revelar");
                        }
                    }
                    else if (accion == ACCION_BANDERA)
                    {
                        if (tablero_alternar_bandera(tablero, fila, columna) == 1)
                        {
                            render_imprimir(tablero);
                        }
                        else
                        {
                            puts("No se pudo alternar bandera");
                        }
                    }
                    else if (accion == ACCION_HINT)
                    {
                        puts("HINT: sin implementar");
                    }
                    else if (accion == ACCION_SALIR)
                    {
                        puts("Desea guardar la partida? (s/n)");
                        char respuesta = (char)getchar();
                        while (getchar() != '\n'); /* limpia buffer */
                        if (respuesta == 's' || respuesta == 'S')
                        {
                            if (archivos_guardar_partida(tablero, "partida_guardada.txt") == 1)
                            {
                                puts("Partida guardada con éxito.");
                            }
                            else
                            {
                                puts("Error al guardar la partida.");
                            }
                        }
                        salir = 1;
                    }
                }
                else
                {
                    puts("Entrada invalida.");
                }
            }
        }

        /* Mensaje al final/salida del juego */
        if (salir == 1)
        {
            puts("Saliste del juego.");
        }
        /* agregar alguna animacion o algo copado con sleep() */
        else if (estado == JUEGO_GANADO)
        {
            render_imprimir(tablero);
            puts("Ganaste!");
        }
        else if (estado == JUEGO_PERDIDO)
        {
            puts("Perdiste.");
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
