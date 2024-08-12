#ifndef _MENU_H_
#define _MENU_H_
#include "tp.h"
#include "niveles.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void bienvenida_al_juego();

int mostrar_opciones_para_jugar();

int mostrar_menu_de_niveles(TP *tp, unsigned int *tiempo, int *intentos);

#endif /*_MENU_H_*/