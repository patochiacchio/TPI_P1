#include <stdio.h>
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
            if (tablero->grilla[f][c].estado == CELDA_BANDERA)
            {
                /* Imprimimos el emoji "Bandera" como un STRING (%s) */
                printf(" %s", "🚩");
            }
            else if (tablero->grilla[f][c].estado == CELDA_REVELADA)
            {
                /* Calculamos el número */
                unsigned char n = tablero->grilla[f][c].minas_alrededor;
                char ch_num = (char)('0' + n);
                printf(" %c ", ch_num);
            }
            else /* Caso por defecto: CELDA_OCULTA */
            {
                printf(" %s ", "☺"); 
            }            
            c = c + 1;
        }
        printf("\n");
        f = f + 1;
    }
}

/* Render final para partida perdida (ganada ya revelada )*/
void render_imprimir_final(const tablero_t *tablero)
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
            /* Lógica de renderizado final */
            if (tablero->grilla[f][c].es_mina == true)
            {
                if (tablero->grilla[f][c].estado == CELDA_BANDERA)
                {
                    printf(" %s", "🚩"); /* Bandera correcta */
                }
                else
                {
                    printf(" %s", "💣"); /* Mina (no descubierta) */
                }
            }
            else if (tablero->grilla[f][c].estado == CELDA_BANDERA)
            {
                printf(" %s", "❌"); /* Bandera incorrecta */
            }
            else /* Celda normal (revelada o no) */
            {
                unsigned char n = tablero->grilla[f][c].minas_alrededor;
                char ch_num = (char)('0' + n);
                printf(" %c ", ch_num);
            }
            
            c = c + 1;
        }
        printf("\n");
        f = f + 1;
    }
}

/* Render partida terminada, ganada o perdida */
void render_partida_terminada(const tablero_t *tablero, estado_juego_t estado)
{
    if (estado == JUEGO_GANADO)
    {
        /* Si se gana, el tablero ya está revelado por la lógica del juego */
        render_imprimir(tablero);
        
        puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀");
        puts("⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⠀⠀⠀⣶⡆⠀⣰⣿⠇⣾⡿⠛⠉⠁");
        puts("⠀⣠⣴⠾⠿⠿⠀⢀⣾⣿⣆⣀⣸⣿⣷⣾⣿⡿⢸⣿⠟⢓⠀⠀");
        puts("⣴⡟⠁⣀⣠⣤⠀⣼⣿⠾⣿⣻⣿⠃⠙⢫⣿⠃⣿⡿⠟⠛⠁⠀");
        puts("⢿⣝⣻⣿⡿⠋⠾⠟⠁⠀⠹⠟⠛⠀⠀⠈⠉⠀⠉⠀⠀⠀⠀⠀");
        puts("⠀⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⣀⢀⣠⣤⣴⣤⣄⠀");
        puts("⠀⠀⠀⠀⣀⣤⣤⢶⣤⠀⠀⢀⣴⢃⣿⠟⠋⢹⣿⣣⣴⡿⠋⠀");
        puts("⠀⠀⣰⣾⠟⠉⣿⡜⣿⡆⣴⡿⠁⣼⡿⠛⢃⣾⡿⠋⢻⣇⠀⠀");
        puts("⠀⠐⣿⡁⢀⣠⣿⡇⢹⣿⡿⠁⢠⣿⠷⠟⠻⠟⠀⠀⠈⠛⠀⠀");
        puts("⠀⠀⠙⠻⠿⠟⠋⠀⠀⠙⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
        puts("               ¡GANASTE!           ");
        puts("====================================");

        
    }
    else if (estado == JUEGO_PERDIDO)
    {
        /* Si se pierde, usamos el render final que revela todo */
        render_imprimir_final(tablero);
    
        puts("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀");
        puts("⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⠀⠀⠀⣶⡆⠀⣰⣿⠇⣾⡿⠛⠉⠁");
        puts("⠀⣠⣴⠾⠿⠿⠀⢀⣾⣿⣆⣀⣸⣿⣷⣾⣿⡿⢸⣿⠟⢓⠀⠀");
        puts("⣴⡟⠁⣀⣠⣤⠀⣼⣿⠾⣿⣻⣿⠃⠙⢫⣿⠃⣿⡿⠟⠛⠁⠀");
        puts("⢿⣝⣻⣿⡿⠋⠾⠟⠁⠀⠹⠟⠛⠀⠀⠈⠉⠀⠉⠀⠀⠀⠀⠀");
        puts("⠀⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⣀⢀⣠⣤⣴⣤⣄⠀");
        puts("⠀⠀⠀⠀⣀⣤⣤⢶⣤⠀⠀⢀⣴⢃⣿⠟⠋⢹⣿⣣⣴⡿⠋⠀");
        puts("⠀⠀⣰⣾⠟⠉⣿⡜⣿⡆⣴⡿⠁⣼⡿⠛⢃⣾⡿⠋⢻⣇⠀⠀");
        puts("⠀⠐⣿⡁⢀⣠⣿⡇⢹⣿⡿⠁⢠⣿⠷⠟⠻⠟⠀⠀⠈⠛⠀⠀");
        puts("⠀⠀⠙⠻⠿⠟⠋⠀⠀⠙⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
        puts("               ¡PERDISTE!           ");
        puts("====================================");

    }
}