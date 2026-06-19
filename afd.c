#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "afd.h"
#include "tdata.h"

void limpiar(char *str) {
	if (str == NULL) return;
	int i = 0;
	while(str[i] != '\0') {
		if(str[i] == '\r' || str[i] == '\n' || str[i] == ' ' || str[i] == '\t') {
			str[i] = '\0';
		}
		i++;
	}
}
void leecad(char cad[], int tam){
	int j=0, m;
	while(j<tam-1 && (m=getchar()) != EOF && m!='\n'){
		cad[j] = m;
		j++;
	}
	cad[j] = '\0';
	if(m !=EOF && m!= '\n')
		while((m=getchar()) != EOF && m!='\n');
}
	
tdata ing(){
	char buffer[100];
	leecad(buffer,100);
	tdata nuevo = create_str_ast();
	nuevo->string = load2(buffer);
	return nuevo;
}
automata crea_uno(tdata conj_q, tdata alfa_sig, int ini, tdata conj_f){
	automata A = (automata)malloc(sizeof(Automata));
	if(!A) return NULL;
	A->Q = conj_q;
	A->Sigma = alfa_sig;
	A->q0 = ini;
	A->F = conj_f;
	A->delta = NULL;
	A->deltaSize = 0;
	return A;
}
int indice_esta(automata a, tdata estado){
	if(!a || !a->Q || !estado) return -1;
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
	if(!a || !a->Sigma || !simbolo) return -1;
	tdata aux = a->Sigma;
	int i = 0;
	while(aux){
		if(aux->data && igual(aux->data, simbolo)) return i;
		i++;
		aux = aux->next;
	}
	return -1;
}
					
void agregar_transi(automata A, int from, int symbol, tdata est_dest){
	if(!A || !est_dest) return;
	for(int i = 0; i < A->deltaSize; i++){
		if(A->delta[i].from == from && A->delta[i].symbol == symbol){
			tdata u = union_set(A->delta[i].to, est_dest);
			free_tdata(A->delta[i].to);
			A->delta[i].to = u;
			return;
		}
	}
	A->delta = realloc(A->delta, (A->deltaSize + 1) * sizeof(TransitionEntry));
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
				if(texto[i] == ')' && nivel == 0) break;
				trans[j++] = texto[i++];
			}
			trans[j] = '\0';
			char origen[50], simbolo[50], destino[100];
			sepa(trans, origen, simbolo, destino);
			limpiar(origen);
			limpiar(simbolo);
			limpiar(destino);
			
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
	if(!a) return;
	printf("\n TABLA DE TRANSICIONES\n");
	printf("%-15s", "Estado");
	tdata aux = a->Sigma;
	while(aux){
		if(aux->data){ printf("| "); print_tdata(aux->data); printf("\t"); }
		aux = aux->next;
	}
	printf("\n--------------------------------------------------\n");
	tdata b = a->Q;
	int i = 0;
	while(b){
		if(b->data){
			printf("{"); print_str(b->data->string); printf("}\t");
			tdata aux_sim = a->Sigma;
			int j = 0;
			while(aux_sim){
				printf("| ");
				tdata cel_dest = NULL;
				for(int k = 0; k < a->deltaSize; k++){
					if(a->delta[k].from == i && a->delta[k].symbol == j){
						cel_dest = a->delta[k].to;
						break;
					}
				}
				if(!cel_dest || !cel_dest->data) printf("{}\t");
				else { print_tdata(cel_dest); printf("\t"); }
				j++;
				aux_sim = aux_sim->next;
			}
			printf("\n");
		}
		i++;
		b = b->next;
	}
}
	
int es_afd(automata a){
	if(!a || !a->delta) 
		return 0;
	for(int i = 0; i < a->deltaSize; i++){
		if(!a->delta[i].to || !a->delta[i].to->data || a->delta[i].to->next) 
			return 0;
	}
	return 1;
}
		
