#include "str.h"

str load2(const char* s) {
    if (s == NULL || *s == '\0')
        return NULL;

    str n = (str)malloc(sizeof(struct charNode));
    n->letra = *s;
    n->next = load2(s + 1);

    return n;
}

void print_str(str s) {
    str aux = s;
    while (aux != NULL) {
        printf("%c", aux->letra);
        aux = aux->next;
    }
}

int igual_str(str s1, str s2) {
    if (s1 == NULL && s2 == NULL)
        return 1;
    if (s1 == NULL || s2 == NULL || s1->letra != s2->letra)
        return 0;

    return igual_str(s1->next, s2->next);
}

str copy_str(str s) {
    if (s == NULL)
        return NULL;
    str n = (str)malloc(sizeof(struct charNode));
    n->letra = s->letra;
    n->next = copy_str(s->next);
    return n;
}

void free_str(str s) {
    if (s == NULL)
        return;
    free_str(s->next);
    free(s);
}
