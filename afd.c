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
		
automata crea_uno(tdata conj_q, tdata alfa_sig, int ini, tdata conj_f){
	
	automata A =(automata)malloc(sizeof(Automata));
	if( A ==NULL)
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
	if( a == NULL || a->Q == NULL || estado == NULL)
		return -1;
	tdata aux = a->Q;
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
	
	if(a == NULL || a->Sigma == NULL || simbolo == NULL)
		return -1;
	tdata aux = a->Sigma;
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


void agregar_transi(automata A,int from, int symbol, tdata est_dest){
	
	if(A==NULL || est_dest == NULL)
		return;
	int i;
	for(i=0;i<A->deltaSize;i++){//puedo utilizar el return o tengo que mandarle una bandera
		if(A->delta[i].from == from && A->delta[i].symbol == symbol){
			
			tdata copia_dest = clone(est_dest);
			tdata aux = copia_dest;
			while(aux != NULL){
				if(aux->data != NULL)
					insert_set(&(A->delta[i].to),aux->data);
				aux=aux->next;
			}
			free_tdata(copia_dest);
			return;
		}
	}
	
	A->deltaSize++;
	A->delta = (TransitionEntry*)malloc(A->delta, A->deltaSize* sizeof(TransitionEntry));
	
	A->delta[A->deltaSize-1].from = from;
	A->delta[A->deltaSize - 1].symbol = symbol;
	A->delta[A->deltaSize - 1].to = clone(est_dest);
	
	
}

void mostrar(automata a){
	if( a == NULL)
		return ;
	printf("\n TABLA DE TRANSICIONES\n");
	
	printf("%-15s","Estado");
	tdata aux = a->Sigma;
	while(aux != NULL){
		if(aux->data != NULL){
			printf("| ");
			print_tdata(aux->data);
			printf("    ");
		}
		aux = aux->next;
	}
	printf("\n--------------------\n");
	tdata b = a->Q;
	int i = 0;
	while( b != NULL){
		if( b->data != NULL){
			print_tdata(b->data);
			printf(" ");
			
			tdata aux_sim = a->Sigma;
			int j=0;
			while(aux_sim != NULL){
				printf("| ");
				tdata cel_dest = NULL;
				int k=0, ban=0;
				while(k< a->deltaSize && b==0){
					if(a->delta[k].from == i&& a->delta[k].symbol == j){
						cel_dest = a->delta[k].to;
						ban=1;
					}
					k++;
				}
				if(cel_dest == NULL)
					  printf("{}   ");
				else {
						  print_tdata(cel_dest);
						  printf("   ");
				}
			  j++;
			  aux_sim =aux_sim->next;
			}
			printf("\n");
			
		}
		i++;
		b=b->next;
	}
	printf("\n====================\n");
	
}
int es_afd(automata a){
	if(a==NULL)
		return 0;
	int i;
	for(i=0; i< a->deltaSize; i++){
		
		if(a->delta[i].to != NULL){
			if(length(a->delta[i].to) > 1)
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
		for(i=0;i<a->deltaSize; i++){
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
automata carga_aut(){
	tdata conj_q = NULL, alf = NULL, est_fin = NULL;
	int n, i;
	
	printf("INICIO");
	printf("¿Cuantos estados son?\n");
	scanf("%d",&n);
	getchar();
	
	for(i=0;i<n; i++){
		printf("estado %d:\n",i);
		tdata e=ing();
		insert_set(&conj_q,e);
		free_tdata(e);
	}
	
	printf("¿Cuantos simbolos?\n");
	scanf("%d",&n);
	getchar();
	
	for(i=0;i<n; i++){
		printf("simbolo %d:\n",i);
		tdata c= ing();
		insert_set(&alf,c);
		free_tdata(c);
		
	}
	
	printf("¿Cuanto estados son finales?\n");
	scanf("%d",&n);
	getchar();
	for(i=0;i<n;i++){
		printf("estado final %d\n",i);
		tdata f = ing();
		
		tdata aux = conj_q;
		int val = 0;
		while(aux != NULL && val == 0){
			if(aux->data !=NULL && igual(aux->data, f))
				val = 1;
			aux = aux->next;
			
		}
		if(val == 1)
			    insert_set(&est_fin,f);
		else
			printf("ERROR 1\n");
		free_tdata(f);
	}
	
	automata A= crea_uno(conj_q, alf, 0, est_fin);
	
	printf("TRANSICIONES\n");
	printf("ingrese FIN para salir\n");
	int b = 0;
	while(b ==0){
		printf("estado origen:\n");
		tdata ori = ing();
		if(ori->string != NULL && strcmp (ori->string,"FIN\n")==0){
			b = 1;
			free_tdata(ori);
		}else{
			int f = indice_esta(A,ori);
			if(f !=-1){
				printf("simbolo:\n");
				tdata sim = ing();
				printf("estado destino:\n");
				tdata dest = ing();
				int c=indice_simb(A,sim);
				int d = indice_esta(A,dest);
				if(c!=-1 && d!=-1){
					tdata des = NULL;
					insert_set(&des,dest);
					agregar_transi(A,f,c,des);
					printf("Transicion Agregada\n");
					free_tdata(des);
				}
				else{
					printf("Error2\n");
				}
				free_tdata(sim);
				free_tdata(dest);
			}
			else
				printf("ERROR3\n");
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
				printf("no pertenece al alfabeto \n");
				return 0;
			}
		}
		c=c->next;
	}
	return 1;
}
											
int pertene_cadena( automata a, tdata cad_list){
	if(a==NULL || cad_list == NULL)
		return 0;
	if(cad_list ->nodeType != LIST){
		printf("debe ser una lista\n");
		return 0;
	}
	
	if(!vali_cad(a,cad_list))
	      return 0;
	
	
	tdata est_act = NULL;
	tdata aux = a->Q;
	int i =0;
	
	while(aux !=NULL && i != a->q0){
		i++;
		aux = aux->next;
	}
	if(aux != NULL)
		  insert_set(&est_act, aux->data);
	tdata c =cad_list;
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
					int k=0;
					int enc = 0;
					while(k< a->deltaSize && enc ==0){
						if(a->delta[k].from == f && a->delta[k].symbol == col){
							tdata cop_uni = union_set(prox_est, a->delta[k].to);
							if(prox_est != NULL)
								free_tdata(prox_est);
							prox_est = cop_uni;
							enc = 1;
						}
						k++;
						
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
	tdata inter = intersection_set(est_act, a->F);
	int acep = (length(inter)>0);
	
	free_tdata(inter);
	free_tdata(est_act);
	
	return acep;
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
	int i =0;
	while(aux != NULL && i != afnd->q0){
		i++;
		aux = aux->next;
	}
	if(aux != NULL)
		  insert_set(&conj_i, aux->data);
	
	tdata ini_n = renom(cont_p++);
	insert_set(&nue_q, ini_n);
	
	tdata l= intersection_set(conj_i, afnd->F);
	if(length(l)>0)
		insert_set(&nue_f,ini_n);
	free_tdata(l);
	
	tdata sub = NULL, nom = NULL;
	append(&sub, conj_i);
	append(&nom, ini_n);
	
	tdata sub_p = NULL;
	tdata nom_p = NULL;
	append(&sub_p, conj_i);
	append(&nom_p, ini_n);
	
	automata afd_res = crea_uno(nue_q, afnd->Sigma, 0, nue_f);
	
	while(sub_p != NULL){
		
		tdata sub_a = sub_p->data;
		tdata nom_a = nom_p->data;
		
		tdata aux_sim = afnd->Sigma;
		int le=0;
		while (aux_sim != NULL){
			tdata sim = aux_sim->data;
			tdata conj_t = NULL;
			
			tdata aux_sub = sub_a;
			while(aux_sub != NULL){
				int f = indice_esta(afnd, aux_sub->data);
				if(f!= -1){
					int k=0;
					int enc =0;
					while(k<afnd->deltaSize && enc ==0){
						if(afnd->delta[k].from == f && afnd->delta[k].symbol == le){
							tdata u = union_set(conj_t, afnd->delta[k].to);
							if(conj_t != NULL)
								free_tdata(conj_t);
							conj_t= u;
							enc =1;
						}
						k++;
					}
				}
				aux_sub = aux_sub->next;
			}
			if(conj_t != NULL){
				tdata nom_d = bus_nom(sub,nom,conj_t);
				
				if(nom_d == NULL){
					nom_d = renom(cont_p ++);
					
					insert_set(&(afd_res->Q), nom_d);
					tdata inter_f = intersection_set(conj_t,afnd->F);
					if(length(inter_f)>0)
						insert_set(&(afd_res->F), nom_d);
					free_tdata(inter_f);
					
					append(&sub,conj_t);
					append(&nom, nom_d);
					append(&sub_p, conj_t);
					append(&nom_p, nom_d);
				}
				int f_res =indice_esta(afd_res, nom_a);
				agregar_transi(afd_res, f_res, le, conj_t);
			}
			le++;
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
