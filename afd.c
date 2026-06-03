#include "afd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void leecad(char cad[], int tam){
	int j,m;
	j=0;
	while(j<tam-1 && (m=getchar()) != EOF && m!='\n'){
		cad[j] = m;
		j++;
	}
	cad[j] = '\0';
	if(m !=EOF && m!= '\n')
		while((m=getchar ()) != EOF && m!='\n');
}

tdata ing(){
	char buffer[100];
	leecad(buffer,100);
	tdata nuevo = create_str_ast();
	nuevo->string = load2(buffer);
	return nuevo;
}

automata crea_uno(tdata conj_q, tdata alfa_sig, tdata ini, tdata conj_f){
	
	automata A =(automata)malloc(sizeof(struct automataNode));
	if( A ==NULL)
		return NULL;
	
	A->q = conj_q;
	A->sig = alfa_sig;
	A->q0 = ini;
	A->f = conj_f;
	
	A->cantest = length(conj_q);
	A->cantsim = length(alfa_sig);
	
	A->tabla = (tdata**)malloc(A->cantest* sizeof(tdata*));
	if(A->tabla ==NULL){
		free(A);
		return NULL;
		
	}
	int i;
	for( i=0;i<A->cantest; i++){
		
		A->tabla[i] =(tdata*) malloc (A->cantsim * sizeof(tdata));
		
		int j;
		for(j=0;  j<A->cantsim; j++){
			A->tabla[i][j] = NULL;
		}
	}
	
	return A;
}
	

int indice_esta(automata a, tdata estado){
	if( a == NULL || a->q == NULL || estado == NULL)
		return -1;
	tdata aux = a->q;
	int i= 0;
	while(aux!=NULL){
		
		if(aux->data != NULL && igual(aux->data, estado))
			return i;
		i++;
		aux=aux->next;
		
	}
	return -1;
}
	

int indice_simb( automata a, tdata simbolo){
	
	if(a == NULL || a->sig == NULL || simbolo == NULL) 
		return -1;
	tdata aux = a->sig;
	int i=0;
	while( aux!= NULL){
		if( aux->data != NULL && igual(aux->data, simbolo)){
			return i;
			
		}
		i++;
		aux = aux->next;
		
	}
	return -1;
	
}
	

void agregar_transi(automata A, tdata ini, tdata simbolo , tdata est_dest){
	
	if(A==NULL || ini == NULL || simbolo == NULL || est_dest == NULL)
		return;
	int f = indice_esta( A, ini);
	int c = indice_simb(A, simbolo);
		
	if( f == -1 || c == -1){
		printf("ERROR");
		return;
	}
	
	insert_set(&( A->tabla[f][c]), est_dest);
}

void mostrar(automata a){
	if( a == NULL)
		return ;
	printf("\n TABLA DE TRANSICIONES\n");
	
	printf("%-15s","Estado");
	tdata aux = a->sig;
	while(aux != NULL){
		if(aux->data != NULL){
			printf("| ");
			print_tdata(aux->data);
			printf("    ");
		}
		aux = aux->next;
	}
	printf("\n--------------------\n");
	tdata b = a->q;
	int i = 0;
	while( b != NULL){
		if( b->data != NULL){
			print_tdata(b->data);
			printf(" ");
			
			int j;
			for(j=0; j< a->cantsim;j++){
				printf("| ");
				if(a->tabla[i][j] ==  NULL){
					printf("{}   ");
					
				}else{
					tdata d= a->tabla[i][j];
					printf("{");
					while(d != NULL){
						print_tdata(d->data);
						if(d->next != NULL)
							printf(",");
						d=d->next;
					}
					printf("}");
				}
			}
			printf("\n");
			
		}
		i++;
		b=b->next;
	}
	printf("\n====================\n");
	
}
int es_afd(automata a){
	if(a==NULL || a->tabla == NULL )
		return 0;
	int i,j;
	for(i=0; i< a->cantest; i++){
		
		for(j = 0; j< a->cantsim;j++){
			if(a->tabla[i][j] != NULL){
				if(length(a->tabla[i][j]) > 1)
					return 0;
			}
		}
	}
	return 1;
}

