# TP: Carrera de obstáculos

## Repositorio de Sixto Gaspar Nahuel Franco - 108352 - sgfranco@fi.uba.ar

- Para compilar:

```bash
make juego
```

- Para ejecutar:

```bash 
./juego pokemones.txt
```

- Para ejecutar con valgrind:
```bash
make valgrind-alumno
```
---
##  Funcionamiento

### Descripcion general del TDA implementado.

- El TDA implementado en este caso esta diseñado para realizar una carrera entre pokemones, esta misma hace uso de otros tipos de TDA conocidos, en este caso algunos de los implementados son abb y listas. 

- Lo principal del trabajo es darme libertad de seleccionar un pokemon, jugar con el, crear una pista, agregar obstaculos a gusto y competir contra un rival.

### Breve descripcion sobre los structs utilizados en el tp.

```c
typedef struct tdas {
	abb_t *abb;
} TDAS_t;
```
- Esta estructura estaba originalmente pensada para tener más TDAs en uso, pero debido a modificaciones solamente quedó un llamado a un abb.

```c
typedef struct pista {
	lista_t *pista;
	enum TP_OBSTACULO tipo_obstaculo;
} pista_t;
```
- En esta estructura me encargo de crear un puntero a una lista. La pista está implementada en base a una lista de nodos, también hace uso de un enum tipo_obstaculo que contiene el valor de cada obstáculo.

```c
typedef struct info_jugador {
	struct pokemon_info *pokemon;
	bool selecciono_pokemon;
} info_jugador_t;
```
- Esta estructura utiliza información sobre el Pokémon que tendrá un jugador específico, ya sea jugador 1, jugador 2, etc. También utiliza un flag para determinar si el jugador ha seleccionado un Pokémon.


```c
struct tp {
	TDAS_t *tda;
	struct pokemon_info *pokemon;
	unsigned cantidad_pokemones;
	info_jugador_t jugadores[CANTIDAD_JUGADORES];
	pista_t pista_jugador[CANTIDAD_JUGADORES];
};
```
- Esta es la estructura principal que hace uso de todo lo demás en el TP. Tiene un puntero a TDA que maneja los TDAs disponibles, la información del Pokémon (sus atributos y nombre), un vector de jugadores y las pistas para los jugadores.

### Breve descripción de las funciones implementadas de la biblioteca "tp.h"


```c
TP *tp_crear(const char *nombre_archivo);
```

- En esta función me encargo de inicializar y crear la estructura TP. Esta estructura contiene todos los datos necesarios para la implementación del juego.

- En esta función también me encargo de hacer un diagrama de flujo de la función.


![](img/diagrama_flujo_tp_crear.svg)

```c
int tp_cantidad_pokemon(TP *tp);
```

- Aca en esta función me encargo de devolver la cantidad de pokémon cargados en el respectivo TDA elegido.


```c
const struct pokemon_info *tp_buscar_pokemon(TP *tp, const char *nombre);
```
- En esta funcion busco el pokemon por nombre y se encarga de devolver toda la informacion relacionada a ese pokemon

```c
char *tp_nombres_disponibles(TP *tp);
```
- En esta función me muestra los pokémon disponibles. Si algún jugador ya lo eligió, este no estará disponible.

```c
bool tp_seleccionar_pokemon(TP *tp, enum TP_JUGADOR jugador,const char *nombre);
```
- Aca se encarga de verificar que el pokémon no esté en uso por otro jugador. Si está libre, lo asigna al jugador correspondiente.

```c
const struct pokemon_info *tp_pokemon_seleccionado(TP *tp, enum TP_JUGADOR jugador);
```
- Devuelve el pokémon seleccionado por el jugador.


```c
unsigned tp_agregar_obstaculo(TP *tp, enum TP_JUGADOR jugador,enum TP_OBSTACULO obstaculo, unsigned posicion);
```

- En esta función me encargo de asignar un obstáculo (de los que están disponibles) en la pista del respectivo jugador.

```c
unsigned tp_quitar_obstaculo(TP *tp, enum TP_JUGADOR jugador,unsigned posicion);
```

- Elimino el obstáculo de la pista del respectivo jugador. Se elimina en una posición predeterminada.


```c
char *tp_obstaculos_pista(TP *tp, enum TP_JUGADOR jugador);
```

- Me encargo de recorrer la pista de obstáculos y devuelvo un string con los obstáculos que están en la misma.

```c
void tp_limpiar_pista(TP *tp, enum TP_JUGADOR jugador);
```

- Limpia la pista de todos los obstáculos del jugador.

```c
unsigned tp_calcular_tiempo_pista(TP *tp, enum TP_JUGADOR jugador);
```

- Calcula el tiempo que tardó el pokémon elegido por el jugador en recorrer la pista. Se devuelven x unidades de tiempo.

```c
char *tp_tiempo_por_obstaculo(TP *tp, enum TP_JUGADOR jugador);
```

- Genera un string que devuelve el tiempo que tardó en pasar cada obstáculo el pokémon.


```c
void tp_destruir(TP *tp);
```
- En esta función simplemente libero toda la memoria usada por el TDA implementado.


### Breve descripcion de las funciones implementadas de la biblioteca "niveles.h"

- Esta biblioteca se implementó para que la computadora ya tenga armados de manera predeterminada sus niveles.

