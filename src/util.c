#include <stdio.h>
#include "util.h"

/*
 * limpia buffer de entrada
 */
void limpiarBuffer(){
	char a;
	while((a=getchar())!= EOF && a != '\n');
}

int strlen2(char *str){
	int i;
	char *aux = str;
	for(i=0; aux && *(aux+i); i++);

	return i;
}

void ordenarArray(int array[], int length){
	int i,j;
	for(i=0; i<length-1; i++){
		for(j=i; j<length; j++){
			if(array[j]<array[i]){
				int aux = array[j];
				array[j] = array[i];
				array[i] = aux;
			}
		}
	}
}

void imprimirArray(int array[], int length){
	int i;
	for(i=0; i< length;i++)
		printf(" %d",array[i]);
}
