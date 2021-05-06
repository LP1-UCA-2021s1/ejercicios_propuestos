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
