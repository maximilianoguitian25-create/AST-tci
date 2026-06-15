#include "tdata.h"

tdata create_str_ast(){
    tdata n = (tdata)malloc(sizeof(struct dataType));
    if(n==NULL)
        return NULL;
    n->nodeType = STR;
    n->string = NULL;
    
    return n;
}

tdata create_set(){
    tdata n= (tdata)malloc(sizeof(struct dataType));
    
    if( n == NULL)
        return NULL;
    
    n->nodeType = SET;
    n->data = NULL;
    n->next = NULL;
    
    return n;
}
    
tdata create_list(){
    tdata n = (tdata)malloc(sizeof(struct dataType));
    
    if(n == NULL)
        return NULL;
    n->nodeType = LIST;
    
    n->data = NULL;
    n->next = NULL;

    return n;
}

void append(tdata *a, tdata e){
    tdata nuevo = create_list();
    nuevo->data = clone(e);
    nuevo->next = NULL;
    
    if(*a == NULL){
        *a = nuevo;
        
    }else{
        tdata aux = *a;
        if(aux->data == NULL){
            free_tdata(nuevo);
            aux->data = clone(e);
        }else{
            while(aux->next != NULL){
                aux = aux->next;
            }
            aux->next = nuevo;
        }
    }
}

tdata clone(tdata n){
    if(n == NULL)
        return NULL;
    tdata nuevo = NULL;
    
    if(n->nodeType == STR){
        nuevo = create_str_ast();
        nuevo->string = copy_str(n->string);
    }
    else{
        if(n->nodeType == SET || n->nodeType == LIST){
            tdata aux = n;
            tdata cab = NULL;
            tdata col = NULL;
            
            while( aux!= NULL){
                
                if (aux->data == NULL && cab == NULL) {
                    return (n->nodeType == SET) ? create_set() : create_list();
                }
                
                tdata nodo_lista = (n->nodeType == SET)? create_set(): create_list();
                nodo_lista->data = clone(aux->data);
                nodo_lista->next = NULL;
                
                if(cab == NULL){
                    cab = nodo_lista;
                    col = nodo_lista;
                }else{
                    col->next = nodo_lista;
                    col = nodo_lista;
                }
                aux = aux->next;
            }
        return cab;
        }
    }
    return nuevo;
}
    
int belongs(tdata set, tdata elem){
    tdata aux = set;
    while(aux != NULL){
        if( igual(aux->data,elem))
            return 1;
        
        aux = aux->next;
    }
    return 0;
}

int igual(tdata a, tdata b){
    if(a == b) return 1;
    if(a == NULL && b == NULL) return 1; 
    if(a == NULL || b == NULL) return 0; 
    if(a->nodeType != b->nodeType) return 0;
    
    if(a->nodeType == STR){
        return igual_str(a->string, b->string);
    }
    return igual(a->data, b->data) && igual(a->next, b->next);
}

void insert_set(tdata* set, tdata elem){
    if (elem == NULL || belongs(*set,elem))
        return;
    
    tdata n = create_set();
    if(n!= NULL){
        n->data = clone(elem);
        n->next = NULL;
        if(*set == NULL) {
            *set = n;
        } else {
            tdata aux = *set;
            while(aux->next != NULL) {
                aux = aux->next;
            }
            aux->next = n;
        }
    }
}

tdata union_set(tdata a, tdata b){
    tdata uni = NULL;
    tdata aux = a;
    
    while(aux != NULL){
        if (aux->data != NULL)
            insert_set(&uni,aux->data);
        aux = aux->next;
    }
    aux = b;
    while( aux!=NULL){
        if(aux->data != NULL)
            insert_set(&uni,aux->data);
        aux = aux->next;
    }
    return uni;
}

tdata intersection_set(tdata a, tdata b){
    tdata res = NULL;
    tdata aux = a;
    
    while(aux!= NULL){
        if(belongs(b, aux->data)==1){
            insert_set(&res, aux->data);
        }
        aux = aux->next;
    }
    
    return res;
}
            
tdata difference_set(tdata a, tdata b){
    tdata res = NULL;
    tdata aux = a;
    
    while(aux!=NULL){
        if (!belongs(b,aux->data))
            insert_set(&res, aux->data);
        aux = aux->next;
    }
    return res;
}

