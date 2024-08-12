#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>

/*
En esta funcion me encargo de reservar memoria para un nodo, guardo el elemento pasado por parametro y los restantes los inicializo en 0 por el uso del Calloc
*/
nodo_abb_t *nodo_creado(void *elemento)
{
	nodo_abb_t *nodo = calloc(1, sizeof(nodo_abb_t));

	if (!nodo)
		return NULL;

	nodo->elemento = elemento;

	return nodo;
}

abb_t *abb_crear(abb_comparador comparador)
{
	if (!comparador)
		return NULL;

	abb_t *arbol = calloc(1, sizeof(abb_t));

	if (!arbol)
		return NULL;

	arbol->comparador = comparador;

	return arbol;
}

nodo_abb_t *insertar_en_el_arbol_rec(nodo_abb_t *nodo_actual, void *elemento,
				     int (*comparador)(void *, void *))
{
	if (!nodo_actual) {
		nodo_actual = nodo_creado(elemento);
		return nodo_actual;
	}

	int comparar = comparador(elemento, nodo_actual->elemento);

	if (!comparador)
		return NULL;

	if (comparar <= 0) {
		nodo_actual->izquierda = insertar_en_el_arbol_rec(
			nodo_actual->izquierda, elemento, comparador);
	} else {
		nodo_actual->derecha = insertar_en_el_arbol_rec(
			nodo_actual->derecha, elemento, comparador);
	}

	return nodo_actual;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;

	arbol->nodo_raiz = insertar_en_el_arbol_rec(arbol->nodo_raiz, elemento,
						    arbol->comparador);
	arbol->tamanio++;

	return arbol;
}

nodo_abb_t *buscar_nodo_predecesor(nodo_abb_t *nodo_actual, void **elemento)
{
	nodo_abb_t *raiz_guardada = NULL;

	if (!nodo_actual->derecha) {
		raiz_guardada = nodo_actual->izquierda;
		*elemento = nodo_actual->elemento;
		free(nodo_actual);
		return raiz_guardada;
	}

	nodo_actual->derecha =
		buscar_nodo_predecesor(nodo_actual->derecha, elemento);

	return nodo_actual;
}

nodo_abb_t *eliminar_nodo_del_arbol_rec(abb_t *arbol, nodo_abb_t *nodo_actual,
					void *elemento, bool *nodo_eliminado)
{
	if (!nodo_actual)
		return NULL;

	nodo_abb_t *hijo_nuevo = NULL;
	void *auxiliar = 0;
	int comparar = arbol->comparador(elemento, nodo_actual->elemento);

	if (comparar == 0) {
		if (!nodo_actual->izquierda || !nodo_actual->derecha) {
			hijo_nuevo = nodo_actual->derecha;
			if (!hijo_nuevo)
				hijo_nuevo = nodo_actual->izquierda;
			free(nodo_actual);
			*nodo_eliminado = true;
			arbol->tamanio--;
			return hijo_nuevo;
		} else if (nodo_actual->derecha && nodo_actual->izquierda) {
			nodo_actual->izquierda = buscar_nodo_predecesor(
				nodo_actual->izquierda, &auxiliar);
			nodo_actual->elemento = auxiliar;
			*nodo_eliminado = true;
			arbol->tamanio--;
			return nodo_actual;
		}
	}

	if (comparar < 0) {
		nodo_actual->izquierda = eliminar_nodo_del_arbol_rec(
			arbol, nodo_actual->izquierda, elemento,
			nodo_eliminado);
	} else {
		nodo_actual->derecha = eliminar_nodo_del_arbol_rec(
			arbol, nodo_actual->derecha, elemento, nodo_eliminado);
	}

	return nodo_actual;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;

	bool se_elimino_del_arbol = false;

	arbol->nodo_raiz = eliminar_nodo_del_arbol_rec(
		arbol, arbol->nodo_raiz, elemento, &se_elimino_del_arbol);

	if (!se_elimino_del_arbol)
		return NULL;

	return elemento;
}

void *buscar_en_el_arbol_rec(nodo_abb_t *nodo_actual, void *elemento,
			     int (*comparador)(void *, void *))
{
	if (!nodo_actual)
		return NULL;

	int comparar = comparador(elemento, nodo_actual->elemento);

	if (comparar == 0)
		return nodo_actual->elemento;

	if (comparar < 0)
		return buscar_en_el_arbol_rec(nodo_actual->izquierda, elemento,
					      comparador);

	return buscar_en_el_arbol_rec(nodo_actual->derecha, elemento,
				      comparador);
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;

	return buscar_en_el_arbol_rec(arbol->nodo_raiz, elemento,
				      arbol->comparador);
}

bool abb_vacio(abb_t *arbol)
{
	return (!arbol || arbol->tamanio == 0);
}

size_t abb_tamanio(abb_t *arbol)
{
	if (!arbol)
		return 0;

	return arbol->tamanio;
}

void destruir_todo_del_arbol_rec(nodo_abb_t *nodo_actual,
				 void (*destructor)(void *))
{
	if (!nodo_actual)
		return;

	if (destructor != NULL)
		destructor(nodo_actual->elemento);

	destruir_todo_del_arbol_rec(nodo_actual->izquierda, destructor);
	destruir_todo_del_arbol_rec(nodo_actual->derecha, destructor);
	free(nodo_actual);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (!arbol)
		return;

	destruir_todo_del_arbol_rec(arbol->nodo_raiz, destructor);

	free(arbol);
}