```c
void generar_cantidad_obstaculos_dependiendo_nivel(
	TP *tp, enum TP_OBSTACULO obstaculo_random, int cantidad_obstaculos);
```

- Esta función representa el nivel del jugador. Esta pista llama a las funciones implementadas en tp.h para armar la pista y dar funcionalidad al nivel. Cada nivel cuenta con distinta cantidad de obstaculos y tambien orden de los obstaculos, estos mismos se generan de forma aleatoria.

- En estos niveles se hacen uso de estas funciones del TDA implementado:

```c
char *tp_nombres_disponibles(TP *tp);
bool tp_seleccionar_pokemon(TP *tp, enum TP_JUGADOR jugador,const char *nombre);
unsigned tp_agregar_obstaculo(TP *tp, enum TP_JUGADOR jugador,enum TP_OBSTACULO obstaculo, unsigned posicion);
char *tp_obstaculos_pista(TP *tp, enum TP_JUGADOR jugador);
unsigned tp_calcular_tiempo_pista(TP *tp, enum TP_JUGADOR jugador);
```

- El nivel facil cuenta con 4 obstaculos.
- El nivel medio cuenta con 6 obstáculos.
- El nivel difícil cuenta con 9 obstáculos.
- El nivel imposible cuenta con 15 obstáculos.

### Breve descripcion de las funciones implementadas de la biblioteca "menu.h"

```c
void bienvenida_al_juego();
```

- Esta función no devuelve nada ni tiene parámetros, ya que solamente es un mensaje de bienvenida que explica las reglas del juego.

```c
int mostrar_opciones_para_jugar();
```

- Esta función muestra las opciones que tiene el jugador. En este caso son 2, ya que selecciona si empezar el juego o simplemente salir del mismo.


```c
int mostrar_menu_de_niveles(TP *tp, unsigned int *tiempo, int *intentos);
```

- En esta función el jugador, después de seleccionar jugar, decide qué nivel jugar. Se muestran las características de cada nivel al seleccionar cuál jugar. Se presenta una forma de salir del juego.


### Breve descripcion de las funciones implementadas de la biblioteca "funcionalidad_jugador_2.h"

```c
void mensaje_jugador_2();
```
- Esta función solamente le indica al jugador un mensaje explicativo, nada más.


```c
void elegir_pokemon(TP* tp);
```
- Esta función le permite al jugador 2 elegir un pokémon para jugar.


```c
void eliminar_obstaculo(TP *tp, int *cantidad_obstaculos);
```

- Esta función me permite eliminar el último obstáculo insertado en caso de equivocarme o querer cambiar.

```c
int agregar_obstaculos(TP *tp, int cantidad_obstaculos);
```
- En esta función, al ya empezar a jugar, le dice al jugador que agregue los obstáculos y también elige la cantidad de obstáculos a implementar en su pista.

```c
unsigned int jugar_juego_con_jugador_2(TP *tp, int* intentos);
```

- Esta función engloba todo lo antes explicado. Aquí se realiza todo el procedimiento donde el jugador elige cada paso de su armado de la pista o selección del pokémon.

---


### Complicaciones realizando el TDA y la logica del juego

- Algunas complicaciones que encontré durante el TP, como siempre, la liberación de toda la memoria en el TDA. También me costó armar los structs. Pensaba en armarlo todo al principio, pero me di cuenta de que era mejor ir armando los structs a medida que avanzaba el programa, ya que iban surgiendo nuevas ideas.

- Otra complicación fue el tema de armar el menú. Me costaba salir de las funciones cuando había algún error. Encontré una función llamada exit que, si se insertaba un valor (como se muestra en pantalla), terminaba la ejecución del programa. Esto me facilito parte del trabajo, pero me di cuenta que no podia usarlo, entonces modifique algunas funciones para que devolvieran un valor, dependiendo el valor que devolvieran se ejecutaria el programa o no.

- Al ir probando el menú, cada error era muy costoso de arreglar.

- Scanf me dio muchos problemas de uso, asi que decidi usar fgets, tambien tenia errores pero se me hizo mas simple. Cuando inserto un dato siempre uso esto, inserto valores numericos por el teclado, se me hace mas facil entenderlos (como usuario y jugador), que andar ingresando caracteres por ejemplo en el caso de Fuerza, Destreza e inteligencia. Eso si use strings en dos partes del codigo, para insertar el nombre del pokemon (me parecio lo mas logico), tambien en la parte de si decidir quitar un obstaculo (si/no).

- En el menu no encontraba la forma de liberar memoria, no entendia donde asignaba memoria dinamica, despues me di cuenta que al llamar funciones que asignaban memoria dinamica del archivo tp.h tambien tenia que liberarla en la implementacion del menu, luego de hacerlo cuando me pongo a jugar al llegar al final del juego ganando o perdiendo segun valgrind no se pierde memoria.

- Otro ajuste fue respecto a los niveles, tenia niveles prearmados (obstaculos e intentos), cambie a que la cantidad de obstaculos se de por un parametro, asi solamente uso una sola funcion, modularizando mucho mas el trabajo.

- Un ajuste que hice respecto a poder usar valgrind fue que pase un archivo directamente por parametro para ver como funcionaba las funciones, despues de verificar todo volvi a hacer que el argumento sea la linea de comandos.