tdata pro_cate(tdata a, tdata b){
    tdata resu = NULL;
    tdata auxa = a;
    tdata auxb;
    
    while(auxa != NULL){
            auxb = b;
            while(auxb != NULL){
                    tdata par = create_list();
                    
                    append(&par, auxa->data);
                    append(&par, auxb->data);
                    insert_set(&resu, par);
                    
                    free_tdata(par);
                    auxb = auxb->next;
                }
            auxa = auxa->next;
    }
    return resu;    
}

int length(tdata list){
    int c=0;
    tdata aux=list;
    while(aux!=NULL){
        c++;
        aux=aux->next;
    }
    return c;
}
        
int search(tdata list, tdata elem){
    tdata aux = list;
    
    while(aux!=NULL){
        if(igual(aux->data,elem)){ 
            return 1;
        }
        aux=aux->next;
    }
    return 0;
}

int subset(tdata a, tdata b){
    tdata aux = a;
    while(aux!=NULL){
        if(!belongs(b, aux->data))
            return 0;
        
        aux = aux->next;
    }
    return 1;
}

int equals_set(tdata a, tdata b){
    return subset(a,b) && subset(b,a);
}

void print_tdata(tdata n){
    if(n == NULL){
        printf("{}"); 
        return;
    }
    
    if(n->nodeType == STR)
       print_str(n->string);
    
    else {
        char open = (n->nodeType == LIST)? '[' : '{';
        char close = (n->nodeType == LIST)? ']' : '}';
        
        printf("%c", open);
        tdata aux = n;
        while(aux != NULL){
            if (aux->data != NULL) 
                print_tdata(aux->data);
            
            if(aux->next != NULL && aux->next->data != NULL)
                printf(",");
            aux = aux->next;
        }
        printf("%c", close);
    }
}
    
void remove_set(tdata* set, tdata elem){
    tdata aux = *set;
    tdata prev = NULL;
    
    while(aux!= NULL){
        if(igual(aux->data,elem)){
            if(prev == NULL){
                *set = aux->next;
            }
            else{
                prev->next = aux->next; 
            }
            free_tdata(aux->data);
            aux->next= NULL;
            free(aux);
            return;
        }
        prev = aux;
        aux = aux->next;
    }
}

tdata concaternar_lista(tdata l1, tdata l2){
    tdata con = NULL;
    tdata aux = l1;
    
    while(aux != NULL){
        if( aux->data != NULL)
            append(&con, aux->data);
        aux = aux->next;
    }
    aux = l2;
    while(aux != NULL){
        if( aux->data != NULL)
            append(&con, aux->data);
        aux = aux->next;
    }
    return con;
}
    
tdata list_to_str(tdata lista){
    if(lista == NULL || lista->nodeType != LIST)
        return NULL;
    
    tdata res = create_str_ast();
    res->string = NULL;
    
    tdata aux = lista;
    str u=NULL;
    
    while(aux != NULL){
        if(aux->data != NULL && aux->data->nodeType == STR){
            str copia = copy_str(aux->data->string);
            
            if(res->string == NULL)
                res->string = copia;
            else {
                u = res->string;
                while(u->next != NULL){
                    u = u->next;
                }
                u->next = copia;
            }
        }
        aux =aux->next;
    }
    return res;
}
    
tdata str_to_list(tdata s){
    if(s == NULL || s->nodeType != STR)
        return NULL;
    
    tdata res = NULL;
    str aux = s->string;
    
    while(aux != NULL){
        tdata l= create_str_ast();
        
        str nodo =(str)malloc(sizeof (struct charNode));
        nodo->letra = aux->letra;
        nodo->next = NULL;
        
        l->string = nodo;
        
        append(&res,l);
        free_tdata(l);
        
        aux =aux->next;
    }
    return res;
}
    
void free_tdata(tdata n){
    if(n == NULL) return;
    
    if(n->nodeType == STR) {
        if(n->string != NULL) free_str(n->string);
    } else {
        if(n->data != NULL) free_tdata(n->data);
        if(n->next != NULL) free_tdata(n->next);
    }
    free(n);
}

void sino(int b){
    if(b==1)
        printf("si\n");
    else 
        printf("no\n");
    
    return;
}
