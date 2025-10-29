Pasos a seguir

1- Creamos el tablero y render minimo
Estructuras:
celda (es mina o cantidad de minas alrededor)
estados de celda (oculta, revelada o flag)
tablero

2- Colocar minas

3- Contar vecinos

4- Entrada de datos
Leer la jugada y validar los rangos

5- Revelar celda
Ver que hacer en caso de que sea =0 la celda revelada

6- Flags
Flags para jugador, si esta revelada no se puede

7- Estado del juego
En curso
Ganaste
Perdiste

8- Loop del juego
Mostrar tablero -> leer jugada -> aplicar -> volver a mostrar hasta fin

9- Guardar / Cargar partida
Texto sencillo: dimensiones, minas, estado de cada celda, estado de partida

10- Hints (MODO INTELIGENTE)
Dar pistas/sugerencias

11- Detalles finales
Crear makefile, mejorar interfaz, crear README.md, emprolijar codigo y repositorio


Estructura del proyecto (en principio)

```
buscaminas/
  ├─ src/
  │   ├─ main.c
  │   ├─ tablero.h
  │   ├─ tablero.c
  │   ├─ render.h
  │   ├─ render.c
  │   ├─ entrada.h
  │   └─ entrada.c
  └─ Makefile (VER)
```