#include <stdio.h>
#include "menu.h"

#define JUGAR 1
#define SALIR 2

#define OPCION_SALIR 0
#define OPCION_FACIL 1
#define OPCION_MEDIO 2
#define OPCION_DIFICIL 3
#define OPCION_IMPOSIBLE 4

void bienvenida_al_juego()
{
	printf("\033[7;33mBienvenido al juego de los pokemon!\033[0m\n");
	printf("\n\n");

	printf("\033[4;32mUna pequeña introducción al juego: En cada nivel vas a tener que seleccionar un pokemon y enfrentarte a distintos obstaculos. Cada pokemon tiene una habilidad distinta, que le va  permitir pasar el obstaculo en determinado tiempo. El objetivo del juego es pasar todos los obstaculos, llegando a tener el mismo tiempo que tu rival\n\033[0m\n");

	printf("A continuación se te va a mostrar el menu de opciones\n");
}

int mostrar_opciones_para_jugar()
{
	int opcion = 0;

	bienvenida_al_juego();

	printf("\033[0;34m1. Jugar\033[0m\n");
	printf("\033[0;34m2. Salir\033[0m\n");

	printf("\033[0;36mIngresa una opcion: \033[0m");

	char elegir_jugar[5];
	if (fgets(elegir_jugar, sizeof(elegir_jugar), stdin))
		opcion = atoi(elegir_jugar);

	while (opcion != JUGAR && opcion != SALIR) {
		printf("\033[0;34mIngrese una opción correcta: \033[0m\n");
		printf("\033[0;34m1. Jugar\033[0m\n");
		printf("\033[0;34m2. Salir\033[0m\n");
		printf("\033[0;36mIngrese una opción: \033[0m");
		if (fgets(elegir_jugar, sizeof(elegir_jugar), stdin))
			opcion = atoi(elegir_jugar);
	}

	if (opcion == JUGAR) {
		return JUGAR;
	} else if (opcion == SALIR) {
		return SALIR;
	} else {
		return 0;
	}

	return 0;
}

int mostrar_menu_de_niveles(TP *tp, unsigned int *tiempo, int *intentos)
{
	int opcion = mostrar_opciones_para_jugar();
	int seleccionar_nivel = -1;
	char elegir_dificultad[5];
	printf("\n");

	if (opcion == JUGAR) {
		printf("\033[0;34mSeleccione un nivel para jugar\033[0m\n");
		printf("\033[0;34m1. FAcil\033[0m\n");
		printf("\033[0;34m2. Medio\033[0m\n");
		printf("\033[0;34m3. Dificil\033[0m\n");
		printf("\033[0;34m4. Imposible\033[0m\n");

		printf("\033[0;36mSeleccione el nivel: \033[0m");
		if (fgets(elegir_dificultad, sizeof(elegir_dificultad), stdin))
			seleccionar_nivel = atoi(elegir_dificultad);

		while (!(seleccionar_nivel >= OPCION_FACIL &&
			 seleccionar_nivel <= OPCION_IMPOSIBLE)) {
			printf("\033[0;36mSeleccione un nivel válido (1 a 4): \033[0m");
			if (fgets(elegir_dificultad, sizeof(elegir_dificultad),
				  stdin))
				seleccionar_nivel = atoi(elegir_dificultad);
		}

		switch (seleccionar_nivel) {
		case OPCION_FACIL:
			armar_nivel_para_el_jugador_1(
				tp, tiempo, CANTIDAD_OBSTACULOS_NIVEL_FACIL,
				intentos);
			*intentos = 6;
			break;
		case OPCION_MEDIO:
			armar_nivel_para_el_jugador_1(
				tp, tiempo, CANTIDAD_OBSTACULOS_NIVEL_MEDIO,
				intentos);
			*intentos = 4;
			break;
		case OPCION_DIFICIL:
			armar_nivel_para_el_jugador_1(
				tp, tiempo, CANTIDAD_OBSTACULOS_NIVEL_DIFICIL,
				intentos);
			*intentos = 3;
			break;
		case OPCION_IMPOSIBLE:
			armar_nivel_para_el_jugador_1(
				tp, tiempo, CANTIDAD_OBSTACULOS_NIVEL_IMPOSIBLE,
				intentos);
			*intentos = 2;
			break;
		default:
			break;
		}
		return 1;
	}
	printf("\033[1;31mSaliendo del juego\033[0m\n");
	return 0;
}
