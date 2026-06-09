
#ifndef AFD_H
#define AFD_H

#include "tdata.h"
#include "str.h"

typedef struct {
	int from;
	int symbol;
	tdata to;
} TransitionEntry;
typedef struct {
	tdata Q;
	tdata Sigma;
	TransitionEntry* delta;
	int deltaSize;
	int q0;
	tdata F;
} Automata;


typedef Automata* automata; 

automata crea_uno(tdata conj_q, tdata alfa_sig, int ini, tdata conj_f);
void agregar_transi(automata a, int from, int symbol, tdata est_dest);
void mostrar(automata a);
int pertene_cadena(automata a, tdata cad_list); 
int es_afd(automata a);
void liberar(automata a);
int indice_esta(automata a, tdata estado);
int indice_simb(automata a, tdata simbolo);
automata carga_aut();
int vali_cad(automata a, tdata cad);
automata convertir(automata afnd);

#endif
