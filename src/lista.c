#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *nodo_inicio;
	nodo_t *tope;
	size_t contador_elementos;
};

struct lista_iterador {
	nodo_t *corriente;
	lista_t *lista;
};

nodo_t *crear_nodo()
{
	return calloc(1, sizeof(nodo_t));
}

lista_t *lista_crear()
{
	lista_t *lista_nueva = calloc(1, sizeof(lista_t));

	if (!lista_nueva)
		return NULL;

	return lista_nueva;
}

void *buscar_posicion_del_nodo(lista_t *lista, size_t posicion)
{
	nodo_t *nodo_actual = crear_nodo();
	size_t posicion_nodo = 0;

	nodo_actual = lista->nodo_inicio;

	while (nodo_actual) {
		if (posicion_nodo == posicion)
			return nodo_actual;
		nodo_actual = nodo_actual->siguiente;
		posicion_nodo++;
	}

	return NULL;
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (!lista)
		return NULL;

	nodo_t *nodo_nuevo = crear_nodo();

	if (!nodo_nuevo)
		return NULL;

	nodo_nuevo->elemento = elemento;
	nodo_nuevo->siguiente = NULL;

	if (!lista->nodo_inicio) {
		lista->nodo_inicio = nodo_nuevo;
	} else {
		lista->tope->siguiente = nodo_nuevo;
	}
	lista->tope = nodo_nuevo;
	lista->contador_elementos++;
	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (!lista)
		return NULL;

	nodo_t *nodo_nuevo = crear_nodo();
	nodo_t *posicion_nodo = crear_nodo();

	nodo_nuevo->elemento = elemento;
	nodo_nuevo->siguiente = NULL;

	if (posicion == 0) {
		nodo_nuevo->siguiente = lista->nodo_inicio;
		lista->nodo_inicio = nodo_nuevo;
		lista->contador_elementos++;
		if (lista->contador_elementos == 1)
			lista->tope = nodo_nuevo;
		return lista;
	}

	posicion_nodo = buscar_posicion_del_nodo(lista, posicion - 1);

	if (posicion_nodo) {
		nodo_nuevo->siguiente = posicion_nodo->siguiente;
		posicion_nodo->siguiente = nodo_nuevo;
		if (lista->contador_elementos == posicion)
			lista->tope = nodo_nuevo;
		lista->contador_elementos++;
		return lista;
	}
	free(nodo_nuevo);
	free(posicion_nodo);
	return lista_insertar(lista, elemento);
}

