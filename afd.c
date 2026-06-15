#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "afd.h"
#include "tdata.h"

void leecad(char cad[], int tam){
    int j=0, m;
    while(j < tam-1 && (m = getchar()) != EOF && m != '\n'){
        cad[j] = m;
        j++;
    }
    cad[j] = '\0';
    if(m != EOF && m != '\n')
        while((m = getchar()) != EOF && m != '\n');
}

tdata ing(){
    char buffer[100];
    leecad(buffer, 100);
    tdata nuevo = create_str_ast();
    nuevo->string = load2(buffer);
    return nuevo;
}

automata crea_uno(tdata conj_q, tdata alfa_sig, int ini, tdata conj_f){
    automata A = (automata)malloc(sizeof(Automata));
    if(A == NULL)
        return NULL;
    A->Q = conj_q;
    A->Sigma = alfa_sig;
    A->q0 = ini;
    A->F = conj_f;
    A->delta = NULL;
    A->deltaSize = 0;
    return A;
}

int indice_esta(automata a, tdata estado){
    if(!a || !a->Q || !estado || !estado->string)
        return -1;
    tdata aux = a->Q;
    int i = 0;
    while(aux){
        if(aux->data && igual(aux->data, estado))
            return i;
        i++;
        aux = aux->next;
    }
    return -1;
}

int indice_simb(automata a, tdata simbolo){
    if(!a || !a->Sigma || !simbolo || !simbolo->string)
        return -1;
    tdata aux = a->Sigma;
    int i = 0;
    while(aux){
        if(aux->data && igual(aux->data, simbolo))
            return i;
        i++;
        aux = aux->next;
    }
    return -1;
}

void agregar_transi(automata A, int from, int symbol, tdata est_dest){
    if(!A || !est_dest)
        return;
    for(int i = 0; i < A->deltaSize; i++){
        if(A->delta[i].from == from && A->delta[i].symbol == symbol){
            tdata u = union_set(A->delta[i].to, est_dest);
            free_tdata(A->delta[i].to);
            A->delta[i].to = u;
            return;
        }
    }
    TransitionEntry* tmp = realloc(A->delta, (A->deltaSize + 1) * sizeof(TransitionEntry));
    if(!tmp)
        return;
    A->delta = tmp;
    A->delta[A->deltaSize].from = from;
    A->delta[A->deltaSize].symbol = symbol;
    A->delta[A->deltaSize].to = clone(est_dest);
    A->deltaSize++;
}

void leer_transiciones(char texto[], automata A){
    int i = 0;
    while(texto[i] != '\0'){
        if(texto[i] == '('){
            char trans[200];
            int j = 0;
            int nivel = 0;
            i++;
            while(texto[i] != '\0'){
                if(texto[i] == '{') nivel++;
                if(texto[i] == '}') nivel--;
                if(texto[i] == ')' && nivel == 0)
                    break;
                trans[j++] = texto[i++];
            }
            trans[j] = '\0';
            char origen[50], simbolo[50], destino[100];
            sepa(trans, origen, simbolo, destino);
            tdata ori = create_str_ast();
            ori->string = load2(origen);
            tdata sim = create_str_ast();
            sim->string = load2(simbolo);
            tdata dest = leer_conjunto(destino);
            int from = indice_esta(A, ori);
            int col = indice_simb(A, sim);
            if(from != -1 && col != -1){
                agregar_transi(A, from, col, dest);
            }
            free_tdata(ori);
            free_tdata(sim);
            free_tdata(dest);
        }
        i++;
    }
}

