/*
 * leer denominaciones de la entrada
 * leer el valor de N
 * N es descompuesto en cada una de las denominaciones
 * verificar las veces que se puede repetir una denominacion
 */
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#define MAX_MONEDAS 20
#define MAX_DIGITOS 5
#define MAX_CAMBIOS 100
#define MAX_N 10

void obtenerCambios(int denominaciones[MAX_MONEDAS], int lengthDenominaciones, int valorACambiar, int cambios[MAX_CAMBIOS][MAX_N]);
int descomponerValor(int valorOriginal, int descomponer, int *cambioActual, int cambios[MAX_CAMBIOS][MAX_N], int denominaciones[MAX_MONEDAS], int lengthDenominaciones, int denAnterior);
void insertarValoresCambioActual(int from, int to, int value, int* cambioActual, int incrementarCambio, int cambios[MAX_CAMBIOS][MAX_N]);
void imprimirCambios(int cambios[MAX_CAMBIOS][MAX_N], int valor);

void parcial(){
	char continuar = 1;
	int id=0, denominaciones[MAX_MONEDAS];

	// denominaciones y valor de entrada
	do{
		// valores de entrada
		char valor[MAX_DIGITOS], c;

		// lectura del valor
		printf("Ingrese las denominaciones de moneda disponibles.\n");
		int r = scanf("%[0-9]s", valor);
		limpiarBuffer();
		if(r>0){
			denominaciones[id++] = atoi(valor);
			printf("Denominacion agregada\n");
		}else{
			printf("El valor ingresado es incorrecto\n");
		}

		// finaliza entrada?
		if((continuar = (id<MAX_MONEDAS))){
			printf("Desea seguir ingresando? y/n\n");
			r = scanf("%c",&c);
			limpiarBuffer();
			continuar = (c == 'y');
		}
	}while(continuar);

	// ordenamos los valores
	ordenarArray(denominaciones, id);
	printf("Las denominaciones de moneda (en orden): {");
	imprimirArray(denominaciones, id);
	printf(" }\n");

	// descomponer el valor
	int valor;
	printf("Introduzca el valor a cambiar, su valor maximo es %d \n", MAX_N);
	int r = scanf("%d",&valor);
	if(r>0 && valor>0 && valor <= MAX_N){
		// array de cambios obtenidos, inicializado en 0
		int cambios[MAX_CAMBIOS][MAX_N] = {0};
		int i,j;
		obtenerCambios(denominaciones, id, valor, cambios);
		imprimirCambios(cambios, valor);
	}else{
		printf("valor incorrecto\n");
	}
}

void obtenerCambios(int denominaciones[MAX_MONEDAS], int lengthDenominaciones, int valorACambiar, int cambios[MAX_CAMBIOS][MAX_N]){
	// recorremos el array de denominaciones completo y vemos como cada valor se puede combinar con los siguientes de la lista
	// en cada iteracion se considera solo las denominaciones que vienen despues (las anteriores ya fueron tenidas en cuenta previamente)
	int i, q, qi, sigCambio = 0;
	for(i=0; i< lengthDenominaciones;i++){
		// si un valor cabe mas de una vez en valorACambiar, se verifica todas las descomposiciones hasta incluirlo una sola vez
		q = valorACambiar / denominaciones[i];	// cuantas veces entra la denominacion actual en el valor a cambiar?
		int avanzar = 1;
		for(qi = q; qi>=1 && avanzar; qi--){
			int j, difDescomponer;
			difDescomponer = valorACambiar - (denominaciones[i] * qi);
			// incluyendo la denominacion actual qi veces, como puedo rellenar la diferencia? es posible encontrar mas de un cambio, esta funcion modifica el valor de sigCambio
			avanzar = descomponerValor(valorACambiar, difDescomponer, &sigCambio, cambios, denominaciones, lengthDenominaciones, i);
		}
		agregarCambiosDescompuestosParaDif(&sigCambio, cambios, denominaciones, lengthDenominaciones, denominaciones[i]);
	}
}

// se lee todo el array de cambios, descartando los valores que coinciden con el parametro noRepetir y se vuelve a ubicar sus descomposiciones en el array
void agregarCambiosDescompuestosParaDif(int *cambioActual, int cambios[MAX_CAMBIOS][MAX_N], int denominaciones[MAX_MONEDAS], int lengthDenominaciones, int noRepetir){
	int i,j;
	for(i=0;i<MAX_CAMBIOS && cambios[i][0]>0; i++){
		int pudoDescomponer = 0;
		for(j=0; !pudoDescomponer && j<MAX_N && cambios[i][j]>0;j++){
			// si tiene descomposicion valida, insertar al array de cambios
			if(cambios[i][j]!=noRepetir && cambios[i][j]>denominaciones[lengthDenominaciones-1]){
				pudoDescomponer = descomponerDiferencia(j, cambios[i][j], lengthDenominaciones, denominaciones, cambioActual, cambios, 0);
			}else{
				cambios[*cambioActual][j] = cambios[i][j];
			}
		}

		while(j<MAX_N && cambios[i][j]>0){
			cambios[*cambioActual][j] = cambios[i][j];
		}

		if(pudoDescomponer) *cambioActual += 1;
	}
}