void liberar(automata a){
	int i, j;
	if(a == NULL)
		return;
	if(a->tabla != NULL){
		for(i=0;i<a->cantest; i++){
			for(j=0;j< a->cantsim; j++){
				if(a->tabla[i][j] != NULL)
					free_tdata(a->tabla[i][j]);
			}
			free(a->tabla[i]);
		}
		free(a->tabla);
	}
	free(a);
}

automata carga_aut(){
	tdata conj_q = NULL, alf = NULL, est_fin = NULL, ini = NULL;
	int n, i;
	
	printf("INICIO");
	printf("¿Cuantos estados son?");
	scanf("%d",&n);
	
	for(i=0;i<n; i++){
		printf("estado %d:",i);
		tdata e=ing();
		insert_set(&conj_q,e);
		if( i == 0 )
			ini = e;
	}
	
	printf("¿Cuantos simbolos?");
	scanf("%d",&n);
	
	for(i=0;i<n; i++){
		printf("simbolo %d:",i);
		tdata c= ing();
		insert_set(&alf,c);
		
	}
	
	printf("¿Cuanto estados son finales?");
	scanf("%d",&n);
	for(i=0;i<n;i++){
		printf("estado final %d",i);
		tdata f = ing();
		
		tdata aux = conj_q;
		int val = 0;
		while(aux != NULL){
			if(igual(aux->data, f))
				val = 1;
			aux = aux->next;
			
		}
		if(val == 1)
			insert_set( &est_fin,f);
		else
		printf("ERROR");
	}
	
	automata A= crea_uno(conj_q, alf, ini, est_fin);
	
	printf("TRANSICIONES");
	printf("ingrese FIN para salir");
	int b = 1;
	while(b ==1){
		printf("estado origen:");
		tdata ori = ing();
		if(ori->string != NULL && strcmp (ori->string,"FIN")==0){
			b = 0;
			free_tdata(ori);
		}else{
			if(indice_esta(A,ori) != -1){
				printf("simbolo:");
				tdata sim = ing();
				printf("estado destino:");
				tdata dest = ing();
				
				if(indice_simb(A,sim) !=-1 && indice_esta(A,dest)!= -1){
					agregar_transi(A,ori,sim,dest);
					printf("Transicion Agregada\n");
				}
				else{
					printf("Error\n");
					free_tdata(sim);
					free_tdata(dest);
				}
			}
			else
			   printf("ERROR\n");
			free_tdata(ori);
		}
	}
	return A;
}
	
int vali_cad(automata a, tdata cad){
	if(a == NULL || cad == NULL)
		return 0;
	tdata c= cad;
	while( c!= NULL){
		if(c->data != NULL){
		
			if(indice_simb(a, c->data) == -1){
				printf("no pertenece al alfabeto cargado\n");
				return 0;
			}
		}
		c=c->next;
	}
	return 1;
}

