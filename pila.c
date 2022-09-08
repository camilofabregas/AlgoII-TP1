#include "pila.h"
#include <stdlib.h>
#define TAM_INICIAL 4

/* Definición del struct pila proporcionado por la cátedra. */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/


pila_t* pila_crear(void) {
	pila_t* pila = malloc(sizeof(pila_t));
	if (pila == NULL) return NULL;
	pila->datos = malloc(TAM_INICIAL * sizeof(void*));
	if (pila->datos == NULL) {
		free(pila);
		return NULL;
	}
	pila->cantidad = 0;
	pila->capacidad = TAM_INICIAL;
	return pila;
}


void pila_destruir(pila_t* pila) {
	free(pila->datos);
	free(pila);
}


bool pila_esta_vacia(const pila_t* pila) {
	return pila->cantidad == 0;
}


static bool pila_redimensionar(pila_t* pila, size_t nuevo_tam) {
	void** datos_nuevo = realloc(pila->datos, nuevo_tam * sizeof(void*));
	if (datos_nuevo == NULL) return false;
	pila->datos = datos_nuevo;
	pila->capacidad = nuevo_tam;
	return true;
}


bool pila_apilar(pila_t* pila, void* valor) {
	if (pila->cantidad == pila->capacidad) {
		if (!pila_redimensionar(pila, pila->capacidad * 2)) return false;
	}
	pila->datos[pila->cantidad] = valor;
	pila->cantidad += 1;
	return true;
}


void* pila_ver_tope(const pila_t* pila) {
	if (pila->cantidad == 0) return NULL;
	return pila->datos[pila->cantidad - 1];
}


void* pila_desapilar(pila_t* pila) {
	if (pila->cantidad == 0) return NULL;
	if (pila->capacidad > TAM_INICIAL && pila->cantidad * 4 <= pila->capacidad){
		pila_redimensionar(pila, pila->capacidad / 2);
	}
	pila->cantidad -= 1;
	return pila->datos[pila->cantidad];
}


// ...