// para el cambio actual, que se esta calculando en cambios[cambioActual], se ve de cuantas formas se
// puede combinar el resto de los valores de denominacion para sumar el valor restante a descomponer
// retorna valor numerico indicando si se puede avanzar o no con las descomposiciones de la denominacion actual
int descomponerValor(int valorOriginal, int descomponer, int *cambioActual, int cambios[MAX_CAMBIOS][MAX_N], int denominaciones[MAX_MONEDAS], int lengthDenominaciones, int denAnterior){
	// si ya descompusimos todos los valores anteriores, directamente verificamos si el ultimo valor es divisor del valor a descomponer
	if(denAnterior >= lengthDenominaciones - 1){
		// solo en el caso de que no tenga resto, podemos agregar el cambio
		if((valorOriginal % denominaciones[denAnterior])==0){
			insertarValoresCambioActual(0, valorOriginal/denominaciones[denAnterior],denominaciones[denAnterior], cambioActual, 1, cambios);
		}
		return 0;	// ya incluimos todos los valores de la ultima denominacion
	}

	// la denominacion anterior cubre totalmente el valor a cambiar
	if(descomponer == 0){
		insertarValoresCambioActual(0,valorOriginal/denominaciones[denAnterior], denominaciones[denAnterior],cambioActual, 1, cambios);
	}else{
		// se recorre el array de denominaciones a partir del siguiente valor
		int i;
		for(i=denAnterior+1;i<lengthDenominaciones;i++){
			int q = descomponer / denominaciones[i]; // cuantas veces entra el valor actual en la diferencia?
			// se inserta el valor actual la cantidad de veces que entra en el valor a descomponer
			insertarValoresCambioActual(0, q, denominaciones[i], cambioActual, 0, cambios);
			int resto = descomponer % denominaciones[i]; // cual es la diferencia restante?
			descomponerDiferencia(q, resto, lengthDenominaciones, denominaciones, cambioActual, cambios, 1);
		}
	}

	return 1;
}

// descompone el valor dado por la diferencia y lo agrega al array de cambios en caso de que se encuentre su descomposicion exacta
// si se agrego el elemento retorna verdadero
int descomponerDiferencia(int dondeInsertar, int resto, int lengthDenominaciones, int denominaciones[MAX_MONEDAS], int *cambioActual, int cambios[MAX_CAMBIOS][MAX_N], int actualizarCambio){
	// si hay diferencia, ver si alguno de los elementos restantes del array la cubren
	int qnext, j=0; // siguientes elementos del array
	while(resto>0 && j<=lengthDenominaciones){
		qnext = resto / denominaciones[j];
		// si entra el valor leido, lo añadimos al array de cambios y calculamos el resto
		if(qnext > 0){
			insertarValoresCambioActual(dondeInsertar, qnext, denominaciones[j], cambioActual, 0, cambios);
			dondeInsertar += qnext;
			resto = resto % denominaciones[j];
		}
		j++;
	}
	// si al recorrer todo el array el resto era > 0, no cubrimos el valor a descomponer totalmente, necesitamos quitar el elemento del array de cambios
	if(resto > 0){
		for(j=0;j<MAX_N; j++)
			cambios[*cambioActual][j] = 0;
	}else{
		if(actualizarCambio)
			*cambioActual += 1;	// se confirma el cambio cargado y pasamos al siguiente
	}
	return resto >0;
}

// se inserta el valor especificado en el array de cambios, actualizando el valor de cambio actual si asi se desea.
void insertarValoresCambioActual(int from, int to, int value, int* cambioActual, int incrementarCambio, int cambios[MAX_CAMBIOS][MAX_N]){
	int j;
	for(j=from;j<to;j++){
		cambios[*cambioActual][j] = value;
	}
	if(incrementarCambio)
		*cambioActual += 1;
}

// se imprime todos los valores <> 0 de cada fila
void imprimirCambios(int cambios[MAX_CAMBIOS][MAX_N], int valor){
	int i,j;
	printf("Los cambios posibles para el valor %d ingresado son:\n",valor);
	for(i=0;i<MAX_CAMBIOS && cambios[i][0]>0; i++){
		printf("{");
		for(j=0;j<MAX_N && cambios[i][j]>0;j++){
			if(j>0)
				printf(",");
			printf(" %d", cambios[i][j]);
		}
		printf(" }\n");
	}
	printf("(%d cambios en total)\n",i);
}
