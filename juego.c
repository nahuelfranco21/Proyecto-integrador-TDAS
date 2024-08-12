#include <stdio.h>
#include <stdlib.h>
#include "src/tp.h"
#include "src/menu.h"
#include "src/funcionalidades_jugador_2.h"

#define VALOR_CALCULO 100

int main(int argc, char const *argv[])
{
	TP *tp = tp_crear(argv[1]);
	unsigned int tiempo_jugador_1 = 0;
	unsigned int tiempo_jugador_2 = 0;
	int formula_ganadora = 0;
	int intentos = 0;
	bool termino = false;

	int menu_seguir_jugando =
		mostrar_menu_de_niveles(tp, &tiempo_jugador_1, &intentos);
	if (menu_seguir_jugando == 0) {
		tp_destruir(tp);
		return 0;
	}

	printf("Cantidad de intentos disponibles: %i\n", intentos);

	mensaje_jugador_2();
	elegir_pokemon(tp);

	while (intentos != 0 && !termino) {
		tiempo_jugador_2 =
			(unsigned int)jugar_juego_con_jugador_2(tp, &intentos);
		if (tiempo_jugador_2 == 0) {
			printf("Saliendo del juego. Gracias por jugar!\n");
			tp_destruir(tp);
			return 0;
		}
		printf("\nEl tiempo del jugador 1 es: %u, El tiempo del jugador 2 es: %u\n",
		       tiempo_jugador_1, tiempo_jugador_2);
		formula_ganadora =
			VALOR_CALCULO -
			VALOR_CALCULO *
				abs(((int)tiempo_jugador_1 -
				     (int)tiempo_jugador_2)) /
				((int)tiempo_jugador_1 + (int)tiempo_jugador_2);
		printf("\nEl resultado de la formula es: %i\n",
		       formula_ganadora);

		if (formula_ganadora >= VALOR_CALCULO - 5) {
			printf("GANASTE\n");
			termino = true;
		} else {
			printf("PERDISTE\n");
			intentos--;
			if (intentos > 0)
				printf("Te quedan %i intentos, volve a intentar\n",
				       intentos);
			else
				printf("Te quedaste sin intentos, PERDISTE\n");
		}
	}

	tp_destruir(tp);
	return 0;
}