void mostrar(automata a){
    if(a == NULL)
        return;
    printf("\n TABLA DE TRANSICIONES\n");
    printf("%-15s", "Estado");
    tdata aux = a->Sigma;
    while(aux != NULL){
        if(aux->data != NULL){
            printf("| ");
            print_tdata(aux->data);
            printf("\t\t");
        }
        aux = aux->next;
    }
    printf("\n--------------------------------------------------\n");
    tdata b = a->Q;
    int i = 0;
    while(b != NULL){
        if(b->data != NULL){
            printf("{");
            print_str(b->data->string);
            printf("}");
            printf("            ");
            tdata aux_sim = a->Sigma;
            int j = 0;
            while(aux_sim != NULL){
                printf("| ");
                tdata cel_dest = NULL;
                int k = 0, ban = 0;
                while(k < a->deltaSize && ban == 0){
                    if(a->delta[k].from == i && a->delta[k].symbol == j){
                        cel_dest = a->delta[k].to;
                        ban = 1;
                    }
                    k++;
                }
                if(cel_dest == NULL || cel_dest->data == NULL) {
                    printf("{}\t\t");
                } else {
                    print_tdata(cel_dest);
                    printf("\t\t");
                }
                j++;
                aux_sim = aux_sim->next;
            }
            printf("\n");
        }
        i++;
        b = b->next;
    }
    printf("\n==================================================\n");
}

int es_afd(automata a){
    if(a == NULL || a->delta == NULL)
        return 0;
    for(int i = 0; i < a->deltaSize; i++){
        tdata dest = a->delta[i].to;
        if(dest == NULL || dest->data == NULL){
            return 0;
        }
        if(dest->next != NULL){
            return 0;
        }
    }
    return 1;
}

void liberar(automata a){
    if(a == NULL)
        return;
    if(a->delta != NULL){
        int i;
        for(i = 0; i < a->deltaSize; i++){
            if(a->delta[i].to != NULL)
                free_tdata(a->delta[i].to);
        }
        free(a->delta);
    }
    if(a->Q != NULL)
        free_tdata(a->Q);
    if(a->Sigma != NULL)
        free_tdata(a->Sigma);
    if(a->F != NULL)
        free_tdata(a->F);
    free(a);
}

int indice_esta_nodo(tdata lista, tdata estado){
    tdata aux = lista;
    int i = 0;
    while(aux != NULL){
        if(aux->data != NULL && igual(aux->data, estado))
            return i;
        i++;
        aux = aux->next;
    }
    return -1;
}

void sepa(char trans[], char origen[], char simbolo[], char destino[]){
    int i = 0;
    int j = 0;
    int campo = 0;
    int nivel = 0;
    while(trans[i] != '\0'){
        if(trans[i] == ' ' || trans[i] == '\t' || trans[i] == '\n' || trans[i] == '\r'){
            i++;
            continue;
        }
        if(trans[i] == '{') nivel++;
        if(trans[i] == '}') nivel--;
        if(trans[i] == ',' && nivel == 0){
            if(campo == 0) origen[j] = '\0';
            if(campo == 1) simbolo[j] = '\0';
            campo++;
            j = 0;
        }
        else{
            if(campo == 0){
                origen[j] = trans[i];
            }
            else if(campo == 1){
                simbolo[j] = trans[i];
            }
            else{
                destino[j] = trans[i];
            }
            j++;
        }
        i++;
    }
    destino[j] = '\0';
}

void separar_automata(char *lin, char est[], char alf[], char tra[], char ini[], char fin[]){
    int i = 1;
    int campo = 0;
    int j = 0;
    int nivel = 0;
    int nivel_p = 0;
    char *dest[5];
    dest[0] = est;
    dest[1] = alf;
    dest[2] = tra;
    dest[3] = ini;
    dest[4] = fin;
    while(lin[i] != '\0'){
        if(lin[i] == '\r' || lin[i] == '\n'){
            i++;
            continue;
        }
        if(lin[i] == ')' && nivel == 0 && nivel_p == 0 && lin[i+1] == '\0'){
            break;
        }
        if(lin[i] == '{') nivel++;
        if(lin[i] == '}') nivel--;
        if(lin[i] == '(') nivel_p++;
        if(lin[i] == ')') nivel_p--;
        if(lin[i] == ',' && nivel == 0 && nivel_p == 0){
            dest[campo][j] = '\0';
            campo++;
            j = 0;
        }
        else{
            if(campo < 5){
                dest[campo][j] = lin[i];
                j++;
            }
        }
        i++;
    }
    dest[campo][j] = '\0';
}

