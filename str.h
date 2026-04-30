#ifndef STR_H
#define STR_H

#include <stdio.h>
#include <stdlib.h>

struct charNode {
	char letra;
	struct charNode* next;
};
typedef struct charNode* str;

str load2(const char*  );
void print_str(str );
int igual_str(str, str);
str copy_str(str );
#endif
