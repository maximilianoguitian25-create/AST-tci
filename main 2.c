#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "afd.h"

int main() {
    printf("--- Iniciando programa ---\n");
    automata a = carga_aut(); 
    
    if (a == NULL) {
        printf("Error: No se pudo cargar el automata. Saliendo...\n");
        return 1;
    }
    
    int opc;
    char cad[100]; 
    
    do{
        menu();
        printf("Ingrese una opcion: ");
        scanf("%d", &opc);
        
        switch(opc) {
        case 1:
            mostrar(a);
            break;
            
        case 2:
            if (es_afd(a)) printf("Es AFD\n");
            else printf("Es AFND\n");
            break;
            
        case 3:{
            int b=1;
            while(b==1) {
                printf("\nIngrese la cadena a evaluar (o '0' para volver al menu): ");
                leecad(cad,100);
                getchar();
                
                if (strcmp(cad, "0") == 0) {
                    b=0;
                }
                else{
                    if (pertenece_cadena(a, cad)) {
                        printf(">> ACEPTADA\n");
                    } else {
                        printf(">> RECHAZADA\n");
                    }
                }
            }
            break;
        }
        case 4:{
            automata afd=convertir(a);
            if(afd != NULL)
                mostrar(afd);
            else
                printf("Ya es un afd");
            }
            break;
        case 5:
            liberar(a);
            printf("PROFE APRUEBEME");
            break;
            
        default:
            printf("Opcion no valida.\n");
        }
    }while(opc!=5);
        
    return 0;
}

void menu(){
    printf("\n--- MENU ---\n");
    printf("1. Mostrar tabla de transiciones\n");
    printf("2. Verificar si es AFD o AFND\n");
    printf("3. Verificar si una cadena pertenece al automata\n");
    printf("4. Convertir AFND a AFD\n");
    printf("5. Salir\n");
}