int pertene_cadena( automata a, tdata cad_list){
	if(a==NULL || a->tabla == NULL || cad_list == NULL)
		return 0;
	if(cad_list ->nodeType != LIST){
		printf("debe ser una cadena");
		return 0;
	}
	
	if(!vali_cad(a,cad_list))
	   return 0;
	
	
	tdata est_act = NULL;
	insert_set(&est_act, a->q0);
	
	tdata c = cad_list;
	while(c!= NULL){
		tdata sim= c->data;
		
		if(sim != NULL){
			
			int col= indice_simb(a,sim);
			tdata prox_est = NULL;
			tdata aux_act = est_act;
			
			while ( aux_act != NULL){
				tdata act = aux_act->data;
				int f = indice_esta(a,act);
				if(f!=-1){
					tdata dest = a->tabla[f][col];
					if( dest != NULL){
						tdata cop_uni = union_set(prox_est, dest);
						if(prox_est != NULL)
							free_tdata(prox_est);
						prox_est = cop_uni;
					}
				}
				aux_act = aux_act->next;
				
			}
			free_tdata(est_act);
			est_act = prox_est;
			if(est_act == NULL)
				return 0;
			
		}
		c=c->next;
	}
	tdata inter = intersection_set(est_act, a->f);
	int acep = (length(inter)>0);
	
	free_tdata(inter);
	free_tdata(est_act);
	
	return acep;
}
tdata renom(int i){
	char buffer[20];
	sprintf(buffer, "p%d", i); // Formatea directamente "p0", "p1", etc.
	
	tdata nuevo = create_str_ast();
	nuevo->string = load2(buffer);
	return nuevo;
}	
	/*
tdata renom(int i){
	char buffer[20];
	buffer[0]= 'p';
	if(i==0){
		buffer[1] = '0';
		buffer[2] = '\0':
		
	}else{
		char inv[15];
		int p_inv = 0;
		int temp = i;
		
		while(temp >0){
			inv[p_inv] = (temp %10) + '0';
			p_inv++;
			temp = temp/10;
		}
		int p_bu
	}
	tdata nuevo = create_str_ast();
	nuevo->string = load2(buffer);
	return nuevo;
}
*/
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
automata convertir(automata afnd){
	if(afnd == NULL)
		return NULL;
	if(es_afd(afnd)){
		printf("El automata ya es un AFD\n");
		return afnd;
	}
	
	int cont_p = 0;
	tdata nue_q = NULL, nue_f = NULL, ini_n = NULL;
	
	tdata conj_i = NULL;
	insert_set(&conj_i, afnd->q0);
	
	ini_n = renom(cont_p++);
	insert_set(&nue_q, ini_n);
	
	tdata l= intersection_set(conj_i, afnd->f);
	if(length(l)>0)
		insert_set(&nue_f,ini_n);
	free_tdata(l);
	
	tdata sub = NULL;
	tdata nom = NULL;
	append(&sub, conj_i);
	append(&nom, ini_n);
	
	tdata sub_p = NULL;
	tdata nom_p = NULL;
	append(&sub_p, conj_i);
	append(&nom_p, ini_n);
	
	automata afd_res = crea_uno(nue_q, afnd->sig, ini_n, nue_f);
	
	while(sub_p != NULL){
		
		tdata sub_a = sub_p->data;
		tdata nom_a = nom_p->data;
		
		tdata aux_sim = afnd->sig;
		while (aux_sim != NULL){
			tdata sim = aux_sim->data;
			tdata conj_t = NULL;
			
			tdata aux_sub = sub_a;
			while(aux_sub != NULL){
				int f = indice_esta(afnd, aux_sub->data);
				int c = indice_simb(afnd, sim);
				if ( f != -1 && c!= -1 && afnd->tabla[f][c] != NULL){
					tdata u = union_set(conj_t, afnd->tabla[f][c]);
					if(conj_t != NULL)
						free_tdata(conj_t);
					conj_t= u;
					
				}
				aux_sub = aux_sub->next;
			}
			if(conj_t != NULL){
				tdata nom_d = bus_nom(sub,nom,conj_t);
				
				if(nom_d == NULL){
					nom_d = renom(cont_p ++);
					
					insert_set(&(afd_res->q), nom_d);
					afd_res->cantest = length(afd_res->q);
					afd_res->tabla = (tdata**)realloc(afd_res->tabla, afd_res->cantest * sizeof(tdata*));
					afd_res->tabla[afd_res->cantest -1] = (tdata*)malloc(afd_res->cantsim * sizeof(tdata));
					int k;
					for(k=0; k< afd_res->cantsim; k++)
						afd_res->tabla[afd_res->cantest -1][k] = NULL;
					tdata inter_f = intersection_set(conj_t, afnd->f);
					if(length(inter_f) >0)
						insert_set(&(afd_res->f), nom_d);
					free_tdata(inter_f);
					
					append(&sub,conj_t);
					append(&nom, nom_d);
					append(&sub_p, conj_t);
					append(&nom_p, nom_d);
				}
				agregar_transi(afd_res, nom_a, sim, nom_d);
			}
			aux_sim = aux_sim->next;
		}
		tdata bor_sub = sub_p;
		tdata bor_nom = nom_p;
		sub_p = sub_p->next;
		nom_p = nom_p->next;
		
		free(bor_sub);
		free(bor_nom);
	}
	
	free_tdata(sub);
	return afd_res;
}