void *lista_quitar(lista_t *lista)
{
	void *elemento_a_eliminar = calloc(1, sizeof(void *));
	nodo_t *nodo_anterior = NULL;
	nodo_t *nodo_actual = NULL;

	if (!lista || !lista->nodo_inicio) {
		free(nodo_anterior);
		free(nodo_actual);
		return NULL;
	}

	nodo_actual = lista->nodo_inicio;

	while (nodo_actual->siguiente) {
		nodo_anterior = nodo_actual;
		nodo_actual = nodo_actual->siguiente;
	}

	if (!nodo_anterior) {
		lista->nodo_inicio = NULL;
	} else {
		lista->tope = nodo_anterior;
		nodo_anterior->siguiente = NULL;
	}

	if (nodo_anterior || nodo_actual) {
		elemento_a_eliminar = nodo_actual->elemento;
		lista->contador_elementos--;
		free(nodo_actual);
		return elemento_a_eliminar;
	}

	return NULL;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	void *elemento_a_eliminar = 0;
	nodo_t *posicion_nodo_buscado = crear_nodo();
	nodo_t *nodo = crear_nodo();
	nodo_t *nodo_anterior_al_buscado = crear_nodo();

	if (!lista || !lista->nodo_inicio) {
		free(nodo);
		free(posicion_nodo_buscado);
		free(nodo_anterior_al_buscado);

		return NULL;
	}

	if (posicion == 0) {
		nodo = lista->nodo_inicio;
		lista->nodo_inicio = nodo->siguiente;
		elemento_a_eliminar = nodo->elemento;
		free(nodo);
		lista->contador_elementos--;
		return elemento_a_eliminar;
	}

	posicion_nodo_buscado = buscar_posicion_del_nodo(lista, posicion);
	nodo_anterior_al_buscado =
		buscar_posicion_del_nodo(lista, posicion - 1);

	if (posicion_nodo_buscado) {
		nodo_anterior_al_buscado->siguiente =
			posicion_nodo_buscado->siguiente;
		elemento_a_eliminar = posicion_nodo_buscado->elemento;
		free(posicion_nodo_buscado);
		lista->contador_elementos--;
		return elemento_a_eliminar;
	} else {
		return lista_quitar(lista);
	}

	return NULL;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	nodo_t *posicion_nodo_buscado = crear_nodo();

	void *elemento = calloc(1, sizeof(void *));

	if (!lista) {
		free(posicion_nodo_buscado);
		return NULL;
	}

	posicion_nodo_buscado = buscar_posicion_del_nodo(lista, posicion);

	if (posicion_nodo_buscado) {
		elemento = posicion_nodo_buscado->elemento;
		return elemento;
	}

	return NULL;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	nodo_t *nodo_actual = crear_nodo();

	if (!lista || !comparador)
		return NULL;

	nodo_actual = lista->nodo_inicio;

	while (nodo_actual) {
		if (comparador(nodo_actual->elemento, contexto) == 0)
			return nodo_actual->elemento;
		nodo_actual = nodo_actual->siguiente;
	}

	return NULL;
}

void *lista_primero(lista_t *lista)
{
	if (!lista || !lista->nodo_inicio)
		return NULL;

	return lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if (!lista || !lista->nodo_inicio)
		return NULL;

	return lista->tope->elemento;
}

bool lista_vacia(lista_t *lista)
{
	if (!lista || !lista->nodo_inicio)
		return true;

	return false;
}

size_t lista_tamanio(lista_t *lista)
{
	if (!lista || !lista->nodo_inicio)
		return 0;

	return lista->contador_elementos;
}

void lista_destruir(lista_t *lista)
{
	if (!lista)
		return;

	nodo_t *nodo_actual = lista->nodo_inicio;
	nodo_t *nodo_siguiente = NULL;

	while (nodo_actual) {
		nodo_siguiente = nodo_actual->siguiente;
		free(nodo_actual);
		nodo_actual = nodo_siguiente;
	}

	free(lista);
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (!lista)
		return;

	nodo_t *nodo_actual = lista->nodo_inicio;
	nodo_t *nodo_siguiente = NULL;

	while (nodo_actual) {
		if (funcion)
			funcion(nodo_actual->elemento);
		nodo_siguiente = nodo_actual->siguiente;
		free(nodo_actual);
		nodo_actual = nodo_siguiente;
	}

	free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista)
		return NULL;

	lista_iterador_t *iterador = calloc(1, sizeof(lista_iterador_t));

	if (!iterador)
		return NULL;

	iterador->lista = lista;
	iterador->corriente = lista->nodo_inicio;

	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->corriente)
		return false;

	return (iterador->corriente);
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->corriente)
		return false;

	iterador->corriente = iterador->corriente->siguiente;
	return (iterador->corriente);
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->corriente)
		return NULL;

	return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (!lista || !funcion || !contexto)
		return 0;

	nodo_t *nodo_actual = NULL;

	size_t elementos = 0;
	bool continuar = true;
	size_t tamanio = lista_tamanio(lista);

	nodo_actual = lista->nodo_inicio;
	while (continuar && elementos < tamanio) {
		if (funcion) {
			continuar = funcion(nodo_actual->elemento, contexto);
			elementos++;
			nodo_actual = nodo_actual->siguiente;
		}
	}

	return elementos;
}