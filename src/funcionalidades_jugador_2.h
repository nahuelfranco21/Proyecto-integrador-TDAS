#ifndef _FUNCIONALIDAD_JUGADOR_2_H_
#define _FUNCIONALIDAD_JUGADOR_2_H_

#include "tp.h"
#include "split.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

void mensaje_jugador_2();

void elegir_pokemon(TP *tp);

void eliminar_obstaculo(TP *tp, int *cantidad_obstaculos);

int agregar_obstaculos(TP *tp, int cantidad_obstaculos);

unsigned int jugar_juego_con_jugador_2(TP *tp, int *intentos);

#endif /*_FUNCIONALIDAD_JUGADOR_2_H_*/