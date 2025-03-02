Lista de tareas y notas importantes sobre el desarrollo de este proyecto.
- Las tareas pendientes se recogen en [tareas](#Tareas).
- Las anotaciones se recogen en [notas](#Notas).
- Las posibles optimizaciones se recogen en [optimizaciones](#Optimizaciones).
- Los bugs a corregir se recogen en [bugs](#Bugs).
## Tareas

| Tarea                  | Entidad                       | Resumen                                                                                           |
| ---------------------- | ----------------------------- | ------------------------------------------------------------------------------------------------- |
| Jugabilidad y objetivo |                               | Pensar sobre el gameplay y el objetivo del juego [\[✒️\]](#notas#gameplay)                        |
| Controles              |                               | Librería o utilidad que permita definir acciones y la tecla asociada [\[✒️\]](#notas#inputs)      |
| Cámara                 | Player                        | Creación de la cámara, de forma que el espacio visible no dependa del tamaño de la pantalla       |
| Enemigos               | Enemy                         | Creación de la entidad enemigo, funciones necesarias, acciones, movimiento...                     |
| Habilidades            | Ability<br>Player<br>Enemy    | Inventar nuevas habilidades para los jugadores y enemigos [\[✒️\]](#notas#posibles%20habilidades) |
| Colisiones             | Player<br>Enemy<br>Projectile | Comprobación de colisiones entre jugadores, enemigos y proyectiles                                |
| IA de enemigos         | Enemy                         | Creación de un algoritmo de IA para activar el movimiento y acciones de los enemigos              |
>\[✒️\] - Enlace a la nota donde se añade contexto.

---
## Notas
### Gameplay

- El concepto del juego es el espacio exterior. Navegas en una nave que tiene distintas habilidades que pueden ser usadas a gusto del jugador.

- La exploración espacial puede ser uno de los puntos clave del juego. Viajar por el espacio exterior encontrando asteroides/meteoritos (¿materiales?), estaciones espaciales (¿comercio?), otras naves (¿combate?) e incluso agujeros negros/agujeros de gusano (¿mazmorras?).

-  Perspectiva 2D, ejes X e Y

- Mapamundi:
	- En una esquina de la pantalla
	- Mostrarlo en grande a petición del jugador

- Rejugabilidad (¿varias fichas de partida guardada?) mediante la mejora de la nave y alcanzar ciertos objetivos (¿qué objetivos?). Se consigue a partir de:
	- O bien: A partir de un sistema de runs.
	- O bien: No hay runs, puedes crear varios mundos, cada mundo es procedural (¿posibilidad de conectar mundos?)

- Cooperativo local:
	- 1P - Pantalla completa
	- 2P - Pantalla dividida vertical/horizontal
	- 3P - Pantalla dividida en modo Y
	- 4P - Pantalla dividida en cuatro sectores
	- Posibilidad de crear múltiples ventanas en la que cada una se muestre un jugador (ver [Windowkill](https://torcado.itch.io/windowkill)) (ver [discusión sobre múltiples ventanas en raylib](https://github.com/raysan5/raylib/issues/1989#issuecomment-2291474771))

- Los agujeros negros pueden conectar distintas partes del mapa:
	- El interior es una mazmorra generada de forma procedural con tantas salidas como conexiones con el exterior tenga

- El mapa jugable:
	- O bien: Un cuadrado muy grande de un tamaño preestablecido con recursos que se van regenerando
	- O bien: Un mundo procedural que se va ampliando conforme el jugador va explorando

- Personalización de la nave:
	- Colores
	- Sprite
	- Adornos

### Posibles habilidades

| Habilidad | Descripción                                                                                              |
| --------- | -------------------------------------------------------------------------------------------------------- |
| Cohete    | Lanza un cohete, más potente que un disparo normal, pero con más tiempo de recarga / consume más energía |
### Inputs
La librería de inputs debería ser la encargada de realizar la comprobación de acciones a raíz de las teclas presionadas.

- Los inputs pueden ser de varios tipos:
	- Teclado y ratón:
		- Pulsación de teclas del teclado.
		- Pulsación de botones del ratón
		- Movimiento del ratón (ejes: +X, -X, +Y, -Y)
	- Mando
		- Botón de mando
		- Gatillo de mando, si tiene (¿olvidar esto debido a limitaciones?)
		- Joystick de mando (ejes: +X, -X, +Y, -Y)

- A partir de un input especifico, tener una función intermedia que obtenga el peso a utilizar en la modificación de la variable asociada \[0..1\]:
	- En el caso de una **tecla del teclado**, **botón del ratón** o **botón de mando**, la función devolverá los valores 0 o 1
	- En el caso de **movimiento del ratón**, la función devolverá valores entre 0 y 1 dependiendo de la posición del ratón con respecto al jugador con respecto al eje de referencia
	- En el caso de **gatillo de mando**, la función devolverá valores entre 0 y 1 dependiendo de la cantidad de recorrido que haya hecho el gatillo
	- En el caso de **joystick de mando**, la función devolverá valores entre 0 y 1 dependiendo de la distancia a la que se encuentre el joystick del centro con respecto al eje de referencia


---
## Bugs

| Bug | Localización | Resumen |
| --- | ------------ | ------- |
|     |              |         |
|     |              |         |

---
## Optimizaciones

| Tarea                         | Resumen                                                                                                       |
| ----------------------------- | ------------------------------------------------------------------------------------------------------------- |
| Reorganización de estructuras | Reorganización de los miembros de las estructuras para minimizar el uso de memoria y maximizar el rendimiento |

