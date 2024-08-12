#include "niveles.h"
#include "split.h"
#include <stdlib.h>
#include <stdio.h>

void liberar_nombres(char **vector)
{
	int i = 0;
	while (vector[i] != NULL) {
		free(vector[i]);
		i++;
	}

	free(vector);
}

void generar_cantidad_obstaculos_dependiendo_nivel(
	TP *tp, enum TP_OBSTACULO obstaculo_random, int cantidad_obstaculos)
{
	for (unsigned int i = 0; i < cantidad_obstaculos; i++) {
		obstaculo_random = rand() % 3;
		tp_agregar_obstaculo(tp, JUGADOR_1, obstaculo_random, i);
	}
}

/*Este es un nivel prearmado, solo se sabe la cantidad de obstaculos, pero no se sabe cuales son*/
void armar_nivel_para_el_jugador_1(TP *tp, unsigned int *tiempo_para_recorrer,
				   int obstaculos_del_nivel,
				   int *cantidad_intentos)
{
	printf("\033[7;33m\nBIENVENIDO AL NIVEL FACIL DEL JUGADOR 1, ESTA ES LA INFORMACION DEL NIVEL: \033[0m\n");

	srand((unsigned)time(NULL));
	int pokemon_random = rand() % tp_cantidad_pokemon(tp);
	char *nombres = tp_nombres_disponibles(tp);
	char **array_nombres = split(nombres, ',');
	enum TP_OBSTACULO obstaculo_random = 0;
	char *obstaculos_de_la_pista = 0;

	printf("\033[1;33m\nEl pokemon seleccionado es %s\n\033[0m",
	       array_nombres[pokemon_random]);

	tp_seleccionar_pokemon(tp, JUGADOR_1, array_nombres[pokemon_random]);

	generar_cantidad_obstaculos_dependiendo_nivel(tp, obstaculo_random,
						      obstaculos_del_nivel);

	obstaculos_de_la_pista = tp_obstaculos_pista(tp, JUGADOR_1);
	printf("\033[1;31m\nLa pista tiene un largo de %i y tiene %i obstaculos,estos mismos son: \n\033[0m\n",
	       obstaculos_del_nivel, obstaculos_del_nivel);

	for (int i = 0; i < obstaculos_del_nivel; i++) {
		if ((i + 1) % 3 == 0)
			printf("\033[4;32m?\033[0m");
		else
			printf(("\033[4;32m%c\033[0m"),
			       obstaculos_de_la_pista[i]);
	}
	printf("\n\n");
	*tiempo_para_recorrer = tp_calcular_tiempo_pista(tp, JUGADOR_1);
	free(obstaculos_de_la_pista);
	free(nombres);
	liberar_nombres(array_nombres);
}