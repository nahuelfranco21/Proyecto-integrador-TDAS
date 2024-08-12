#include "tp.h"
#include "lista.h"
#include "abb.h"
#include "split.h"
#include <string.h>
#include <strings.h>
#include <stdio.h>

#define CANTIDAD_JUGADORES 2
#define VALOR_FORMULA 10

typedef struct tdas {
	abb_t *abb;
} TDAS_t;

typedef struct pista {
	lista_t *pista;
	enum TP_OBSTACULO tipo_obstaculo;
} pista_t;

typedef struct info_jugador {
	struct pokemon_info *pokemon;
	bool selecciono_pokemon;
} info_jugador_t;

struct tp {
	TDAS_t *tda;
	struct pokemon_info *pokemon;
	unsigned cantidad_pokemones;
	info_jugador_t jugadores[CANTIDAD_JUGADORES];
	pista_t pista_jugador[CANTIDAD_JUGADORES];
};

void liberar_obstaculos(void *obstaculo)
{
	free(obstaculo);
}

size_t contar_pokemones(TP *tp, const char *nombre_archivo)
{
	FILE *archivo = fopen(nombre_archivo, "r");

	if (!archivo)
		return 0;

	char linea[50];

	while (fgets(linea, sizeof(linea), archivo))
		tp->cantidad_pokemones++;

	fclose(archivo);
	return (size_t)tp->cantidad_pokemones;
}

TP *inicializar_struct(TP *tp, const char *nombre_archivo)
{
	if (!tp)
		return NULL;

	size_t cantidad = contar_pokemones(tp, nombre_archivo);

	tp->pokemon = calloc(cantidad + 1, sizeof(struct pokemon_info));

	if (!tp->pokemon) {
		free(tp);
		return NULL;
	}

	return tp;
}

bool procesar_lineas(TP *tp, char *linea, int indice)
{
	char **vector = split(linea, ',');

	if (vector && vector[0] && vector[1] && vector[2] && vector[3] &&
	    !vector[4]) {
		tp->pokemon[indice].nombre = vector[0];
		tp->pokemon[indice].fuerza = atoi(vector[1]);
		tp->pokemon[indice].destreza = atoi(vector[2]);
		tp->pokemon[indice].inteligencia = atoi(vector[3]);
		free(vector[1]);
		free(vector[2]);
		free(vector[3]);
		free(vector);
	} else {
		liberar_vector(vector, 4);
		return false;
	}
	return true;
}

TP *guardar_pokemones_en_struct(TP *tp, const char *nombre_archivo)
{
	FILE *archivo = fopen(nombre_archivo, "r");

	if (!archivo)
		return NULL;

	char linea[50];
	int iterador = 0;
	size_t longitud = 0;

	while (fgets(linea, sizeof(linea), archivo)) {
		longitud = strlen(linea);
		if (longitud > 0 && linea[longitud - 1] == '\n')
			linea[longitud - 1] = 0;

		if (!procesar_lineas(tp, linea, iterador)) {
			fclose(archivo);
			return NULL;
		}
		iterador++;
	}
	fclose(archivo);
	return tp;
}

int comparador_de_pokemones(void *a, void *b)
{
	struct pokemon_info *pokemon_1 = (struct pokemon_info *)a;
	struct pokemon_info *pokemon_2 = (struct pokemon_info *)b;

	return strcasecmp(pokemon_1->nombre, pokemon_2->nombre);
}

TP *insertar_pokemones_en_abb(TP *tp)
{
	tp->tda->abb = abb_crear(comparador_de_pokemones);

	if (!tp->tda->abb)
		return NULL;

	for (size_t i = 0; i < tp->cantidad_pokemones; i++)
		tp->tda->abb = abb_insertar(tp->tda->abb, &(tp->pokemon[i]));

	return tp;
}

TP *inicializar_pistas_para_los_jugadores(TP *tp)
{
	if (!tp)
		return NULL;

	tp->pista_jugador[JUGADOR_1].pista = lista_crear();
	tp->pista_jugador[JUGADOR_2].pista = lista_crear();

	if (!tp->pista_jugador[JUGADOR_1].pista ||
	    !tp->pista_jugador[JUGADOR_2].pista) {
		return NULL;
	}
	return tp;
}