tdata leer_conjunto(char texto[]){
    tdata conjunto = NULL;
    char buffer[50];
    int i = 0, k = 0;
    while(texto[i] != '\0'){
        if(texto[i] == ' ' || texto[i] == '\n' || texto[i] == '\t' || texto[i] == '\r' || texto[i] == '{'){
            i++;
            continue;
        }
        if(texto[i] != ',' && texto[i] != '}'){
            buffer[k++] = texto[i];
        } else {
            buffer[k] = '\0';
            if(k > 0){
                tdata e = create_str_ast();
                e->string = load2(buffer);
                insert_set(&conjunto, e);
                free_tdata(e);
            }
            k = 0;
        }
        i++;
    }
    return conjunto;
}

automata carga_aut(){
    char linea[1000];
    char est[200];
    char alf[200];
    char tra[500];
    char ini[100];
    char fin[200];
    printf("Ingrese la definicion del automata:\n");
    leecad(linea, 1000);
    separar_automata(linea, est, alf, tra, ini, fin);
    printf("\nEST = %s", est);
    printf("\nALF = %s", alf);
    printf("\nTRA = %s", tra);
    printf("\nINI = %s", ini);
    printf("\nFIN = %s\n", fin);
    tdata estados = leer_conjunto(est);
    tdata alfabeto = leer_conjunto(alf);
    tdata finales = leer_conjunto(fin);
    printf("\nESTADOS COMO TDATA: ");
    print_tdata(estados);
    printf("\nALFABETO COMO TDATA: ");
    print_tdata(alfabeto);
    printf("\nFINALES COMO TDATA: ");
    print_tdata(finales);
    tdata inicial = create_str_ast();
    inicial->string = load2(ini);
    int indice_ini = indice_esta_nodo(estados, inicial);
    automata A = crea_uno(estados, alfabeto, indice_ini, finales);
    leer_transiciones(tra, A);
    free_tdata(inicial);
    return A;
}

int vali_cad(automata a, tdata cad){
    if(a == NULL || cad == NULL)
        return 0;
    tdata c = cad;
    while(c != NULL){
        if(c->data != NULL){
            if(indice_simb(a, c->data) == -1){
                printf(" RECHAZADA(no pertenece al alfabeto) \n");
                return 0;
            }
        }
        c = c->next;
    }
    return 1;
}

tdata renom(int i){
    char buffer[20];
    sprintf(buffer, "p%d", i);
    tdata nuevo = create_str_ast();
    nuevo->string = load2(buffer);
    return nuevo;
}

tdata bus_nom(tdata lis_sub, tdata lis_nom, tdata sub_bus){
    tdata aux_sub = lis_sub;
    tdata aux_nom = lis_nom;
    while(aux_sub != NULL && aux_nom != NULL){
        tdata conj_exi = aux_sub->data;
        tdata inter = intersection_set(conj_exi, sub_bus);
        if(length(inter) == length(conj_exi) && length(inter) == length(sub_bus)){
            free_tdata(inter);
            return aux_nom->data;
        }
        free_tdata(inter);
        aux_sub = aux_sub->next;
        aux_nom = aux_nom->next;
    }
    return NULL;
}

int pertenece_cadena(automata a, char *cadena) {
    if (a == NULL || cadena == NULL) return 0;

    tdata aux_q = a->Q;
    int idx = 0;
    while (aux_q != NULL && idx < a->q0) {
        idx++;
        aux_q = aux_q->next;
    }
    if (aux_q == NULL || aux_q->data == NULL) return 0;

    tdata estados_actuales = NULL;
    insert_set(&estados_actuales, aux_q->data);

    int i = 0;
    while (cadena[i] != '\0') {
        char buffer_s[2] = {cadena[i], '\0'};
        tdata temp_sim = create_str_ast();
        temp_sim->string = load2(buffer_s);
        int sim_idx = indice_simb(a, temp_sim);
        free_tdata(temp_sim);

        if (sim_idx == -1) {
            free_tdata(estados_actuales);
            return 0;
        }

        tdata estados_siguientes = NULL;
        tdata curr = estados_actuales;

        while (curr != NULL) {
            int fila = indice_esta(a, curr->data);
            if (fila != -1) {
                for (int k = 0; k < a->deltaSize; k++) {
                    if (a->delta[k].from == fila && a->delta[k].symbol == sim_idx) {
                        tdata u = union_set(estados_siguientes, a->delta[k].to);
                        if (estados_siguientes != NULL) free_tdata(estados_siguientes);
                        estados_siguientes = u;
                    }
                }
            }
            curr = curr->next;
        }

        free_tdata(estados_actuales);
        estados_actuales = estados_siguientes;

        if (estados_actuales == NULL) {
            return 0;
        }
        i++;
    }

    int aceptada = 0;
    int l = 0;
    if (estados_actuales != NULL) {
        tdata inter = intersection_set(estados_actuales, a->F);
        if (inter != NULL) {
            if (length(inter) > 0) {
                l = 1;
            }
            free_tdata(inter);
        }
        free_tdata(estados_actuales);
    }
    return l;
}

