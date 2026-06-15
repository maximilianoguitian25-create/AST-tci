#ifndef STR_H
#define STR_H

#include <stdio.h>
#include <stdlib.h>

struct charNode {
    char letra;
    struct charNode* next;
};
typedef struct charNode* str;

str load2(const char* s);
void print_str(str s);
int igual_str(str s1, str s2);
str copy_str(str s);
void free_str(str s);

#endif