TP *tp_crear(const char *nombre_archivo)
{
	if (!nombre_archivo)
		return NULL;

	TP *tp = calloc(1, sizeof(TP));

	if (!tp)
		return NULL;

	tp->tda = calloc(1, sizeof(TDAS_t));

	if (!tp->tda) {
		free(tp);
		return NULL;
	}

	if (!inicializar_struct(tp, nombre_archivo)) {
		free(tp->tda);
		free(tp);
		return NULL;
	}

	if (!guardar_pokemones_en_struct(tp, nombre_archivo)) {
		free(tp->pokemon);
		free(tp->tda);
		free(tp);
		return NULL;
	}

	if (!insertar_pokemones_en_abb(tp)) {
		free(tp->tda->abb);
		free(tp->tda);
		free(tp->pokemon);
		free(tp);
		return NULL;
	}

	if (!inicializar_pistas_para_los_jugadores(tp)) {
		free(tp->tda);
		free(tp->pokemon);
		free(tp);
		return NULL;
	}

	return tp;
}

int tp_cantidad_pokemon(TP *tp)
{
	if (!tp || !tp->tda || !tp->tda->abb)
		return 0;

	return (int)abb_tamanio(tp->tda->abb);
}

const struct pokemon_info *tp_buscar_pokemon(TP *tp, const char *nombre)
{
	if (!tp || !nombre)
		return NULL;

	struct pokemon_info pokemon_buscado;
	pokemon_buscado.nombre = (char *)nombre;

	void *pokemon = abb_buscar(tp->tda->abb, &pokemon_buscado);

	if (!pokemon)
		return NULL;

	return (const struct pokemon_info *)pokemon;
}

char *tp_nombres_disponibles(TP *tp)
{
	if (!tp)
		return NULL;

	size_t tamanio_abb = abb_tamanio(tp->tda->abb);
	void **array_nombres =
		calloc(abb_tamanio(tp->tda->abb), sizeof(void *));
	size_t cant_nombres_guardados = 0;
	struct pokemon_info *pokemon = NULL;
	size_t longitud_nombres = 0;
	char *string_ordenado = calloc(abb_tamanio(tp->tda->abb), sizeof(char));

	cant_nombres_guardados =
		abb_recorrer(tp->tda->abb, INORDEN, array_nombres, tamanio_abb);

	for (size_t i = 0; i < cant_nombres_guardados; i++) {
		pokemon = (struct pokemon_info *)array_nombres[i];
		if (tp->jugadores[JUGADOR_1].pokemon != pokemon &&
		    tp->jugadores[JUGADOR_2].pokemon != pokemon)
			longitud_nombres += strlen(pokemon->nombre) + 1;
	}

	string_ordenado = calloc(longitud_nombres + 1, sizeof(char));

	for (size_t i = 0; i < cant_nombres_guardados; i++) {
		pokemon = (struct pokemon_info *)array_nombres[i];
		if (tp->jugadores[JUGADOR_1].pokemon != pokemon &&
		    tp->jugadores[JUGADOR_2].pokemon != pokemon) {
			strcat(string_ordenado, pokemon->nombre);
			strcat(string_ordenado, ",");
		}
	}

	if (strlen(string_ordenado) > 0)
		string_ordenado[strlen(string_ordenado) - 1] = 0;

	free(array_nombres);

	return string_ordenado;
}

bool tp_seleccionar_pokemon(TP *tp, enum TP_JUGADOR jugador, const char *nombre)
{
	if (!nombre || !tp)
		return false;

	struct pokemon_info *pokemon_buscado =
		(struct pokemon_info *)tp_buscar_pokemon(tp, nombre);

	if (!pokemon_buscado)
		return false;

	if ((jugador == JUGADOR_1 &&
	     tp->jugadores[JUGADOR_2].pokemon == pokemon_buscado) ||
	    (jugador == JUGADOR_2 &&
	     tp->jugadores[JUGADOR_1].pokemon == pokemon_buscado))
		return false;

	tp->jugadores[jugador].pokemon = pokemon_buscado;
	tp->jugadores[jugador].selecciono_pokemon = true;
	return true;
}

