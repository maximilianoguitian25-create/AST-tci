#include <stdlib.h>
#include <stdio.h>
#include "str.h"

#define STR 1
#define SET 2
#define LIST 3

struct dataType{
	int nodeType;   /* STR, SET, LIST */
	union{
		str string;
		struct{
			struct dataType* data;
			struct dataType* next;
		};
	};
};

typedef struct dataType* tdata;



tdata create_str_ast();
tdata create_list();
tdata create_set();



tdata clone(tdata);
int igual(tdata, tdata);
void append(tdata*, tdata );
int length(tdata);
void print_tdata(tdata );


int belongs(tdata, tdata);
void insert_set(tdata*, tdata);
void remove_set(tdata*, tdata);
tdata union_set(tdata, tdata);
tdata intersection_set(tdata, tdata);
tdata difference_set(tdata, tdata);
int subset(tdata, tdata);
tdata pro_cate(tdata, tdata);

tdata conc_str(tdata, tdata);
tdata str_to_list(tdata);
tdata list_to_str(tdata);

#endif
