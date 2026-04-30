
#include <stdio.h>
#include "tdata.h"
#include "str.h"


int main(){
	printf("------CARGA DE DATOS------\n");
	
	tdata sA = create_str_ast();
	sA->string = load2("HOLA");
	
	tdata sB = create_str_ast();
	sB->string = load2("LEO");
	
	tdata sC = create_str_ast();
	sC->string = load2("PROGRA");
	
	printf("cadenas creadas\n");
	print_tdata(sA);
	printf("\n");
	print_tdata(sB);
	printf("\n");
	print_tdata(sC);
	printf("\n");
	
	
	// conjunto vacio 
	tdata c1 = NULL;
	tdata c2 = NULL;
	tdata c3 = NULL;
	
	printf("debe aparecer{}:");
	print_tdata(c1);
	printf("\n");

	insert_set(&c2,sA);
	insert_set(&c2,sB);
	insert_set(&c2,sA);

	printf("Conjunto C2 (debe tener A y B sin repetir): ");
	print_tdata(c2);
	printf("\n");
	// ver si sale duplicado el a para ver si funciona 
	
	insert_set(&c3,sB);
	insert_set(&c3,sC);
	
	printf("Conjunto C3 (B, C): ");
	print_tdata(c3);
	printf("\n");
	
	
	printf("UNION c2 y c3");
	tdata uni = union_set(c2, c3);
	print_tdata(uni);
	printf("\n");
	
	printf("INTERSECCION c2 y c3");
	tdata inter = intersection_set(c2,c3);
	print_tdata(inter);
	printf("\n");
	
	tdata pc = pro_cate(c2, c3);
	printf("C2:");
	print_tdata(c2);
	printf("\n");
	printf("C2:");
	print_tdata(c3);
	printf("\n");
	printf("Producto Cartesiano\n");
	print_tdata(pc);
	printf("\n");
	
	printf("LISTAS ANIDADAS\n");//preguntar al profe si a esto se refiere con lista anidadas 
	tdata list = create_list();
	tdata sub = create_list();
	
	append(&sub,sA);
	append(&sub,sB);
	
	append(&list,sub);
	append(&list,sC);
	print_tdata(list);
	printf("\n");
	
	printf("CONVERSIONES\n");
	tdata palabra = create_str_ast();
	palabra->string =load2("Profe Apruebeme");
	printf("\n");
	printf("ORIGINAL(STR):\n");
	print_tdata(palabra);
	printf("\n");
	tdata conv = str_to_list(palabra);
	printf("CONVERTIDA A  LISTA:\n");
	print_tdata(conv);
	printf("\n");
	tdata vuel = list_to_str(conv);
	printf("CONVERTIDA A STR:\n");
	print_tdata(vuel);
	printf("\n");
	
	printf("COPIAS VS REFERENCIAS\n");
	tdata ori = create_set();
	insert_set(&ori, sA);
	
	tdata copia = clone(ori);
	insert_set(&copia, sB);
	
	printf("Original:\n");// solo debe aparecer A
	print_tdata(ori);
	printf("\n");
	printf("Copia:\n");// debe tener a y b
	print_tdata(copia);
	printf("\n");
	
	printf("funciones para saber si funcionan\n");
	// agregar funciones que falta 
	tdata diff = difference_set(c2,c3);
	printf("Diferencia entre c2 y c3");
	print_tdata(diff);
	printf("\n");
	
	int b=subset(c2,uni);
	
	printf("c2 es subconjunto de la union:\n"); 
	sino(b);
	
	b=equals_set(c2,c3);
	printf("c2 y c3 son conjuntos iguales:\n");
	sino(b);
	
	b=length(list);
	printf("longitud de la lista: %d\n", b);
	b= search(list,sC);

	printf("esta PROGRA en la lista:\n");
	sino(b);
	printf("antes de eliminar HOLA:\n");
	print_tdata(c2);
	printf("\n");
	remove_set(&c2, sA);
	printf("despues  de eliminar HOLA:\n");
	print_tdata(c2);
	printf("\n");
	
	tdata l1 = create_list(); 
	append(&l1, sA);
	tdata l2 = create_list(); 
	append(&l2, sB);
	tdata conca = concaternar_lista(l1, l2);
	printf("Lista concatenada: "); 
	print_tdata(conca); 
	printf("\n");
	
	
	// preguntar al profe 
	free_tdata(sA);
	free_tdata(sB);
	free_tdata(sC);
	free_tdata(c2);
	free_tdata(c3);
	free_tdata(uni);
	free_tdata(inter);
	free_tdata(pc);
	free_tdata(list);
	free_tdata(sub);
	free_tdata(palabra);
	free_tdata(conv);
	free_tdata(vuel);
	free_tdata(ori);
	free_tdata(copia);
	free_tdata(diff);
	free_tdata(l1);
	free_tdata(l2);
	free_tdata(conca);

	
	return 0;
	
}