const struct pokemon_info *tp_pokemon_seleccionado(TP *tp,
						   enum TP_JUGADOR jugador)
{
	if (!tp || jugador >= CANTIDAD_JUGADORES)
		return NULL;

	return tp->jugadores[jugador].pokemon;
}

unsigned tp_agregar_obstaculo(TP *tp, enum TP_JUGADOR jugador,
			      enum TP_OBSTACULO obstaculo, unsigned posicion)
{
	if (!tp || jugador >= CANTIDAD_JUGADORES)
		return 0;

	enum TP_OBSTACULO *obstaculo_a_insertar =
		calloc(1,sizeof(enum TP_OBSTACULO));

	if (!obstaculo_a_insertar)
		return 0;

	*obstaculo_a_insertar = obstaculo;

	lista_insertar_en_posicion(tp->pista_jugador[jugador].pista,
				   obstaculo_a_insertar, posicion);

	return (unsigned)lista_tamanio(tp->pista_jugador[jugador].pista);
}

unsigned tp_quitar_obstaculo(TP *tp, enum TP_JUGADOR jugador, unsigned posicion)
{
	if (!tp || jugador >= CANTIDAD_JUGADORES)
		return 0;

	void *obstaculo_quitado = lista_quitar_de_posicion(
		tp->pista_jugador[jugador].pista, posicion);
	free(obstaculo_quitado);

	return (unsigned)lista_tamanio(tp->pista_jugador[jugador].pista);
}

void recorrer_pista_y_armar_string(TP *tp, char *str, enum TP_JUGADOR jugador)
{
	lista_iterador_t *iterador =
		lista_iterador_crear(tp->pista_jugador[jugador].pista);

	if (!iterador) {
		return;
	}

	enum TP_OBSTACULO tipo_obstaculo = 0;
	size_t i = 0;

	while (lista_iterador_tiene_siguiente(iterador)) {
		tipo_obstaculo =
			*(enum TP_OBSTACULO *)lista_iterador_elemento_actual(
				iterador);

		if (tipo_obstaculo == OBSTACULO_FUERZA)
			str[i] = IDENTIFICADOR_OBSTACULO_FUERZA;
		else if (tipo_obstaculo == OBSTACULO_DESTREZA)
			str[i] = IDENTIFICADOR_OBSTACULO_DESTREZA;
		else if (tipo_obstaculo == OBSTACULO_INTELIGENCIA)
			str[i] = IDENTIFICADOR_OBSTACULO_INTELIGENCIA;
		else
			str[i] = '.';

		i++;
		lista_iterador_avanzar(iterador);
	}

	str[i] = 0;
	lista_iterador_destruir(iterador);
}

char *tp_obstaculos_pista(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp || jugador >= CANTIDAD_JUGADORES ||
	    !lista_tamanio(tp->pista_jugador[jugador].pista))
		return NULL;

	char *string_obstaculos =
		calloc(lista_tamanio(tp->pista_jugador[jugador].pista) + 1,
		       sizeof(char));

	recorrer_pista_y_armar_string(tp, string_obstaculos, jugador);

	return string_obstaculos;
}

void tp_limpiar_pista(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp || jugador >= CANTIDAD_JUGADORES)
		return;

	lista_destruir_todo(tp->pista_jugador[jugador].pista,
			    liberar_obstaculos);
	tp->pista_jugador[jugador].pista = lista_crear();
}

void recorrer_pista_y_calcular_lo_pedido(TP *tp, int *tiempo,
					 enum TP_JUGADOR jugador, char *string)
{
	if (!tp || jugador >= CANTIDAD_JUGADORES ||
	    !tp->pista_jugador[jugador].pista)
		return;

	lista_iterador_t *iterador =
		lista_iterador_crear(tp->pista_jugador[jugador].pista);

	if (!iterador)
		return;

	enum TP_OBSTACULO tipo_obstaculo_actual;
	enum TP_OBSTACULO tipo_obstaculo_anterior = -1;
	int tiempo_obstaculo_actual = 0;
	int obstaculos_repetidos = 0;
	int posicion_actual_str = 0;
	int tiempo_individual = 0;

