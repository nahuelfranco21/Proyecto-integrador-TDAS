#include "funcionalidades_jugador_2.h"

void mensaje_jugador_2()
{
	printf("\n\n");

	printf("\033[4;32mAhora despues de seleccionar la dificultad, es momento de que armes tu pista para competir. Tenes 3 tipos de obstaculos para elegir, estos son: FUERZA, INTELIGENCIA y DESTREZA. Dependiendo el nivel de habilidad del pokemon elegido va a pasar cada obstaculo en un determinado tiempo. Podes elegir la cantidad de obstaculos que quieras, pero acordate la regla principal del juego: Tu tiempo tiene que ser igual al de tu contrincante o lo mas cercano posible para ganar.\n\033[0m\n");
	printf("\n");
}

void elegir_pokemon(TP *tp)
{
	char *pokes_disponibles = tp_nombres_disponibles(tp);

	printf("\033[1;36mLos pokemon que tenes disponibles para seleccionar son: \n%s\n\n\033[0m\n",
	       pokes_disponibles);

	char pokemon_elegido[100];
	printf("\033[0;36mSeleccione el pokemoon que quiera: \033[0m");
	if (fgets(pokemon_elegido, sizeof(pokemon_elegido), stdin))
		pokemon_elegido[strcspn(pokemon_elegido, "\n")] = 0;

	while (!tp_seleccionar_pokemon(tp, JUGADOR_2, pokemon_elegido)) {
		printf("El pokemon que elegiste no esta disponible, por favor elegi otro: ");
		if (fgets(pokemon_elegido, sizeof(pokemon_elegido), stdin))
			pokemon_elegido[strcspn(pokemon_elegido, "\n")] = 0;
	}

	printf("\n");
	printf("\033[1;33m\nEl pokemon que elegiste es: %s\n\033[0m",
	       pokemon_elegido);
	free(pokes_disponibles);
}

void eliminar_obstaculo(TP *tp, int *cantidad_obstaculos)
{
	char confirmar_eliminar[5];

	printf("Estas seguro que queres eliminar el obstaculo? Solo podes eliminar el ultimo (si/no): ");
	if (fgets(confirmar_eliminar, sizeof(confirmar_eliminar), stdin)) {
		confirmar_eliminar[strcspn(confirmar_eliminar, "\n")] = 0;
		if (strcasecmp(confirmar_eliminar, "si") == 0) {
			if (*cantidad_obstaculos > 0) {
				tp_quitar_obstaculo(
					tp, JUGADOR_2,
					(unsigned int)*cantidad_obstaculos - 1);
				(*cantidad_obstaculos)--;
				printf("El último obstaculo fue eliminado de la pista.\n");
			} else
				printf("No hay obstaculos para eliminar en la pista.\n");
		} else
			printf("Se cancelo la eliminacion.\n");
	}
}

int agregar_obstaculos(TP *tp, int cantidad_obstaculos)
{
	tp_limpiar_pista(tp, JUGADOR_2);
	char obstaculo_a_insertar[5];

	for (int i = 0; i < cantidad_obstaculos; i++) {
		int tipo_obstaculo = -1;
		while (!(tipo_obstaculo >= OBSTACULO_FUERZA &&
			 tipo_obstaculo <= OBSTACULO_INTELIGENCIA)) {
			printf("\n\033[0;34mElegi el tipo de obstaculo o elimina el ultimo obstaculo:\n\033[0m");
			printf("\033[0;34m 0. Fuerza\n 1. Destreza\n 2. Inteligencia\n 3. Eliminar ultimo obstáculo\n 4. Salir\n\033[0m");
			printf("Selecciona el número correspondiente al tipo de obstáculo: ");
			if (fgets(obstaculo_a_insertar,
				  sizeof(obstaculo_a_insertar), stdin)) {
				tipo_obstaculo = atoi(obstaculo_a_insertar);
				if (tipo_obstaculo == 3) {
					eliminar_obstaculo(tp, &i);
					tipo_obstaculo = -1;
				} else if (tipo_obstaculo == 4) {
					return 0;
				} else if (!(tipo_obstaculo >=
						     OBSTACULO_FUERZA &&
					     tipo_obstaculo <=
						     OBSTACULO_INTELIGENCIA))
					printf("El tipo de obstáculo seleccionado no es válido. Por favor, elige uno de los números válidos.\n");
			}
		}
		if (tipo_obstaculo != -1)
			tp_agregar_obstaculo(tp, JUGADOR_2, tipo_obstaculo,
					     (unsigned int)i);
		else
			i--;
	}
	char *obstaculos_jugador_2 = tp_obstaculos_pista(tp, JUGADOR_2);
	printf("Los obstáculos que elegiste son: %s\n", obstaculos_jugador_2);
	printf("\n");
	free(obstaculos_jugador_2);
	return 1;
}

unsigned int jugar_juego_con_jugador_2(TP *tp, int *intentos)
{
	int resultado = 0;
	printf("\033[0;35m\nAhora es momento de armar tu pista\n\033[0m");

	printf("\033[0;36mIngresa la cantidad de obstaculos que queres agregar: \033[0m");

	int cantidad_obstaculos = 0;
	char elegir_cantidad_obstaculos[10];
	if (fgets(elegir_cantidad_obstaculos,
		  sizeof(elegir_cantidad_obstaculos), stdin))
		cantidad_obstaculos = atoi(elegir_cantidad_obstaculos);

	while (cantidad_obstaculos <= 0) {
		printf("\033[0;36mLa cantidad de obstaculos tiene que ser mayor a 0. Por favor, ingresa una cantidad válida: \033[0m");
		if (fgets(elegir_cantidad_obstaculos,
			  sizeof(elegir_cantidad_obstaculos), stdin))
			cantidad_obstaculos = atoi(elegir_cantidad_obstaculos);
	}

	resultado = agregar_obstaculos(tp, cantidad_obstaculos);
	if (resultado == 0)
		return 0;

	printf("Tu pista esta armada, ahora es momento de que corras la carrera: \n");

	unsigned int tiempo_carrera = tp_calcular_tiempo_pista(tp, JUGADOR_2);

	return tiempo_carrera;
}