void abb_destruir(abb_t *arbol)
{
	if (!arbol)
		return;

	abb_destruir_todo(arbol, NULL);
}

size_t recorrer_arbol_preorden(nodo_abb_t *nodo_actual, void **vector,
			       size_t tamaño_vector, size_t *contador)
{
	if (!nodo_actual || *contador >= tamaño_vector)
		return *contador;

	if (*contador < tamaño_vector)
		vector[(*contador)++] = nodo_actual->elemento;

	*contador = recorrer_arbol_preorden(nodo_actual->izquierda, vector,
					    tamaño_vector, contador);

	*contador = recorrer_arbol_preorden(nodo_actual->derecha, vector,
					    tamaño_vector, contador);

	return *contador;
}

size_t recorrer_arbol_inorden(nodo_abb_t *nodo_actual, void **vector,
			      size_t tamaño_vector, size_t *contador)
{
	if (!nodo_actual || *contador >= tamaño_vector)
		return *contador;

	*contador = recorrer_arbol_inorden(nodo_actual->izquierda, vector,
					   tamaño_vector, contador);

	if (*contador < tamaño_vector)
		vector[(*contador)++] = nodo_actual->elemento;

	*contador = recorrer_arbol_inorden(nodo_actual->derecha, vector,
					   tamaño_vector, contador);

	return *contador;
}

size_t recorrer_arbol_posorden(nodo_abb_t *nodo_actual, void **vector,
			       size_t tamaño_vector, size_t *contador)
{
	if (!nodo_actual || *contador >= tamaño_vector)
		return *contador;

	*contador = recorrer_arbol_posorden(nodo_actual->izquierda, vector,
					    tamaño_vector, contador);

	*contador = recorrer_arbol_posorden(nodo_actual->derecha, vector,
					    tamaño_vector, contador);

	if (*contador < tamaño_vector)
		vector[(*contador)++] = nodo_actual->elemento;

	return *contador;
}

bool arbol_con_cada_elemento_preorden(nodo_abb_t *nodo_actual,
				      bool (*funcion)(void *, void *),
				      void *auxiliar, size_t *contador)
{
	if (!nodo_actual)
		return true;

	bool continuar_a_izquierda = false;

	(*contador)++;

	if (!funcion(nodo_actual->elemento, auxiliar))
		return false;

	continuar_a_izquierda = arbol_con_cada_elemento_preorden(
		nodo_actual->izquierda, funcion, auxiliar, contador);

	if (!continuar_a_izquierda)
		return false;

	return arbol_con_cada_elemento_preorden(nodo_actual->derecha, funcion,
						auxiliar, contador);
}

bool arbol_con_cada_elemento_inorden(nodo_abb_t *nodo_actual,
				     bool (*funcion)(void *, void *),
				     void *auxiliar, size_t *contador)
{
	if (!nodo_actual)
		return true;

	bool continuar_a_izquierda = false;

	continuar_a_izquierda = arbol_con_cada_elemento_inorden(
		nodo_actual->izquierda, funcion, auxiliar, contador);

	if (!continuar_a_izquierda)
		return false;

	(*contador)++;

	if (!funcion(nodo_actual->elemento, auxiliar))
		return false;

	return arbol_con_cada_elemento_inorden(nodo_actual->derecha, funcion,
					       auxiliar, contador);
}

bool arbol_con_cada_elemento_posorden(nodo_abb_t *nodo_actual,
				      bool (*funcion)(void *, void *),
				      void *auxiliar, size_t *contador)
{
	if (!nodo_actual)
		return true;

	bool continuar = false;

	continuar = arbol_con_cada_elemento_posorden(
		nodo_actual->izquierda, funcion, auxiliar, contador);

	if (!continuar)
		return false;

	continuar = arbol_con_cada_elemento_posorden(
		nodo_actual->derecha, funcion, auxiliar, contador);

	if (!continuar)
		return false;

	(*contador)++;

	if (!funcion(nodo_actual->elemento, auxiliar))
		return false;

	return true;
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (!arbol || !funcion)
		return 0;

	size_t contador = 0;

	if (recorrido == INORDEN) {
		arbol_con_cada_elemento_inorden(arbol->nodo_raiz, funcion, aux,
						&contador);
	} else if (recorrido == PREORDEN) {
		arbol_con_cada_elemento_preorden(arbol->nodo_raiz, funcion, aux,
						 &contador);
	} else if (recorrido == POSTORDEN) {
		arbol_con_cada_elemento_posorden(arbol->nodo_raiz, funcion, aux,
						 &contador);
	}

	return contador;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (abb_vacio(arbol) || !array || tamanio_array == 0)
		return 0;

	size_t contador = 0;

	if (recorrido == INORDEN) {
		contador = recorrer_arbol_inorden(arbol->nodo_raiz, array,
						  tamanio_array, &contador);
	} else if (recorrido == PREORDEN) {
		contador = recorrer_arbol_preorden(arbol->nodo_raiz, array,
						   tamanio_array, &contador);
	} else if (recorrido == POSTORDEN) {
		contador = recorrer_arbol_posorden(arbol->nodo_raiz, array,
						   tamanio_array, &contador);
	}

	return contador;
}