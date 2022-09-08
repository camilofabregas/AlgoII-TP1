#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "strutil.h"
#include "pila.h"

// Suma, resta, multiplicación, divisón, exponenciacion y logaritmo todas usan dos operandos, 
// asi que puedo usar una misma funcion para las seis operaciones.
typedef enum {SUMA, RESTA, MULTIPLICACION, DIVISION, EXPONENCIACION, LOGARITMO} oper_t;


long int operar_dos(long int* x, long int* y, oper_t oper) {
	long int resultado;
	if (oper == SUMA) resultado = *y + *x;
	else if (oper == RESTA) resultado = *y - *x;
	else if (oper == MULTIPLICACION) resultado = *y * *x;
	else if (oper == DIVISION) resultado = *y / *x;
	else if (oper == EXPONENCIACION) resultado = pow(*y, *x);
	else resultado = (log(*y) / log(*x));
	return resultado;
}


long int raiz_cuadrada(long int* x) {
	long int resultado = sqrt(*x);
	return resultado;
}


long int ternario(long int* x, long int* y, long int* z) {
	long int resultado = (*z) ? (*y) : (*x);
	return resultado;
}


bool pila_reducir(pila_t* pila_numeros, char* resto) {
	long *x = NULL, *y = NULL, *z = NULL;
	long int resultado = 0;
	bool ok = true;
	if (strcmp(resto, "+") == 0) {
		x = pila_desapilar(pila_numeros);
		y = pila_desapilar(pila_numeros);
		if (!x || !y) {
			ok = false;
		}
		else resultado = operar_dos(x, y, SUMA);
	}
	else if (strcmp(resto, "-") == 0) {
		x = pila_desapilar(pila_numeros);
		y = pila_desapilar(pila_numeros);
		if (!x || !y) {
			ok = false;
		}
		else resultado = operar_dos(x, y, RESTA);
	}
	else if (strcmp(resto, "*") == 0) {
		x = pila_desapilar(pila_numeros);
		y = pila_desapilar(pila_numeros);
		if (!x || !y) {
			ok = false;
		}
		else resultado = operar_dos(x, y, MULTIPLICACION);
	}
	else if (strcmp(resto, "/") == 0) {
		x = pila_desapilar(pila_numeros);
		y = pila_desapilar(pila_numeros);
		if (!x || !y || *x == 0) {
			ok = false;
		}
		else resultado = operar_dos(x, y, DIVISION);
	}
	else if (strcmp(resto, "^") == 0) {
		x = pila_desapilar(pila_numeros);
		y = pila_desapilar(pila_numeros);
		if (!x || !y || *x < 0) {
			ok = false;
		}
		else resultado = operar_dos(x, y, EXPONENCIACION);
	}
	else if (strcmp(resto, "log") == 0) {
		x = pila_desapilar(pila_numeros);
		y = pila_desapilar(pila_numeros);
		if (!x || !y || *x < 2) {
			ok = false;
		}
		else resultado = operar_dos(x, y, LOGARITMO);
	}
	else if (strcmp(resto, "sqrt") == 0) {
		x = pila_desapilar(pila_numeros);
		if (!x || *x < 0) {
			ok = false;
		}
		else resultado = raiz_cuadrada(x);
	}	
	else if (strcmp(resto, "?") == 0) {
		x = pila_desapilar(pila_numeros);
		y = pila_desapilar(pila_numeros);
		z = pila_desapilar(pila_numeros);
		if (!x || !y || !z) {
			ok = false;
		}
		else resultado = ternario(x, y, z);
	}
	else ok = false;
	if (ok) {
		long int* resultado_ptr = malloc(sizeof(long int));
		*resultado_ptr = resultado;
		pila_apilar(pila_numeros, resultado_ptr);
	}
	free(x);
	free(y);
	free(z);
	return ok;
}


char* eliminar_newline_final(char* linea) {
	size_t linea_largo = strlen(linea);
	if (linea[linea_largo-1] == '\n') {
		linea[linea_largo-1] = '\0';
	}
	return linea;
}

char** splittear_linea(char* linea) {
	char** linea_splitteada = split(linea, ' ');
	return linea_splitteada;
}


void procesar_linea(char* linea) {
	// Paso 1: Eliminar el \n del final de la linea.
	linea = eliminar_newline_final(linea);

	// Paso 2: Splittear los tokens en un arreglo.
	char** linea_splitteada = splittear_linea(linea);

	// Paso 3: Ciclar y enviar a la pila o realizar operacion.
	pila_t* pila_numeros = pila_crear();
	for (size_t i = 0; linea_splitteada[i]; i++) {
		char* cadena = linea_splitteada[i];
		char* resto = NULL;
		long int r = strtol(cadena, &resto, 10);
		long* r_ptr;
		if (*resto == '\0' && linea_splitteada[i]) {
			r_ptr = malloc(sizeof(long int));
			if (!r_ptr) return;
			*r_ptr = r;
			pila_apilar(pila_numeros, r_ptr);
		}
		else if (resto != cadena) {
			pila_destruir(pila_numeros);
			free_strv(linea_splitteada);
			printf("ERROR\n");
			return;
		}
		else {
			bool resultado = pila_reducir(pila_numeros, resto);
			if (!resultado) {
				while (!pila_esta_vacia(pila_numeros)) {
					long int* num = pila_desapilar(pila_numeros);
					free(num);
				}
				pila_destruir(pila_numeros);
				free_strv(linea_splitteada);
				printf("ERROR\n");
				return;
			}
		}
	}

	if (!pila_esta_vacia(pila_numeros)) {
		long int* resultado_final = pila_desapilar(pila_numeros);
		if (!pila_esta_vacia(pila_numeros)) {
				while (!pila_esta_vacia(pila_numeros)) {
					long int* num = pila_desapilar(pila_numeros);
					free(num);
				}
			printf("ERROR\n");
		}
		else printf("%ld\n", *resultado_final);
		free(resultado_final);
	}

	// Paso 4: liberar toda la memoria.
	pila_destruir(pila_numeros);
	free_strv(linea_splitteada);
}


int main(int argc, char* argv[]) {
	if (argc != 1) {
		printf("ERROR\n");
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