	while (lista_iterador_tiene_siguiente(iterador)) {
		tipo_obstaculo_actual =
			*(enum TP_OBSTACULO *)lista_iterador_elemento_actual(
				iterador);
		tiempo_obstaculo_actual = 0;

		if (tipo_obstaculo_actual == OBSTACULO_FUERZA)
			tiempo_obstaculo_actual =
				tp->jugadores[jugador].pokemon->fuerza;
		else if (tipo_obstaculo_actual == OBSTACULO_DESTREZA)
			tiempo_obstaculo_actual =
				tp->jugadores[jugador].pokemon->destreza;
		else if (tipo_obstaculo_actual == OBSTACULO_INTELIGENCIA)
			tiempo_obstaculo_actual =
				tp->jugadores[jugador].pokemon->inteligencia;
		else
			tiempo_obstaculo_actual = 0;

		if (tiempo) {
			if (tipo_obstaculo_actual == tipo_obstaculo_anterior) {
				obstaculos_repetidos++;
				int tiempo_calculado = VALOR_FORMULA -
						       obstaculos_repetidos -
						       tiempo_obstaculo_actual;
				if (tiempo_calculado > 0)
					*tiempo += tiempo_calculado;
			} else {
				obstaculos_repetidos = 0;
				int tiempo_calculado =
					VALOR_FORMULA - tiempo_obstaculo_actual;
				if (tiempo_calculado > 0)
					*tiempo += tiempo_calculado;
			}
		}

		if (string) {
			if (tipo_obstaculo_actual == tipo_obstaculo_anterior) {
				obstaculos_repetidos++;
				int tiempo_calculado = VALOR_FORMULA -
						       obstaculos_repetidos -
						       tiempo_obstaculo_actual;
				if (tiempo_calculado > 0)
					tiempo_individual = tiempo_calculado;
				else
					tiempo_individual = 0;
			} else {
				obstaculos_repetidos = 0;
				int tiempo_calculado =
					VALOR_FORMULA - tiempo_obstaculo_actual;
				if (tiempo_calculado > 0)
					tiempo_individual = tiempo_calculado;
				else
					tiempo_individual = 0;
			}
			posicion_actual_str +=
				sprintf(string + posicion_actual_str, "%d,",
					tiempo_individual);
		}

		tipo_obstaculo_anterior = tipo_obstaculo_actual;
		lista_iterador_avanzar(iterador);
	}

	if (string && strlen(string) > 0)
		string[strlen(string) - 1] = 0;

	lista_iterador_destruir(iterador);
}

unsigned tp_calcular_tiempo_pista(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp || jugador >= CANTIDAD_JUGADORES ||
	    !tp->jugadores[jugador].pokemon ||
	    !lista_tamanio(tp->pista_jugador[jugador].pista))
		return 0;

	int unidades_de_tiempo = 0;

	recorrer_pista_y_calcular_lo_pedido(tp, &unidades_de_tiempo, jugador,
					    NULL);

	return (unsigned)unidades_de_tiempo;
}

char *tp_tiempo_por_obstaculo(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp || jugador >= CANTIDAD_JUGADORES ||
	    !tp->jugadores[jugador].pokemon ||
	    !lista_tamanio(tp->pista_jugador[jugador].pista))
		return NULL;

	char *string_csv = calloc(1024 ,sizeof(char));

	if (!string_csv)
		return NULL;

	string_csv[0] = 0;

	recorrer_pista_y_calcular_lo_pedido(tp, NULL, jugador, string_csv);

	return string_csv;
}

void tp_destruir(TP *tp)
{
	if (!tp)
		return;

	char *obstaculo = tp_obstaculos_pista(tp, JUGADOR_1);

	if (obstaculo)
		free(obstaculo);

	if (tp->pokemon) {
		for (int i = 0; i < tp->cantidad_pokemones; i++)
			free(tp->pokemon[i].nombre);
		free(tp->pokemon);
	}

	for (int i = 0; i < CANTIDAD_JUGADORES; i++) {
		if (tp->pista_jugador[i].pista)
			lista_destruir_todo(tp->pista_jugador[i].pista,
					    liberar_obstaculos);
	}

	if (tp->tda) {
		if (tp->tda->abb)
			abb_destruir(tp->tda->abb);
		free(tp->tda);
	}

	free(tp);
}