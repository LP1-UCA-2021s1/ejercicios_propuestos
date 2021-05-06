
/**
 * Dado un número positivo, encontrar el menor valor posible realizando k
 * intercambios de sus dígitos.
 *
 * Input:  S = 934651, k = 1
 * Output: 134659
 *
 * Input:  S = 934651, k = 2
 * Output: 134569
 *
 * Input:  S = 52341, k = 2
 * Output: 12345 (Only 1 swap needed)
 *
 * Input:  S = 12345, k = 2
 * Output: 12345 (no change as all digits are already sorted in increasing order)
 * */
#include <stdio.h>
#include "laboratorios.h"
#include "util.h"
void ejercicio1(){
	char numeroStr[MAX_CARACTERES];
	int k, kAux;

	// lectura de la entrada: cadena de caracteres (solo caracteres numericos 0-9)
	printf("ingrese el valor a procesar:\n");
	scanf(INPUT_STR_FORMAT_DIGITS,numeroStr);
	limpiarBuffer();

	printf("ingrese el valor de k:\n");
	scanf("%d",&k);
	limpiarBuffer();

	 /*
	  * procesar cada caracter y seleccionar aquellos a intercambiar:
	  * - los digitos de menor valor deben quedar hacia adelante
	  * - si hay digitos repetidos a mover, se prefieren los que estan hacia el final de la cadena (mas cercanos a las unidades)
	 */
	int lastSwappedPos = -1, lenStr = strlen2(numeroStr)-1;
	kAux = k;
	// el intercambio se realiza maximo k veces, o cuando la cadena este completamente ordenada
	while(lastSwappedPos < lenStr && kAux>0){
		// se recorre la cadena de atras hacia adelante y se toma el menor digito hasta llegar a la ultima posicion intercambiada
		int i = lenStr,
			minDigit = *(numeroStr + i), // minDigit = numeroStr[i],
			minI = i;
		for(; i>lastSwappedPos; i--){
			// se selecciona el menor digito (entre los restantes) como siguiente a intercambiar
			if(*(numeroStr + i) < minDigit){
				minDigit = *(numeroStr + i);
				minI = i;
			}
		}

		// siguiente posicion a intercambiar
		lastSwappedPos++;
		// se intercambian: valor seleccionado y el siguiente a intercambiar
		if(lastSwappedPos < minI && *(numeroStr+lastSwappedPos) > minDigit){
			char aux = *(numeroStr+minI);
			*(numeroStr+minI) = *(numeroStr+lastSwappedPos);
			*(numeroStr+lastSwappedPos) = aux;
			kAux--;
		}
	}

	printf("Se intercambio %d veces. El resultado final es %s\n", k - kAux, numeroStr);
 }
