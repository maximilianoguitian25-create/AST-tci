#ifndef AFD_H
#define AFD_H

#include "tdata.h"
#include "str.h"

typedef sruct {
	tdata q;
	tdata sig;
	tdata** tabla;
	int cantest;
	int cantsim;
	tdata q0;
	tdata f;
	
}automataNode;

typedef struct automataNode* automata;

automata crea_uno(tdata, tdata, tdata, tdata);
void agregar_transi(automata, tdata, tdata , tdata);
void mostrar(automata);
void pertene_cadena(automata, tdata );
int es_afd(automata);
void liberar(automata);

int indice_esta(automata, tdata);
int indice_simb(automata, tdata);

automata carga_aut();
int vali_cad(automata, tdata);
automata convertir(automata );

#endif