void liberar(automata a){
	if(!a) return;
	if(a->delta){
		for(int i=0; i<a->deltaSize; i++) if(a->delta[i].to) free_tdata(a->delta[i].to);
		free(a->delta);
	}
	if(a->Q) free_tdata(a->Q);
	if(a->Sigma) free_tdata(a->Sigma);
	if(a->F) free_tdata(a->F);
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
	for(int k=0; k<5; k++) 
		limpiar(dest[k]);
}
tdata leer_conjunto(char texto[]){
	tdata conjunto = NULL;
	char buffer[50];
	int i = 0, k = 0;
	while(texto[i] != '\0'){
		if(texto[i] == ' ' || texto[i] == '\n' || texto[i] == '\t' || texto[i] == '\r' || texto[i] =='{'){
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
	tdata estados = leer_conjunto(est);
	tdata alfabeto = leer_conjunto(alf);
	tdata finales = leer_conjunto(fin);
	tdata inicial = create_str_ast();
	inicial->string = load2(ini);
	int indice_ini = indice_esta_nodo(estados, inicial);
	automata A = crea_uno(estados, alfabeto, indice_ini, finales);
	leer_transiciones(tra, A);
	free_tdata(inicial);
	return A;
}
		
int vali_cad(automata a, char *cad){
	if(a == NULL || cad == NULL)
		return 0;
	int i;
	for(i=0; cad[i] !='\0'; i++){
		char sim_act = cad[i];
		int e = 0;
		
		tdata aux_sig = a->Sigma;
		while(aux_sig != NULL){
			if( aux_sig->data != NULL && aux_sig->data->string != NULL && aux_sig->data->string->letra == sim_act){
			e=1;
			break;
			}
			aux_sig = aux_sig->next;
		}
		if(!e)
			  return 0;
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
tdata obtener_estado_por_indice(tdata lista, int index) {
	tdata aux = lista;
	int i = 0;
	while (aux != NULL && i < index) {
		aux = aux->next;
		i++;
	}
	return (aux != NULL) ? aux->data : NULL;
}
int pertenece_cadena(automata a, char *cadena) {
	if (a == NULL || cadena == NULL) return 0;
	tdata estado_inicial = obtener_estado_por_indice(a->Q, a->q0);
	
	tdata estados_actuales = create_set();
	insert_set(&estados_actuales, estado_inicial);
	for (int i = 0; cadena[i] != '\0'; i++) {
		char simbolo_char = cadena[i];
		int sym_idx = -1;
		tdata aux_sig = a->Sigma;
		int s_idx = 0;
		while(aux_sig != NULL) {
			if (aux_sig->data != NULL && aux_sig->data->string != NULL && aux_sig->data->string->letra == simbolo_char) {
				sym_idx = s_idx;
				break;
			}
			s_idx++;
			aux_sig = aux_sig->next;
		}
		if (sym_idx == -1) {
			free_tdata(estados_actuales);
			return 0; 
		}
		
		tdata proximos_estados = create_set();
		tdata aux = estados_actuales;
		while (aux != NULL) {
			int idx_origen = indice_esta(a, aux->data);
			
			for (int k = 0; k < a->deltaSize; k++) {
				if (a->delta[k].from == idx_origen && a->delta[k].symbol == sym_idx) {
					tdata u = union_set(proximos_estados, a->delta[k].to);
					if (proximos_estados != NULL) free_tdata(proximos_estados);
					proximos_estados = u;
				}
			}
			aux = aux->next;
		}
		
		free_tdata(estados_actuales);
		estados_actuales = proximos_estados;
		
		if (estados_actuales == NULL || length(estados_actuales) == 0) 
			return 0;
	}
	tdata interseccion = intersection_set(estados_actuales, a->F);
	int aceptada = (interseccion != NULL && length(interseccion) > 0);
	
	free_tdata(interseccion);
	free_tdata(estados_actuales);
	return aceptada;
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
					int k;
					for(k = 0; k < afnd->deltaSize; k++){
						if(afnd->delta[k].from == f && afnd->delta[k].symbol == le){
							tdata u = union_set(conj_t, afnd->delta[k].to);
							if(conj_t != NULL)
								free_tdata(conj_t);
							
							conj_t = u;
						}
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
