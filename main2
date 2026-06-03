#include <stdio.h>
#include <stdlib.h>
#include "afd.h"

int main() {
    automata mi_aut = NULL;
    automata aut_afd = NULL;
    int opcion = 0;
    int limpiar_char;

    printf("=== LABORATORIO DE AUTOMATAS - UNSa ===\n");

    while(opcion != 5) {
        printf("\n--- MENU DE OPCIONES ---\n");
        printf("1. Cargar Automata (AFND / AFD)\n");
        printf("2. Mostrar Automata Cargado\n");
        printf("3. Convertir AFND a AFD\n");
        printf("4. Evaluar Cadena en el AFD\n");
        printf("5. Salir\n");
        printf("Seleccione una opcion (1-5): ");
        
        // Capturamos la opción numérica
        if (scanf("%d", &opcion) != 1) {
            opcion = 0;
        }
        
        // Limpiamos el residuo del 'Enter' (\n) en el buffer para que no afecte a leecad
        while ((limpiar_char = getchar()) != EOF && limpiar_char != '\n');

        switch(opcion) {
            case 1:
                if (mi_aut != NULL) {
                    liberar(mi_aut);
                    mi_aut = NULL;
                }
                if (aut_afd != NULL && aut_afd != mi_aut) {
                    liberar(aut_afd);
                    aut_afd = NULL;
                }
                printf("\n[Iniciando carga del automata]\n");
                mi_aut = carga_aut();
                break;

            case 2:
                if (mi_aut != NULL) {
                    printf("\n>>> AUTOMATA ORIGINAL <<<");
                    mostrar(mi_aut);
                } else {
                    printf("\n[ERROR] Primero debe cargar un automata en la opcion 1.\n");
                }
                break;

            case 3:
                if (mi_aut != NULL) {
                    printf("\n[Procesando la conversion de subconjuntos...]\n");
                    aut_afd = convertir(mi_aut);
                    
                    printf("\n>>> AFD RESULTANTE <<<");
                    mostrar(aut_afd);
                } else {
                    printf("\n[ERROR] Primero debe cargar un automata en la opcion 1.\n");
                }
                break;

            case 4:
                // Evaluamos sobre el AFD obtenido en la opción 3 o sobre el original si ya era AFD
                if (aut_afd != NULL || mi_aut != NULL) {
                    automata a_evaluar = (aut_afd != NULL) ? aut_afd : mi_aut;
                    
                    printf("\nIngrese la cadena a evaluar (letra por letra): \n");
                    printf("*(Al terminar de armar la cadena, presione Enter vacio)*\n");
                    printf("Primera letra / simbolo: ");
                    
                    // Creamos la estructura de lista para la cadena usando tus TADs nativos
                    tdata cadena_usuario = ing(); 
                    
                    // Nota: Si para ir agregando letras consecutivas usas una funcion 
                    // como 'append' o 'insert' en bucle, podes armar un mini while aca.
                    // Por ahora evaluamos el caracter directo ingresado:
                    
                    if (pertene_cadena(a_evaluar, cadena_usuario)) {
                        printf("\n>>> ¡CADENA ACEPTADA POR EL AUTOMATA! <<<\n");
                    } else {
                        printf("\n>>> CADENA RECHAZADA (No pertenece al lenguaje) <<<\n");
                    }
                    free_tdata(cadena_usuario);
                } else {
                    printf("\n[ERROR] No hay ningun automata listo para evaluar.\n");
                }
                break;

            case 5:
                printf("\nLiberando memoria y saliendo del programa... ¡Hasta luego!\n");
                if (mi_aut != NULL) liberar(mi_aut);
                if (aut_afd != NULL && aut_afd != mi_aut) liberar(aut_afd);
                break;

            default:
                printf("\n[Opcion invalida] Por favor, ingrese un numero entre 1 y 5.\n");
                break;
        }
    }

    return 0;
}
