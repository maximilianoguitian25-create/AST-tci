#include <stdio.h>
#include <stdlib.h>
#include >string.h>
#include "afd.h"
#include "tdata.h"
#include "str.h"

void menu();

int main(){
	automata a = NULL;
	automata af = NULL;
	int opc = 0;
	char cad[100];
	tdata cad_list =NULL;
	
	do{
	
		menu();
		printf("Ingrese una opcion\n");
		scanf("%d", &opc)
		switch(opc):
			case1:
	switch(opc == 0){
		case 1:
			a=carga_aut();
			printf("Automata\n");
			break;
		case 2:
			mostrar(a);
			if(af!=NULL)
				mostrar(af);
			break;
		case 3:
			if(es_afd(a))
				printf("es AFD\n");
			else
				printf("es AFND\n");
			break;

		case 4:
			printf("Ingrese la cadena a evaluar\n");
			leecad(cad,100);
			tdata leo=create_str_ast();
			leo->string = load2(cad);
			cad_lis = str_to_list(leo);
			if(pertene_cadena(a,cad_lis))
				printf("ACEPTADA\n");
			else
				printf("RECHAZADA\n");
			free_tdata(leo);
			free_tdata(cad_lis);
			cad_lis = NULL;
			break;

		case 5:
			printf("CONVERTIENDO\n");
			af =convertir(a);
			mostrar(af);
			break;

		case 6:
			printf("PROFE APRUEBEME\n");
			break;
			
		default:
			printf("opcion incorrecta\n");
			break;
	}
	}while(opc!=6);
	
	return 0;
}
void menu(){
	printf("MENU\n");
	printf("1. cargar automata \n");
	printf("2. mostrar tabla");
	printf("3. es AFD o AFND\n");
	printf("4. cadena pertenece al automata\n");
	printf("5. convertir AFND a AFD \n");
	printf("6. salir \n");
	printf("Ingrese una opcion: ");
}