automata convertir(automata afnd){
    if(afnd == NULL)
        return NULL;
    if(es_afd(afnd)){
        printf("El automata ya es un AFD\n");
        return afnd;
    }
    int cont_p = 0;
    tdata nue_q = NULL, nue_f = NULL;
    tdata conj_i = NULL;
    tdata aux = afnd->Q;
    int i = 0;
    while(aux != NULL && i != afnd->q0){
        i++;
        aux = aux->next;
    }
    if(aux != NULL)
        insert_set(&conj_i, aux->data);
    tdata ini_n = renom(cont_p++);
    insert_set(&nue_q, ini_n);
    tdata l = intersection_set(conj_i, afnd->F);
    if(length(l) > 0)
        insert_set(&nue_f, ini_n);
    free_tdata(l);
    tdata sub = NULL, nom = NULL;
    append(&sub, conj_i);
    append(&nom, ini_n);
    tdata sub_p = NULL, nom_p = NULL;
    append(&sub_p, conj_i);
    append(&nom_p, ini_n);
    automata afd_res = crea_uno(nue_q, afnd->Sigma, 0, nue_f);
    while(sub_p != NULL){
        tdata sub_a = sub_p->data;
        tdata nom_a = nom_p->data;
        tdata aux_sim = afnd->Sigma;
        int le = 0;
        while(aux_sim != NULL){
            tdata conj_t = NULL;
            tdata aux_sub = sub_a;
            while(aux_sub != NULL){
                tdata item_real = aux_sub->data;
                int f = indice_esta(afnd, item_real);
                if(f != -1){
                    int k = 0;
                    int enc = 0;
                    while(k < afnd->deltaSize && enc == 0){
                        if(afnd->delta[k].from == f && afnd->delta[k].symbol == le){
                            tdata u = union_set(conj_t, afnd->delta[k].to);
                            if(conj_t != NULL)
                                free_tdata(conj_t);
                            conj_t = u;
                            enc = 1;
                        }
                        k++;
                    }
                }
                aux_sub = aux_sub->next;
            }
            if(conj_t != NULL){
                tdata nom_d = bus_nom(sub, nom, conj_t);
                if(nom_d == NULL){
                    nom_d = renom(cont_p++);
                    insert_set(&(afd_res->Q), nom_d);
                    tdata inter_f = intersection_set(conj_t, afnd->F);
                    if(length(inter_f) > 0)
                        insert_set(&(afd_res->F), nom_d);
                    free_tdata(inter_f);
                    append(&sub, conj_t);
                    append(&nom, nom_d);
                    append(&sub_p, conj_t);
                    append(&nom_p, nom_d);
                }
                int f_res = indice_esta(afd_res, nom_a);
                tdata celda = NULL;
                insert_set(&celda, nom_d);
                agregar_transi(afd_res, f_res, le, celda);
                free_tdata(celda);
            }
            le++;
            aux_sim = aux_sim->next;
        }
        tdata bor_sub = sub_p;
        tdata bor_nom = nom_p;
        sub_p = sub_p->next;
        nom_p = nom_p->next;
        bor_sub->next = NULL;
        bor_nom->next = NULL;
        free_tdata(bor_sub);
        free_tdata(bor_nom);
    }
    free_tdata(sub);
    free_tdata(nom);
    return afd_res;
}
