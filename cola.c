#include "cola.h"
#include <stdlib.h>

/* Definición del struct nodo. */
typedef struct nodo {
	void* dato;
	struct nodo* proximo;
} nodo_t;


/* Definición del struct cola. */
struct cola {
	nodo_t* primero;
	nodo_t* ultimo;
};


/* *****************************************************************
 *                    FUNCIONES AUXILIARES
 * *****************************************************************/

static nodo_t* nodo_crear(void) {
	nodo_t* nodo = calloc(1, sizeof(nodo_t));
	return nodo;
}


/* *****************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

cola_t* cola_crear(void) {
	cola_t* cola = calloc(1, sizeof(cola_t));
	return cola;
}


void cola_destruir(cola_t *cola, void (*destruir_dato)(void*)) {
	nodo_t* nodo = cola->primero;
	while (nodo != NULL) {
		if (destruir_dato != NULL){
			destruir_dato(nodo->dato);
		}
		nodo_t* aux = nodo->proximo;
		free(nodo);
		nodo = aux;
	}
	free(cola);
}


bool cola_esta_vacia(const cola_t* cola) {
	return (!cola->primero && !cola->ultimo);
}


bool cola_encolar(cola_t* cola, void* valor) {
	nodo_t* nodo_nuevo = nodo_crear();
	if (!nodo_nuevo) return false;
	if (cola_esta_vacia(cola)) {
		cola->primero = nodo_nuevo;
	}
	else {
		cola->ultimo->proximo = nodo_nuevo;
	}
	cola->ultimo = nodo_nuevo;
	nodo_nuevo->dato = valor;
	return true;
}


void* cola_ver_primero(const cola_t* cola) {
	if (cola_esta_vacia(cola)) return NULL;
	return cola->primero->dato;
}


void* cola_desencolar(cola_t *cola) {
	if (cola_esta_vacia(cola)) return NULL;
	void* dato = cola->primero->dato;
	nodo_t* prox = cola->primero->proximo;
	free(cola->primero);
	cola->primero = prox;
	if (!cola->primero) {
		cola->ultimo = NULL;
	}
	return dato;
}