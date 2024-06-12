# Juego de Serpientes y Escaleras

Este programa simula un juego de mesa de Serpientes y Escaleras para uno o dos jugadores. El juego se puede jugar en modo manual o automático.

## Uso

Para ejecutar el programa, compila y ejecútalo desde la línea de comandos. Puedes correr los siguientes argumentos opcionales al ejecutar el programa:

* **numTiles:** El número de casillas en el tablero (por defecto: 30)
* **numSnakes:** El número de serpientes en el tablero (por defecto: 3)
* **numLadders:** El número de escaleras en el tablero (por defecto: 3)
* **snakePenalty:** El número de casillas que se retrocede al caer en una serpiente (por defecto: 3)
* **ladderReward:** El número de casillas que se avanza al caer en una escalera (por defecto: 3)
* **numPlayers:** El número de jugadores (1 o 2) (por defecto: 2)
* **maxTurns:** El número máximo de turnos permitidos (por defecto: 100)
* **gameType:** El tipo de juego ('M' para manual, 'A' para automático) (por defecto: 'M')

Por ejemplo, para iniciar un juego con 50 casillas, 5 serpientes, 5 escaleras y 2 jugadores sería:

numTiles = 50

numSnakes = 5

numLadders = 5

numPlayers = 2




## Modo de juego

**Modo manual:**

* El juego comienza con todas las fichas de los jugadores en la casilla de inicio (casilla 1).
* Los jugadores tiran un dado virtual por turnos para determinar cuántas casillas avanzar.
* Si una ficha cae en una casilla con la cabeza de una serpiente, la ficha se mueve hacia abajo hasta la cola de la serpiente.
* Si una ficha cae en una casilla con la base de una escalera, la ficha se mueve hacia arriba hasta la parte superior de la escalera.
* El primer jugador en llegar a la última casilla del tablero gana el juego.

**Modo automático:**

* Work-in-progress
## Estructura del código

El código está dividido en varias clases:

* **Tile:** Una clase base abstracta para las casillas del tablero.
* **NormalTile:** Una clase derivada que representa una casilla normal sin ningún efecto especial.
* **SnakeTile:** Una clase derivada que representa una casilla con una serpiente.
* **LadderTile:** Una clase derivada que representa una casilla con una escalera.
* **Game:** Una clase base abstracta para los diferentes modos de juego.
* **ManualGame:** Una clase derivada que implementa el modo de juego manual.
* **AutomaticGame:** Una clase derivada que implementará el modo de juego automático en el futuro.

La función `main()` crea una instancia de la clase `Game` correspondiente según el tipo de juego seleccionado y luego llama al método `play()` para iniciar el juego.
## Notas adicionales

* El programa utiliza la generación de números aleatorios para determinar la posición de las serpientes y escaleras en el tablero.
* El programa no tiene una interfaz gráfica de usuario y se juega en la consola.
* El código se puede ampliar fácilmente para agregar nuevas funciones, como diferentes tipos de casillas o modos de juego.

## Ejemplo de salida

Press C to continue next turn, or E to end the game: C
1 1 1 3 N 4

Press C to continue next turn, or E to end the game: C
2 2 4 2 N 6

Press C to continue next turn, or E to end the game: C
3 1 6 5 S 3

# Nueva v2.2 11/Junio/2024

**Excepciones:**

Se crean las clases InvalidConfigurationException e InvalidOptionException que heredan de std::exception. Estas clases tienen un método what() que devuelve un mensaje de error descriptivo.

**Manejo de excepciones en el constructor Game:**

Se valida que numPlayers y maxTurns sean mayores que 0 en el constructor de la clase Game. Si no lo son, se lanza una excepción InvalidConfigurationException.

**Manejo de excepciones en ManualGame::play:**

Se utiliza un bloque try-catch para capturar la excepción InvalidOptionException si el usuario ingresa una opción inválida.
Se agrega un contador invalidOptionCount para llevar un registro de cuántas veces consecutivas el usuario ingresa una opción inválida.
Si invalidOptionCount alcanza 5, el juego termina y se muestra el mensaje "Invalid menu choice exceeded".

**Manejo de excepciones en main:**

Se utiliza un bloque try-catch para capturar la excepción InvalidConfigurationException al crear el juego. Si se lanza la excepción, se imprime el mensaje de error y el programa termina con un código de salida de 1.

**Liberación de memoria:**

Se asegura de liberar la memoria asignada dinámicamente para el tablero (board) y el objeto del juego (game) al final de la función main.




...

-- GAME OVER --
Player 1 is the winner!!!
## Créditos
-------------------------------- Isaac Martínez Trujillo A01735069 --------------------------------
