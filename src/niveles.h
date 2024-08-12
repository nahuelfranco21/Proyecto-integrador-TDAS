#ifndef _NIVELES_H_
#define _NIVELES_H_
#include "tp.h"
#include "split.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define CANTIDAD_OBSTACULOS_NIVEL_FACIL 4
#define CANTIDAD_OBSTACULOS_NIVEL_MEDIO 6
#define CANTIDAD_OBSTACULOS_NIVEL_DIFICIL 9
#define CANTIDAD_OBSTACULOS_NIVEL_IMPOSIBLE 15

void liberar_nombres(char **vector);

void generar_cantidad_obstaculos_dependiendo_nivel(
	TP *tp, enum TP_OBSTACULO obstaculo_random, int cantidad_obstaculos);

void armar_nivel_para_el_jugador_1(TP *tp, unsigned int *tiempo_para_recorrer,
				   int obstaculos_del_nivel,
				   int *cantidad_intentos);

#endif /*_NIVELES_H_*/