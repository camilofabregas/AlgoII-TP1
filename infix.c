#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "strutil.h"
#include "pila.h"
#include "cola.h"


bool es_operador(char* candidato) {
	if (strcmp(candidato, "+") == 0) return true;
	else if (strcmp(candidato, "-") == 0) return true;
	else if (strcmp(candidato, "*") == 0) return true;
	else if (strcmp(candidato, "/") == 0) return true;
	else if (strcmp(candidato, "^") == 0) return true;
	else return false;
}


bool es_parentesis_izq(char* candidato) {
	return strcmp(candidato, "(") == 0;
}


bool es_parentesis_der(char* candidato) {
	return strcmp(candidato, ")") == 0;
}


int precedencia(char* token) {
	if (!token) return 0;
	if (strcmp(token, "^") == 0) return 3;
	else if (strcmp(token, "*") == 0 || strcmp(token, "/") == 0) return 2;
	else if (strcmp(token, "+") == 0 || strcmp(token, "-") == 0) return 1;
	else return 0;
}

bool es_asociativo_izq(char* token) {
	return strcmp(token, "^") != 0;
}


void imprimir_salida(cola_t* salida) {
	bool es_primero = true;
	while (!cola_esta_vacia(salida)) {
		char* elem = cola_desencolar(salida);
		if (es_primero) {
			printf("%s", elem);
			es_primero = false;
		} 
		else printf(" %s", elem);
		free(elem);
	}
	printf("\n");
	return;
}


cola_t* generar_cola_entrada(cola_t* entrada, char* linea) {
	for (size_t i = 0; linea[i] != '\0'; i++) {
		char* elem;
		size_t largo = 1;
		if (isspace(linea[i])) continue;
		if (isdigit(linea[i])) {
			while (isdigit(linea[i + largo])) largo++;
		}
		elem = substr(linea + i, largo);
		cola_encolar(entrada, elem);
		i += largo-1;
	}
	return entrada;
}


void procesar_linea(char* linea) {
	// Paso 1: Generar cola de entrada.
	cola_t* entrada = cola_crear();
	entrada = generar_cola_entrada(entrada, linea);

	// Paso 3: Ciclar linea y apilar/encolar segun corresponda
	cola_t* salida = cola_crear();
	pila_t* operadores = pila_crear();
	while (!cola_esta_vacia(entrada)) {
		char* token = cola_desencolar(entrada);
		//if (!token) return;
		if (es_operador(token)) {
			while ((!pila_esta_vacia(operadores)) &&
				((precedencia(pila_ver_tope(operadores)) > precedencia(token) ||
				(precedencia(pila_ver_tope(operadores)) == precedencia(token) && es_asociativo_izq(token)))) &&
				(!es_parentesis_izq(pila_ver_tope(operadores)))) {
				cola_encolar(salida, pila_desapilar(operadores));
			}
			pila_apilar(operadores, token);
		}
		else if (es_parentesis_izq(token)) {
			pila_apilar(operadores, token);
		}
		else if (es_parentesis_der(token)) {
			while (!es_parentesis_izq(pila_ver_tope(operadores))) {
				cola_encolar(salida, pila_desapilar(operadores));
			}
			char* parentesis_izq = pila_desapilar(operadores);
			free(parentesis_izq);
			free(token);
		}
		else cola_encolar(salida, token); // Es numero
	}

	while (!pila_esta_vacia(operadores)) {
		cola_encolar(salida, pila_desapilar(operadores));
	}

	imprimir_salida(salida);

	pila_destruir(operadores);
	cola_destruir(salida, NULL);
	cola_destruir(entrada, NULL);
	return;
}


int main(int argc, char* argv[]) {
	if (argc != 1) {
		fprintf(stderr, "ERROR. Uso: %s < arith.txt\n", argv[0]);
		return 1;
	}

	size_t n = 0;
	char* linea = NULL;
	while ((getline(&linea, &n, stdin)) != EOF) {
		procesar_linea(linea);
	}
	free(linea);

	return 0;
}