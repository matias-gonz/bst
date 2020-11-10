#include"test.h"
#include"abb.h"
#include<stdbool.h>
#include<stdio.h>

static const int EXITO = 0;
static const int PRIMERO_MAYOR = 1;
static const int SEGUNDO_MAYOR = -1;


/*Pre: Los punteros recibidos deben ser punteros a int
 *Post: Devuelve 0 si los enteros son iguales; 1 si el primero es mayor, -1 si el segundo es mayor;
*/
int comparador_enteros(void* primero, void* segundo){
	int mayor = *(int*)primero - *(int*)segundo;
	if (mayor > 0) return PRIMERO_MAYOR;
	if (mayor < 0) return SEGUNDO_MAYOR;
	return mayor;
}

void destructor_simple(void* elemento){
	free(elemento);
}

/*Pre: Los punteros recibidos deben ser punteros a int
 *Post: Le suma el valor del elemento a la suma si es que la suma no es mayor a 100
 *luego imprime el valor de la suma y devuelve false
 *Si suma es mayor a 100, devuelve true
*/
bool suma_int_hasta_100(void* elemento, void* suma){
	if(*(int*)suma > 100){
		printf("\n");
		return true;
	}
	*(int*)suma += *(int*)elemento;
	printf(">>%i ",*(int*)suma);
	return false;
}

/*Pre: Array debe ser un vector de int*
 *Tope debe ser menor o igual a la cantidad de enteros/punteros que array y correctos puedan almacenar
 *Post: devuelve true si todos los elementos(hasta el tope) de correctos y array son iguales, sino devuelve false
*/
bool elementos_correctos_int(int* correctos, void** array, int tope){
	bool son_correctos = true;
	for(int i = 0; i < tope;i++){
		if(correctos[i] != *(int*)array[i]){
			son_correctos = false;
		}
	}

	return son_correctos;
}


/*||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
 *||													||
 *||				      FUNCIONES ESPECIFICAS DE CREAR:					||
 *||													||
 *||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
*/

//Se testeara la creacion del arbol, que la inicializacion de este sea correcta
void probar_crear_comparador_destructor_validos(stats_t* stats, abb_comparador comparador, abb_liberar_elemento destructor){
	abb_t* arbol = arbol_crear(comparador,destructor);
	test_warn("Si el arbol no se crea correctamente, algunas pruebas no se ejecutaran");
	if(!test_check(stats,"Arbol se crea con comparador y destructor validos",(arbol != NULL))) return;
	test_check(stats,"Arbol contiene comparador correcto",(arbol->comparador == comparador));
	test_check(stats,"Arbol contiene destructor correcto",(arbol->destructor == destructor));
	test_check(stats,"Arbol se crea con nodo raiz NULL",(arbol->nodo_raiz == NULL));
	test_check(stats,"Funcion arbol vacio es true",(arbol_vacio(arbol)));
	test_check(stats,"Funcion arbol raiz devuelve NULL",(arbol_raiz(arbol) == NULL));
	arbol_destruir(arbol);
}

//Se testearan casos donde el arbol no deba ser creado
void probar_crear_invalido(stats_t* stats){
	test_anunciar_prueba("Prueba de crear arbol con parametros incorrectos");
	abb_t* arbol = arbol_crear(NULL,NULL);
	test_check(stats,"No se puede crear arbol con destructor y comparador NULL",(arbol == NULL));
	arbol = arbol_crear(NULL,&destructor_simple);
	test_check(stats,"No se puede crear arbol con comparador NULL y destructor valido",(arbol == NULL));
	test_check(stats,"Funcion arbol vacio es true",(arbol_vacio(arbol)));
	test_check(stats,"Funcion arbol raiz devuelve NULL",(arbol_raiz(arbol) == NULL));
}

/*||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
 *||													||
 *||				   FUNCIONES ESPECIFICAS DE INSERTAR:					||
 *||													||
 *||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
*/

//Se testeara la insercion en un arbol que esta vacio
void probar_insertar_arbol_vacio(stats_t* stats){
	test_anunciar_prueba("Prueba de insertar un elemento en arbol vacio CAJA NEGRA");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	int a = 15;
	test_check(stats,"Se puede insertar en un arbol vacio",(arbol_insertar(arbol,&a) == EXITO));
	test_check(stats,"Arbol no esta vacio",(!arbol_vacio(arbol)));
	test_check(stats,"Funcion nodo raiz devuelve elemento correcto",(*(int*)arbol_raiz(arbol) == a));
	test_check(stats,"Funcion buscar devuelve elemento correcto",(*(int*)arbol_buscar(arbol,&a) == a));

	arbol_destruir(arbol);
}

//Misma prueba que anterior pero caja blanca
void probar_insertar_arbol_vacio_caja_blanca(stats_t* stats){
	test_anunciar_prueba("Prueba de insertar un elemento en arbol vacio CAJA BLANCA");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	int a = 15;
	arbol_insertar(arbol,&a);
	test_check(stats,"Nodo raiz no es NULL",(arbol->nodo_raiz != NULL));
	test_check(stats,"Nodo raiz no tiene hijo izquierdo",(!arbol->nodo_raiz->izquierda));
	test_check(stats,"Nodo raiz no tiene hijo derecho",(!arbol->nodo_raiz->derecha));
	test_check(stats,"Elemento en nodo raiz es correcto",(*(int*)arbol->nodo_raiz->elemento == a));

	arbol_destruir(arbol);
}

//Se testeara la insercion en un arbol donde los elementos siempre seran mayores (de menor a mayor)
void probar_insertar_arbol_multiples_elementos_mayores(stats_t* stats){
	test_anunciar_prueba("Prueba de insertar multiples elementos de menor a mayor CAJA NEGRA");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	int a = 15, b = 20, c = 30, d = 40;
	test_check(stats,"Se puede insertar primer elemento",(arbol_insertar(arbol,&a) == EXITO));
	test_check(stats,"Se puede insertar segundo elemento",(arbol_insertar(arbol,&b) == EXITO));
	test_check(stats,"Se puede insertar tercer elemento",(arbol_insertar(arbol,&c) == EXITO));
	test_check(stats,"Se puede insertar cuarto elemento",(arbol_insertar(arbol,&d) == EXITO));
	test_check(stats,"Arbol no esta vacio",(!arbol_vacio(arbol)));
	test_check(stats,"Funcion nodo raiz devuelve elemento correcto",(*(int*)arbol_raiz(arbol) == a));
	test_check(stats,"Funcion buscar elemento raiz devuelve elemento correcto",(*(int*)arbol_buscar(arbol,&a) == a));
	test_check(stats,"Funcion buscar segundo elemento elemento correcto",(*(int*)arbol_buscar(arbol,&b) == b));
	test_check(stats,"Funcion buscar tercer elemento elemento correcto",(*(int*)arbol_buscar(arbol,&c) == c));
	test_check(stats,"Funcion buscar cuarto elemento elemento correcto",(*(int*)arbol_buscar(arbol,&d) == d));
	
	arbol_destruir(arbol);
}

//Misma prueba que anterior pero caja blanca
void probar_insertar_arbol_multiples_elementos_mayores_caja_blanca(stats_t* stats){
	test_anunciar_prueba("Prueba de insertar multiples elementos de menor a mayor CAJA BLANCA");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	nodo_abb_t* nodo_aux;
	int a = 15, b = 20, c = 30, d = 40;
	arbol_insertar(arbol,&a);
	arbol_insertar(arbol,&b);
	arbol_insertar(arbol,&c);
	arbol_insertar(arbol,&d);

	test_check(stats,"Nodo raiz no es NULL",(arbol->nodo_raiz != NULL));
	test_check(stats,"Nodo raiz no tiene hijo izquierdo",(!arbol->nodo_raiz->izquierda));
	test_check(stats,"Nodo raiz tiene hijo derecho",(arbol->nodo_raiz->derecha));
	test_check(stats,"Elemento en nodo raiz es correcto",(*(int*)arbol->nodo_raiz->elemento == a));
	
	nodo_aux = arbol->nodo_raiz->derecha;	
	test_check(stats,"Segundo nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Segundo Nodo tiene hijo derecho",(nodo_aux->derecha));
	test_check(stats,"Elemento en segundo nodo es correcto",(*(int*)nodo_aux->elemento == b));

	nodo_aux = nodo_aux->derecha;	
	test_check(stats,"Tercer nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Tercer Nodo tiene hijo derecho",(nodo_aux->derecha));
	test_check(stats,"Elemento en tercer nodo es correcto",(*(int*)nodo_aux->elemento == c));
	
	nodo_aux = nodo_aux->derecha;	
	test_check(stats,"Cuarto nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Cuarto Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en cuarto nodo es correcto",(*(int*)nodo_aux->elemento == d));

	arbol_destruir(arbol);
}

//Se testeara la insercion en un arbol donde los elementos siempre seran menores (de mayor a menor)
void probar_insertar_arbol_multiples_elementos_menores(stats_t* stats){
	test_anunciar_prueba("Prueba de insertar multiples elementos de mayor a menor CAJA NEGRA");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	int a = 40, b = 30, c = 20, d = 10;
	test_check(stats,"Se puede insertar primer elemento",(arbol_insertar(arbol,&a) == EXITO));
	test_check(stats,"Se puede insertar segundo elemento",(arbol_insertar(arbol,&b) == EXITO));
	test_check(stats,"Se puede insertar tercer elemento",(arbol_insertar(arbol,&c) == EXITO));
	test_check(stats,"Se puede insertar cuarto elemento",(arbol_insertar(arbol,&d) == EXITO));
	test_check(stats,"Arbol no esta vacio",(!arbol_vacio(arbol)));
	test_check(stats,"Funcion nodo raiz devuelve elemento correcto",(*(int*)arbol_raiz(arbol) == a));
	test_check(stats,"Funcion buscar elemento raiz devuelve elemento correcto",(*(int*)arbol_buscar(arbol,&a) == a));
	test_check(stats,"Funcion buscar segundo elemento devuelve elemento correcto",(*(int*)arbol_buscar(arbol,&b) == b));
	test_check(stats,"Funcion buscar tercer elemento devuelve elemento correcto",(*(int*)arbol_buscar(arbol,&c) == c));
	test_check(stats,"Funcion buscar cuarto elemento devuelve elemento correcto",(*(int*)arbol_buscar(arbol,&d) == d));

	arbol_destruir(arbol);

}

//Misma prueba que anterior pero caja blanca
void probar_insertar_arbol_multiples_elementos_menores_caja_blanca(stats_t* stats){
	test_anunciar_prueba("Prueba de insertar multiples elementos de mayor a menor CAJA BLANCA");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	nodo_abb_t* nodo_aux;
	int a = 40, b = 30, c = 20, d = 10;
	arbol_insertar(arbol,&a);
	arbol_insertar(arbol,&b);
	arbol_insertar(arbol,&c);
	arbol_insertar(arbol,&d);

	test_check(stats,"Nodo raiz no es NULL",(arbol->nodo_raiz != NULL));
	test_check(stats,"Nodo raiz no tiene hijo derecho",(!arbol->nodo_raiz->derecha));
	test_check(stats,"Nodo raiz tiene hijo izquierdo",(arbol->nodo_raiz->izquierda));
	test_check(stats,"Elemento en nodo raiz es correcto",(*(int*)arbol->nodo_raiz->elemento == a));

	nodo_aux = arbol->nodo_raiz->izquierda;	
	test_check(stats,"Segundo nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Segundo Nodo tiene hijo izquierdo",(nodo_aux->izquierda));
	test_check(stats,"Elemento en segundo nodo es correcto",(*(int*)nodo_aux->elemento == b));

	nodo_aux = nodo_aux->izquierda;	
	test_check(stats,"Tercer nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Tercer Nodo tiene hijo izquierdo",(nodo_aux->izquierda));
	test_check(stats,"Elemento en tercer nodo es correcto",(*(int*)nodo_aux->elemento == c));
	
	nodo_aux = nodo_aux->izquierda;	
	test_check(stats,"Cuarto nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Cuarto Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en cuarto nodo es correcto",(*(int*)nodo_aux->elemento == d));

	arbol_destruir(arbol);
}

//Se testeara la insercion en un arbol donde este quedara balanceado
void probar_insertar_arbol_multiples_elementos_mezclados(stats_t* stats){
	test_anunciar_prueba("Prueba de insertar multiples elementos desordenados CAJA NEGRA");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	int a = 50, b = 20, c = 30, d = 60, e = 70, f = 15 , g = 55;
	test_check(stats,"Se puede insertar primer elemento",(arbol_insertar(arbol,&a) == EXITO));
	test_check(stats,"Se puede insertar segundo elemento",(arbol_insertar(arbol,&b) == EXITO));
	test_check(stats,"Se puede insertar tercer elemento",(arbol_insertar(arbol,&c) == EXITO));
	test_check(stats,"Se puede insertar cuarto elemento",(arbol_insertar(arbol,&d) == EXITO));
	test_check(stats,"Se puede insertar quinto elemento",(arbol_insertar(arbol,&e) == EXITO));
	test_check(stats,"Se puede insertar sexto elemento",(arbol_insertar(arbol,&f) == EXITO));
	test_check(stats,"Se puede insertar septimo elemento",(arbol_insertar(arbol,&g) == EXITO));
	test_check(stats,"Arbol no esta vacio",(!arbol_vacio(arbol)));
	test_check(stats,"Funcion nodo raiz devuelve elemento correcto",(*(int*)arbol_raiz(arbol) == a));
	test_check(stats,"Funcion buscar elemento raiz devuelve elemento correcto",(*(int*)arbol_buscar(arbol,&a) == a));
	test_check(stats,"Funcion buscar segundo elemento elemento correcto",(*(int*)arbol_buscar(arbol,&b) == b));
	test_check(stats,"Funcion buscar tercer elemento elemento correcto",(*(int*)arbol_buscar(arbol,&f) == f));
	test_check(stats,"Funcion buscar cuarto elemento elemento correcto",(*(int*)arbol_buscar(arbol,&c) == c));
	test_check(stats,"Funcion buscar quinto elemento elemento correcto",(*(int*)arbol_buscar(arbol,&d) == d));
	test_check(stats,"Funcion buscar sexto elemento elemento correcto",(*(int*)arbol_buscar(arbol,&g) == g));
	test_check(stats,"Funcion buscar septimo elemento elemento correcto",(*(int*)arbol_buscar(arbol,&e) == e));

	arbol_destruir(arbol);
}

//Misma prueba que anterior pero caja blanca
void probar_insertar_arbol_multiples_elementos_mezclados_caja_blanca(stats_t* stats){
	test_anunciar_prueba("Prueba de insertar multiples elementos desordenados CAJA BLANCA");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	nodo_abb_t* nodo_aux;
	int a = 50, b = 20, c = 30, d = 60, e = 70, f = 15 , g = 55;
	
	arbol_insertar(arbol,&a);
	arbol_insertar(arbol,&b);
	arbol_insertar(arbol,&c);
	arbol_insertar(arbol,&d);
	arbol_insertar(arbol,&e);
	arbol_insertar(arbol,&f);
	arbol_insertar(arbol,&g);

	test_check(stats,"Nodo raiz no es NULL",(arbol->nodo_raiz != NULL));
	test_check(stats,"Nodo raiz tiene hijo izquierdo",(arbol->nodo_raiz->izquierda));
	test_check(stats,"Nodo raiz tiene hijo derecho",(arbol->nodo_raiz->derecha));
	test_check(stats,"Elemento en nodo raiz es correcto",(*(int*)arbol->nodo_raiz->elemento == a));
	
	nodo_aux = arbol->nodo_raiz->izquierda;	
	test_check(stats,"Segundo nodo tiene hijo izquierdo",(nodo_aux->izquierda));
	test_check(stats,"Segundo Nodo tiene hijo derecho",(nodo_aux->derecha));
	test_check(stats,"Elemento en segundo nodo es correcto",(*(int*)nodo_aux->elemento == b));

	nodo_aux = nodo_aux->izquierda;	
	test_check(stats,"Tercer nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Tercer Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en tercer nodo es correcto",(*(int*)nodo_aux->elemento == f));
	
	nodo_aux = arbol->nodo_raiz->izquierda->derecha;	
	test_check(stats,"Cuarto nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Cuarto Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en cuarto nodo es correcto",(*(int*)nodo_aux->elemento == c));

	nodo_aux = arbol->nodo_raiz->derecha;	
	test_check(stats,"Quinto nodo tiene hijo izquierdo",(nodo_aux->izquierda));
	test_check(stats,"Quinto Nodo tiene hijo derecho",(nodo_aux->derecha));
	test_check(stats,"Elemento en quinto nodo es correcto",(*(int*)nodo_aux->elemento == d));

	nodo_aux = nodo_aux->izquierda;	
	test_check(stats,"Sexto nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Sexto Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en sexto nodo es correcto",(*(int*)nodo_aux->elemento == g));

	nodo_aux = arbol->nodo_raiz->derecha->derecha;	
	test_check(stats,"Septimo nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Septimo Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en septimo nodo es correcto",(*(int*)nodo_aux->elemento == e));

	arbol_destruir(arbol);
}


/*||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
 *||													||
 *||				      FUNCIONES ESPECIFICAS DE BORRAR:					||
 *||													||
 *||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
*/

//Se testeara el borrado en un arbol vacio
void probar_borrar_arbol_vacio(stats_t* stats){
	test_anunciar_prueba("Prueba de borrar arbol vacio");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	int  b = 10;
	test_check(stats,"No se puede borrar elemento que no existe",(arbol_borrar(arbol,&b) != EXITO));
	arbol_destruir(arbol);
}

//Se testeara el borrado en un arbol con un solo elemento
void probar_borrar_arbol_unico_elemento(stats_t* stats){
	test_anunciar_prueba("Prueba de borrar el unico elemento de un arbol");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	int a = 15,b = 10;
	arbol_insertar(arbol,&a);
	test_check(stats,"No se puede borrar elemento que no existe",(arbol_borrar(arbol,&b) != EXITO));
	test_check(stats,"Se puede borrar unico elemento",(arbol_borrar(arbol,&a) == EXITO));
	test_check(stats,"No se puede borrar unico elemento por segunda vez",(arbol_borrar(arbol,&a) != EXITO));
	test_check(stats,"Arbol esta vacio",(arbol_vacio(arbol)));

	arbol_destruir(arbol);
}

//Se testeara el borrado de una hoja
void probar_borrar_hoja(stats_t* stats){
	test_anunciar_prueba("Prueba de borrar hoja CAJA NEGRA");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	int a = 15,b = 10,c = 20; 
	arbol_insertar(arbol,&a);
	arbol_insertar(arbol,&b);
	arbol_insertar(arbol,&c);
	test_check(stats,"Se puede borrar hoja",(arbol_borrar(arbol,&c) == EXITO));
	test_check(stats,"Funcion arbol buscar no encuentra elemento borrado",(!arbol_buscar(arbol,&c)));
	test_check(stats,"Se puede borrar segunda hoja",(arbol_borrar(arbol,&b) == EXITO));
	test_check(stats,"Funcion arbol buscar no encuentra elemento borrado",(!arbol_buscar(arbol,&b)));
	test_check(stats,"Se puede borrar ultima hoja",(arbol_borrar(arbol,&a) == EXITO));
	test_check(stats,"Funcion arbol buscar no encuentra elemento borrado",(!arbol_buscar(arbol,&a)));
	test_check(stats,"Arbol esta vacio",(arbol_vacio(arbol)));
	arbol_destruir(arbol);
}

//Misma prueba que anterior pero caja blanca
void probar_borrar_hoja_caja_blanca(stats_t* stats){
	test_anunciar_prueba("Prueba de borrar hoja CAJA BLANCA");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	nodo_abb_t* nodo_aux;
	int a = 15,b = 10,c = 20; 
	arbol_insertar(arbol,&a);
	arbol_insertar(arbol,&b);
	arbol_insertar(arbol,&c);
	

	test_anunciar_prueba("Primer borrado");
	arbol_borrar(arbol,&c);
	nodo_aux = arbol->nodo_raiz;
	test_check(stats,"Primer nodo contiene elemento correcto",((nodo_aux->elemento = &a)));
	test_check(stats,"Primer nodo no tiene hijo derecho", (!nodo_aux->derecha));
	test_check(stats,"Primer nodo tiene hijo izquierdo", (nodo_aux->izquierda));

	nodo_aux = nodo_aux->izquierda;
	test_check(stats,"Segundo nodo contiene elemento correcto",((nodo_aux->elemento = &b)));
	test_check(stats,"Segundo nodo no tiene hijo derecho", (!nodo_aux->derecha));
	test_check(stats,"Segundo nodo no tiene hijo izquierdo", (!nodo_aux->izquierda));

	test_anunciar_prueba("Segundo borrado");
	arbol_borrar(arbol,&b);
	nodo_aux = arbol->nodo_raiz;
	test_check(stats,"Primer nodo contiene elemento correcto",((nodo_aux->elemento = &a)));
	test_check(stats,"Primer nodo no tiene hijo derecho", (!nodo_aux->derecha));
	test_check(stats,"Primer nodo no tiene hijo izquierdo", (!nodo_aux->izquierda));

	test_anunciar_prueba("Tercer borrado");
	arbol_borrar(arbol,&a);
	test_check(stats,"Nodo raiz es NULL",(!arbol->nodo_raiz));

	arbol_destruir(arbol);
}

//Se testeara el borrado de nodos con un solo hijo
void probar_borrar_nodo_un_hijo(stats_t* stats){
	test_anunciar_prueba("Prueba de borrar nodo con un hijo CAJA NEGRA");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	int a = 15,b = 10,c = 5;
	arbol_insertar(arbol,&a);
	arbol_insertar(arbol,&b);
	arbol_insertar(arbol,&c);
	
	test_check(stats,"Se puede borrar nodo con un hijo",(arbol_borrar(arbol,&b) == EXITO));
	test_check(stats,"Funcion arbol buscar no encuentra elemento borrado",(!arbol_buscar(arbol,&b)));
	test_check(stats,"Se puede borrar otro nodo con un hijo",(arbol_borrar(arbol,&a) == EXITO));
	test_check(stats,"Funcion arbol buscar no encuentra elemento borrado",(!arbol_buscar(arbol,&a)));
	test_check(stats,"Arbol no esta vacio",(!arbol_vacio(arbol)));
	arbol_destruir(arbol);
}

//Misma prueba que anterior pero caja blanca
void probar_borrar_nodo_un_hijo_caja_blanca(stats_t* stats){
	test_anunciar_prueba("Prueba de borrar nodo con un hijo CAJA BLANCA");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	nodo_abb_t* nodo_aux;
	int a = 15,b = 10,c = 5;
	arbol_insertar(arbol,&a);
	arbol_insertar(arbol,&b);
	arbol_insertar(arbol,&c);
	
	test_anunciar_prueba("Primer borrado");
	arbol_borrar(arbol,&b);
	nodo_aux = arbol->nodo_raiz;
	test_check(stats,"Primer nodo contiene elemento correcto",((nodo_aux->elemento = &a)));
	test_check(stats,"Primer nodo no tiene hijo derecho", (!nodo_aux->derecha));
	test_check(stats,"Primer nodo tiene hijo izquierdo", (nodo_aux->izquierda));

	nodo_aux = nodo_aux->izquierda;
	test_check(stats,"Segundo nodo contiene elemento correcto",((nodo_aux->elemento = &c)));
	test_check(stats,"Segundo nodo no tiene hijo derecho", (!nodo_aux->derecha));
	test_check(stats,"Segundo nodo no tiene hijo izquierdo", (!nodo_aux->izquierda));

	test_anunciar_prueba("Segundo borrado");
	arbol_borrar(arbol,&a);
	nodo_aux = arbol->nodo_raiz;
	test_check(stats,"Primer nodo contiene elemento correcto",((nodo_aux->elemento = &c)));
	test_check(stats,"Primer nodo no tiene hijo derecho", (!nodo_aux->derecha));
	test_check(stats,"Primer nodo no tiene hijo izquierdo", (!nodo_aux->izquierda));

	arbol_destruir(arbol);
}

//Se testeara el borrado de nodos con dos hijos
void probar_borrar_nodo_dos_hijos(stats_t* stats){
	test_anunciar_prueba("Prueba de borrar nodo con dos hijos CAJA NEGRA");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	int a = 40, b = 20, c = 60, d = 10, e = 30, f = 50 , g = 70;
	arbol_insertar(arbol,&a);
	arbol_insertar(arbol,&b);
	arbol_insertar(arbol,&c);
	arbol_insertar(arbol,&d);
	arbol_insertar(arbol,&e);
	arbol_insertar(arbol,&f);
	arbol_insertar(arbol,&g);

	test_check(stats,"Se puede borrar nodo con dos hijos hoja",(arbol_borrar(arbol,&c) == EXITO));
	test_check(stats,"Funcion arbol buscar no encuentra elemento borrado",(!arbol_buscar(arbol,&c)));
	test_check(stats,"Se puede borrar otro nodo con dos hijos con hijos",(arbol_borrar(arbol,&a) == EXITO));
	test_check(stats,"Funcion arbol buscar no encuentra elemento borrado",(!arbol_buscar(arbol,&a)));
	test_check(stats,"Arbol no esta vacio",(!arbol_vacio(arbol)));
	arbol_destruir(arbol);
}

//Misma prueba que anterior pero caja blanca
void probar_borrar_nodo_dos_hijos_caja_blanca(stats_t* stats){
	test_anunciar_prueba("Prueba de borrar nodo con dos hijos CAJA BLANCA");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	nodo_abb_t* nodo_aux;
	int a = 40, b = 20, c = 60, d = 10, e = 30, f = 50 , g = 70;
	arbol_insertar(arbol,&a);
	arbol_insertar(arbol,&b);
	arbol_insertar(arbol,&c);
	arbol_insertar(arbol,&d);
	arbol_insertar(arbol,&e);
	arbol_insertar(arbol,&f);
	arbol_insertar(arbol,&g);

	test_anunciar_prueba("Primer borrado");
	arbol_borrar(arbol,&c);
	nodo_aux = arbol->nodo_raiz;
	test_check(stats,"Nodo raiz no es NULL",(nodo_aux != NULL));
	test_check(stats,"Nodo raiz tiene hijo izquierdo",(nodo_aux->izquierda));
	test_check(stats,"Nodo raiz tiene hijo derecho",(nodo_aux->derecha));
	test_check(stats,"Elemento en nodo raiz es correcto",(nodo_aux->elemento == &a));
	
	nodo_aux = arbol->nodo_raiz->izquierda;	
	test_check(stats,"Segundo nodo tiene hijo izquierdo",(nodo_aux->izquierda));
	test_check(stats,"Segundo Nodo tiene hijo derecho",(nodo_aux->derecha));
	test_check(stats,"Elemento en segundo nodo es correcto",(nodo_aux->elemento == &b));

	nodo_aux = nodo_aux->izquierda;	
	test_check(stats,"Tercer nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Tercer Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en tercer nodo es correcto",(nodo_aux->elemento == &d));
	
	nodo_aux = arbol->nodo_raiz->izquierda->derecha;	
	test_check(stats,"Cuarto nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Cuarto Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en cuarto nodo es correcto",(nodo_aux->elemento == &e));

	nodo_aux = arbol->nodo_raiz->derecha;	
	test_check(stats,"Quinto nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Quinto Nodo tiene hijo derecho",(nodo_aux->derecha));
	test_check(stats,"Elemento en quinto nodo es correcto",(nodo_aux->elemento == &f));

	nodo_aux = nodo_aux->derecha;	
	test_check(stats,"Sexto nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Sexto Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en sexto nodo es correcto",(nodo_aux->elemento == &g));


	test_anunciar_prueba("Segundo borrado");
	arbol_borrar(arbol,&a);
	nodo_aux = arbol->nodo_raiz;
	test_check(stats,"Nodo raiz no es NULL",(nodo_aux != NULL));
	test_check(stats,"Nodo raiz tiene hijo izquierdo",(nodo_aux->izquierda));
	test_check(stats,"Nodo raiz tiene hijo derecho",(nodo_aux->derecha));
	test_check(stats,"Elemento en nodo raiz es correcto",(nodo_aux->elemento == &e));
	
	nodo_aux = arbol->nodo_raiz->izquierda;	
	test_check(stats,"Segundo nodo tiene hijo izquierdo",(nodo_aux->izquierda));
	test_check(stats,"Segundo nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en segundo nodo es correcto",(nodo_aux->elemento == &b));

	nodo_aux = nodo_aux->izquierda;	
	test_check(stats,"Tercer nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Tercer Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en tercer nodo es correcto",(nodo_aux->elemento == &d));
	
	nodo_aux = arbol->nodo_raiz->derecha;	
	test_check(stats,"Cuarto nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Cuarto Nodo tiene hijo derecho",(nodo_aux->derecha));
	test_check(stats,"Elemento en cuarto nodo es correcto",(nodo_aux->elemento == &f));

	nodo_aux = nodo_aux->derecha;	
	test_check(stats,"Quinto nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Quinto Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en quinto nodo es correcto",(nodo_aux->elemento == &g));

	arbol_destruir(arbol);
}


/*||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
 *||													||
 *||				      FUNCIONES ESPECIFICAS DE RECORRIDOS:				||
 *||													||
 *||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
*/

//Se testeara que no se pueda recorrer el arbol con algunos parametros invalidos
void probar_recorrido_inorden_array_invalido(stats_t* stats){
	test_anunciar_prueba("Prueba de recorrido inorden con array invalido");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	int a = 10;
	void* array[1];
	arbol_insertar(arbol,&a);
	test_check(stats,"No se puede recorrer el arbol con array NULL",(!arbol_recorrido_inorden(arbol,NULL,5)));
	test_check(stats,"No se puede recorrer el arbol con tope = 0",(!arbol_recorrido_inorden(arbol,array,0)));
	arbol_destruir(arbol);
}

void probar_recorrido_postorden_array_invalido(stats_t* stats){
	test_anunciar_prueba("Prueba de recorrido postorden con array invalido");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	int a = 10;
	void* array[1];
	arbol_insertar(arbol,&a);
	test_check(stats,"No se puede recorrer el arbol con array NULL",(!arbol_recorrido_postorden(arbol,NULL,5)));
	test_check(stats,"No se puede recorrer el arbol con tope = 0",(!arbol_recorrido_postorden(arbol,array,0)));
	arbol_destruir(arbol);
}


void probar_recorrido_preorden_array_invalido(stats_t* stats){
	test_anunciar_prueba("Prueba de recorrido preorden con array invalido");
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	int a = 10;
	void* array[1];
	arbol_insertar(arbol,&a);
	test_check(stats,"No se puede recorrer el arbol con array NULL",(!arbol_recorrido_preorden(arbol,NULL,5)));
	test_check(stats,"No se puede recorrer el arbol con tope = 0",(!arbol_recorrido_preorden(arbol,array,0)));
	arbol_destruir(arbol);
}

//Se testeara que se pueda recorrer el arbol con el recorrido dado y con la cantidad maxima del array dado
void probar_recorrido_inorden_array_valido(stats_t* stats, int cantidad){
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	void* array[10];
	int a = 40, b = 20, c = 60, d = 10, e = 30, f = 50 , g = 70;
	arbol_insertar(arbol,&a);
	arbol_insertar(arbol,&b);
	arbol_insertar(arbol,&c);
	arbol_insertar(arbol,&d);
	arbol_insertar(arbol,&e);
	arbol_insertar(arbol,&f);
	arbol_insertar(arbol,&g);
	int correctos[7] = {d,b,e,a,f,c,g};

	int cantidad_correcta = 7;
	if(cantidad < cantidad_correcta){
		cantidad_correcta = cantidad;
	}
	
	test_check(stats,"El recorrido inorden devuelve cantidad correcta",(arbol_recorrido_inorden(arbol,array,cantidad) == cantidad_correcta));
	test_check(stats,"Elementos correctos al recorrer inorden",(elementos_correctos_int(correctos,array,cantidad_correcta)));
	arbol_destruir(arbol);
}

void probar_recorrido_postorden_array_valido(stats_t* stats, int cantidad){
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	void* array[10];
	int a = 40, b = 20, c = 60, d = 10, e = 30, f = 50 , g = 70;
	arbol_insertar(arbol,&a);
	arbol_insertar(arbol,&b);
	arbol_insertar(arbol,&c);
	arbol_insertar(arbol,&d);
	arbol_insertar(arbol,&e);
	arbol_insertar(arbol,&f);
	arbol_insertar(arbol,&g);
	int correctos[7] = {d,e,b,f,g,c,a};

	int cantidad_correcta = 7;
	if(cantidad < cantidad_correcta){
		cantidad_correcta = cantidad;
	}
	
	test_check(stats,"El recorrido postorden devuelve cantidad correcta",(arbol_recorrido_postorden(arbol,array,cantidad) == cantidad_correcta));
	test_check(stats,"Elementos correctos al recorrer postorden",(elementos_correctos_int(correctos,array,cantidad_correcta)));
	arbol_destruir(arbol);
}

void probar_recorrido_preorden_array_valido(stats_t* stats, int cantidad){
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	void* array[10];
	int a = 40, b = 20, c = 60, d = 10, e = 30, f = 50 , g = 70;
	arbol_insertar(arbol,&a);
	arbol_insertar(arbol,&b);
	arbol_insertar(arbol,&c);
	arbol_insertar(arbol,&d);
	arbol_insertar(arbol,&e);
	arbol_insertar(arbol,&f);
	arbol_insertar(arbol,&g);
	int correctos[7] = {a,b,d,e,c,f,g};

	int cantidad_correcta = 7;
	if(cantidad < cantidad_correcta){
		cantidad_correcta = cantidad;
	}
	
	test_check(stats,"El recorrido preorden devuelve cantidad correcta",(arbol_recorrido_preorden(arbol,array,cantidad) == cantidad_correcta));
	test_check(stats,"Elementos correctos al recorrer postorden",(elementos_correctos_int(correctos,array,cantidad_correcta)));
	arbol_destruir(arbol);
}

//Funciones "madres" de cada recorrido, ejecutaran distintas pruebas para cada tipo de recorrido de arbol
void probar_recorrido_inorden(stats_t* stats){
	test_anunciar("Iniciando pruebas de recorrido inorden");
	probar_recorrido_inorden_array_invalido(stats);
	test_anunciar_prueba("Prueba de recorrido inorden con array suficiente");	
	probar_recorrido_inorden_array_valido(stats,7);
	test_anunciar_prueba("Prueba de recorrido inorden con array mas pequeno");
	probar_recorrido_inorden_array_valido(stats,5);
	test_anunciar_prueba("Prueba de recorrido inorden con array mas grande");
	probar_recorrido_inorden_array_valido(stats,10);
}

void probar_recorrido_postorden(stats_t* stats){
	test_anunciar("Iniciando pruebas de recorrido postorden");
	probar_recorrido_postorden_array_invalido(stats);
	test_anunciar_prueba("Prueba de recorrido postorden con array suficiente");	
	probar_recorrido_postorden_array_valido(stats,7);
	test_anunciar_prueba("Prueba de recorrido postorden con array mas pequeno");
	probar_recorrido_postorden_array_valido(stats,5);
	test_anunciar_prueba("Prueba de recorrido postorden con array mas grande");
	probar_recorrido_postorden_array_valido(stats,10);
}

void probar_recorrido_preorden(stats_t* stats){
	test_anunciar("Iniciando pruebas de recorrido preorden");
	probar_recorrido_postorden_array_invalido(stats);
	test_anunciar_prueba("Prueba de recorrido preorden con array suficiente");	
	probar_recorrido_postorden_array_valido(stats,7);
	test_anunciar_prueba("Prueba de recorrido preorden con array mas pequeno");
	probar_recorrido_postorden_array_valido(stats,5);
	test_anunciar_prueba("Prueba de recorrido preorden con array mas grande");
	probar_recorrido_postorden_array_valido(stats,10);
}


/*||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
 *||													||
 *||				  FUNCIONES ESPECIFICAS DE CON CADA ELEMENTO:				||
 *||													||
 *||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
*/

//Dado un recorrido valido ejecutara la funcion suma con cada elemento
void probar_con_cada_elemento_recorrido(stats_t* stats, int recorrido){
	abb_t* arbol = arbol_crear(&comparador_enteros,NULL);
	int suma = 0,suma_correcta;
	int a = 40, b = 20, c = 60, d = 10, e = 30, f = 50 , g = 70;
	arbol_insertar(arbol,&a);
	arbol_insertar(arbol,&b);
	arbol_insertar(arbol,&c);
	arbol_insertar(arbol,&d);
	arbol_insertar(arbol,&e);
	arbol_insertar(arbol,&f);
	arbol_insertar(arbol,&g);
	

	abb_con_cada_elemento(arbol,recorrido,&suma_int_hasta_100,&suma);	
	if(recorrido == ABB_RECORRER_INORDEN){
		suma_correcta = 150;
	}else if(recorrido == ABB_RECORRER_PREORDEN){
		suma_correcta = 160;
	}else{
		suma_correcta = 110;
	}
	
	test_check(stats,"Funcion con cada elemento suma correcta",(suma == suma_correcta));
	arbol_destruir(arbol);
}

/*||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
 *||													||
 *||			        FUNCIONES ESPECIFICAS DE ELEMENTOS EN EL HEAP:				||
 *||													||
 *||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
*/

void probar_arbol_elementos_heap_caja_negra(stats_t* stats){
	test_anunciar_prueba("Prueba de insertar y borrar multiples elementos en el heap desordenados CAJA NEGRA");
	abb_t* arbol = arbol_crear(&comparador_enteros,&destructor_simple);
	int* elementos[7];
	for(int i = 0; i < 7; i++){
		elementos[i] = malloc(sizeof(int));
	}
	*elementos[0] =	40;
	int elemento1aux = 40;
	*elementos[1] =	20;
	*elementos[2] =	60;
	int elemento2aux = 60;
	*elementos[3] =	10;
	*elementos[4] =	30;
	*elementos[5] =	50;
	*elementos[6] =	70;


	test_check(stats,"Se puede insertar primer elemento",(arbol_insertar(arbol,elementos[0]) == EXITO));
	test_check(stats,"Se puede insertar segundo elemento",(arbol_insertar(arbol,elementos[1]) == EXITO));
	test_check(stats,"Se puede insertar tercer elemento",(arbol_insertar(arbol,elementos[2]) == EXITO));
	test_check(stats,"Se puede insertar cuarto elemento",(arbol_insertar(arbol,elementos[3]) == EXITO));
	test_check(stats,"Se puede insertar quinto elemento",(arbol_insertar(arbol,elementos[4]) == EXITO));
	test_check(stats,"Se puede insertar sexto elemento",(arbol_insertar(arbol,elementos[5]) == EXITO));
	test_check(stats,"Se puede insertar septimo elemento",(arbol_insertar(arbol,elementos[6]) == EXITO));
	test_check(stats,"Arbol no esta vacio",(!arbol_vacio(arbol)));
	test_check(stats,"Funcion nodo raiz devuelve elemento correcto",(*(int*)arbol_raiz(arbol) == *elementos[0]));
	test_check(stats,"Funcion buscar elemento raiz devuelve elemento correcto",(*(int*)arbol_buscar(arbol,elementos[0]) == *elementos[0]));
	test_check(stats,"Funcion buscar segundo elemento elemento correcto",(*(int*)arbol_buscar(arbol,elementos[1]) == *elementos[1]));
	test_check(stats,"Funcion buscar tercer elemento elemento correcto",(*(int*)arbol_buscar(arbol,elementos[2]) == *elementos[2]));
	test_check(stats,"Funcion buscar cuarto elemento elemento correcto",(*(int*)arbol_buscar(arbol,elementos[3]) == *elementos[3]));
	test_check(stats,"Funcion buscar quinto elemento elemento correcto",(*(int*)arbol_buscar(arbol,elementos[4]) == *elementos[4]));
	test_check(stats,"Funcion buscar sexto elemento elemento correcto",(*(int*)arbol_buscar(arbol,elementos[5]) == *elementos[5]));
	test_check(stats,"Funcion buscar septimo elemento elemento correcto",(*(int*)arbol_buscar(arbol,elementos[6]) == *elementos[6]));

	test_check(stats,"Se puede borrar nodo con dos hijos hoja",(arbol_borrar(arbol,elementos[2]) == EXITO));
	test_check(stats,"Funcion arbol buscar no encuentra elemento borrado",(!arbol_buscar(arbol,&elemento2aux)));
	test_check(stats,"Se puede borrar otro nodo con dos hijos con hijos",(arbol_borrar(arbol,elementos[0]) == EXITO));
	test_check(stats,"Funcion arbol buscar no encuentra elemento borrado",(!arbol_buscar(arbol,&elemento1aux)));
	test_check(stats,"Arbol no esta vacio",(!arbol_vacio(arbol)));

	arbol_destruir(arbol);
}

void probar_arbol_elementos_heap_caja_blanca(stats_t* stats){
	test_anunciar_prueba("Prueba de insertar y borrar multiples elementos en el heap desordenados CAJA BLANCA");
	abb_t* arbol = arbol_crear(&comparador_enteros,&destructor_simple);
	nodo_abb_t* nodo_aux;
	int* elementos[7];
	for(int i = 0; i < 7; i++){
		elementos[i] = malloc(sizeof(int));
	}
	*elementos[0] =	40;
	*elementos[1] =	20;
	*elementos[2] =	60;
	*elementos[3] =	10;
	*elementos[4] =	30;
	*elementos[5] =	50;
	*elementos[6] =	70;

	arbol_insertar(arbol,elementos[0]);
	arbol_insertar(arbol,elementos[1]);
	arbol_insertar(arbol,elementos[2]);
	arbol_insertar(arbol,elementos[3]);
	arbol_insertar(arbol,elementos[4]);
	arbol_insertar(arbol,elementos[5]);
	arbol_insertar(arbol,elementos[6]);

	test_check(stats,"Nodo raiz no es NULL",(arbol->nodo_raiz != NULL));
	test_check(stats,"Nodo raiz tiene hijo izquierdo",(arbol->nodo_raiz->izquierda));
	test_check(stats,"Nodo raiz tiene hijo derecho",(arbol->nodo_raiz->derecha));
	test_check(stats,"Elemento en nodo raiz es correcto",(*(int*)arbol->nodo_raiz->elemento == *elementos[0]));
	
	nodo_aux = arbol->nodo_raiz->izquierda;	
	test_check(stats,"Segundo nodo tiene hijo izquierdo",(nodo_aux->izquierda));
	test_check(stats,"Segundo Nodo tiene hijo derecho",(nodo_aux->derecha));
	test_check(stats,"Elemento en segundo nodo es correcto",(*(int*)nodo_aux->elemento == *elementos[1]));

	nodo_aux = nodo_aux->izquierda;	
	test_check(stats,"Tercer nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Tercer Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en tercer nodo es correcto",(*(int*)nodo_aux->elemento == *elementos[3]));
	
	nodo_aux = arbol->nodo_raiz->izquierda->derecha;	
	test_check(stats,"Cuarto nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Cuarto Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en cuarto nodo es correcto",(*(int*)nodo_aux->elemento == *elementos[4]));

	nodo_aux = arbol->nodo_raiz->derecha;	
	test_check(stats,"Quinto nodo tiene hijo izquierdo",(nodo_aux->izquierda));
	test_check(stats,"Quinto Nodo tiene hijo derecho",(nodo_aux->derecha));
	test_check(stats,"Elemento en quinto nodo es correcto",(*(int*)nodo_aux->elemento == *elementos[2]));

	nodo_aux = nodo_aux->izquierda;	
	test_check(stats,"Sexto nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Sexto Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en sexto nodo es correcto",(*(int*)nodo_aux->elemento == *elementos[5]));

	nodo_aux = arbol->nodo_raiz->derecha->derecha;	
	test_check(stats,"Septimo nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Septimo Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en septimo nodo es correcto",(*(int*)nodo_aux->elemento == *elementos[6]));

	test_anunciar_prueba("Primer borrado");
	arbol_borrar(arbol,elementos[2]);
	nodo_aux = arbol->nodo_raiz;
	test_check(stats,"Nodo raiz no es NULL",(nodo_aux != NULL));
	test_check(stats,"Nodo raiz tiene hijo izquierdo",(nodo_aux->izquierda));
	test_check(stats,"Nodo raiz tiene hijo derecho",(nodo_aux->derecha));
	test_check(stats,"Elemento en nodo raiz es correcto",(nodo_aux->elemento == elementos[0]));
	
	nodo_aux = arbol->nodo_raiz->izquierda;	
	test_check(stats,"Segundo nodo tiene hijo izquierdo",(nodo_aux->izquierda));
	test_check(stats,"Segundo Nodo tiene hijo derecho",(nodo_aux->derecha));
	test_check(stats,"Elemento en segundo nodo es correcto",(nodo_aux->elemento == elementos[1]));

	nodo_aux = nodo_aux->izquierda;	
	test_check(stats,"Tercer nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Tercer Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en tercer nodo es correcto",(nodo_aux->elemento == elementos[3]));
	
	nodo_aux = arbol->nodo_raiz->izquierda->derecha;	
	test_check(stats,"Cuarto nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Cuarto Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en cuarto nodo es correcto",(nodo_aux->elemento == elementos[4]));

	nodo_aux = arbol->nodo_raiz->derecha;	
	test_check(stats,"Quinto nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Quinto Nodo tiene hijo derecho",(nodo_aux->derecha));
	test_check(stats,"Elemento en quinto nodo es correcto",(nodo_aux->elemento == elementos[5]));

	nodo_aux = nodo_aux->derecha;	
	test_check(stats,"Sexto nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Sexto Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en sexto nodo es correcto",(nodo_aux->elemento == elementos[6]));


	test_anunciar_prueba("Segundo borrado");
	arbol_borrar(arbol,elementos[0]);
	nodo_aux = arbol->nodo_raiz;
	test_check(stats,"Nodo raiz no es NULL",(nodo_aux != NULL));
	test_check(stats,"Nodo raiz tiene hijo izquierdo",(nodo_aux->izquierda));
	test_check(stats,"Nodo raiz tiene hijo derecho",(nodo_aux->derecha));
	test_check(stats,"Elemento en nodo raiz es correcto",(nodo_aux->elemento == elementos[4]));
	
	nodo_aux = arbol->nodo_raiz->izquierda;	
	test_check(stats,"Segundo nodo tiene hijo izquierdo",(nodo_aux->izquierda));
	test_check(stats,"Segundo nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en segundo nodo es correcto",(nodo_aux->elemento == elementos[1]));

	nodo_aux = nodo_aux->izquierda;	
	test_check(stats,"Tercer nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Tercer Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en tercer nodo es correcto",(nodo_aux->elemento == elementos[3]));
	
	nodo_aux = arbol->nodo_raiz->derecha;	
	test_check(stats,"Cuarto nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Cuarto Nodo tiene hijo derecho",(nodo_aux->derecha));
	test_check(stats,"Elemento en cuarto nodo es correcto",(nodo_aux->elemento == elementos[5]));

	nodo_aux = nodo_aux->derecha;	
	test_check(stats,"Quinto nodo no tiene hijo izquierdo",(!nodo_aux->izquierda));
	test_check(stats,"Quinto Nodo no tiene hijo derecho",(!nodo_aux->derecha));
	test_check(stats,"Elemento en quinto nodo es correcto",(nodo_aux->elemento == elementos[6]));

	arbol_destruir(arbol);


}

/*||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
 *||FUNCIONES PRINCIPALES DE PRUEBAS:									||
 *||Las funciones a continuacion ejecutaran una serie de pruebas sobre el TDA.				||
 *||Cada funcion principal se encarga de testear diferentes funcionalidades del TDA.			||
 *||Las funciones especificas estan aisladas y testean funcionalidades minimas y casos bordes.		||
 *||Se brinda una breve descripcion de cada conjunto de pruebas por encima de cada funcion principal.	||
 *||En cada funcion especifica hay diferentes mini-tests, cada uno con un nombre descriptivo		||
 *||Algunas pruebas estan separadas por modalidad de caja negra y caja blanca: estas pruebas utilizaran ||
 *||la misma secuencia de procedimientos con el arbol pero testearan el correcto funcionamiento de	||
 *||manera diferente											||
 *||PRE: Todas las funciones reciben un puntero a estadisticas, este debe ser valido/distinto de null	||
 *||POST: Modificara las estadisticas, agregando la cantidad de pruebas ejecutadas y exitosas		||
 *||AVISO: Las pruebas estan ordenadas jerarquicamente, es decir, si el TDA no pasa algun test, 	||
 *||creara un grado de incertidumbre sobre las pruebas que estan debajo ya que estas utilizaran,	||
 *||en general, las funciones testeadas anteriormente.							||
 *||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
*/

void probar_crear(stats_t* stats){
	probar_crear_invalido(stats);
	test_anunciar_prueba("Prueba de crear arbol con comparador y destrutor validos");
	probar_crear_comparador_destructor_validos(stats,&comparador_enteros,&destructor_simple);
	test_anunciar_prueba("Prueba de crear arbol con comparador valido y destrutor NULL");
	probar_crear_comparador_destructor_validos(stats,&comparador_enteros,NULL);
}

void probar_insertar(stats_t* stats){
	probar_insertar_arbol_vacio(stats);
	probar_insertar_arbol_multiples_elementos_mayores(stats);
	probar_insertar_arbol_multiples_elementos_menores(stats);
	probar_insertar_arbol_multiples_elementos_mezclados(stats);
}

void probar_insertar_caja_blanca(stats_t* stats){
	probar_insertar_arbol_vacio_caja_blanca(stats);
	probar_insertar_arbol_multiples_elementos_mayores_caja_blanca(stats);
	probar_insertar_arbol_multiples_elementos_menores_caja_blanca(stats);
	probar_insertar_arbol_multiples_elementos_mezclados_caja_blanca(stats);
}

void probar_borrar(stats_t* stats){
	probar_borrar_arbol_vacio(stats);
	probar_borrar_arbol_unico_elemento(stats);
	probar_borrar_hoja(stats);	
	probar_borrar_nodo_un_hijo(stats);
	probar_borrar_nodo_dos_hijos(stats);
}

void probar_borrar_caja_blanca(stats_t* stats){
	probar_borrar_hoja_caja_blanca(stats);	
	probar_borrar_nodo_un_hijo_caja_blanca(stats);
	probar_borrar_nodo_dos_hijos_caja_blanca(stats);
}

void probar_recorridos(stats_t* stats){
	probar_recorrido_inorden(stats);
	probar_recorrido_postorden(stats);
	probar_recorrido_preorden(stats);
}

void probar_con_cada_elemento(stats_t* stats){
	test_anunciar_prueba("Prueba con cada elemento recorrido inorden");
	probar_con_cada_elemento_recorrido(stats,ABB_RECORRER_INORDEN);
	test_anunciar_prueba("Prueba con cada elemento recorrido preorden");
	probar_con_cada_elemento_recorrido(stats,ABB_RECORRER_PREORDEN);
	test_anunciar_prueba("Prueba con cada elemento recorrido postorden");
	probar_con_cada_elemento_recorrido(stats,ABB_RECORRER_POSTORDEN);
}

void probar_arbol_elementos_heap(stats_t* stats){
	probar_arbol_elementos_heap_caja_negra(stats);
	probar_arbol_elementos_heap_caja_blanca(stats);
}

//Se testeara que las primitivas del TDA no fallen/no puedan ser ejecutadas con un arbol NULL
void probar_null(stats_t* stats){
	test_check(stats,"No se puede insertar en un arbol NULL",(arbol_insertar(NULL,NULL) != EXITO));
	test_check(stats,"No se puede borrar elemento en un arbol NULL",(arbol_borrar(NULL,NULL) != EXITO));
	test_check(stats,"No se puede buscar en un arbol NULL",(!arbol_buscar(NULL,NULL)));
	test_check(stats,"Raiz no existe en un arbol NULL",(!arbol_raiz(NULL)));
	test_check(stats,"Arbol NULL esta vacio",(arbol_vacio(NULL)));
	test_check(stats,"No se puede recorrer INORDEN un arbol NULL",(arbol_recorrido_inorden(NULL,(void**)1,1) == 0));
	test_check(stats,"No se puede recorrer PREORDEN un arbol NULL",(arbol_recorrido_preorden(NULL,(void**)1,1) == 0));
	test_check(stats,"No se puede recorrer POSTORDEN un arbol NULL",(arbol_recorrido_postorden(NULL,(void**)1,1) == 0));
}

int main(){
	stats_t* stats = test_crear_stats();
	test_anunciar("Iniciando pruebas de creacion");
	probar_crear(stats);
	test_anunciar("Iniciando pruebas con arbol NULL");
	probar_null(stats);
	test_anunciar("Iniciando pruebas de insertar caja negra");
	probar_insertar(stats);
	test_anunciar("Iniciando pruebas de insertar caja blanca");
	test_warn("Si alguna de las pruebas de insertar caja negra fallo, estas pruebas podran generar segmentation fault");
	probar_insertar_caja_blanca(stats);
	test_anunciar("Iniciando pruebas de borrar caja negra");
	probar_borrar(stats);
	test_anunciar("Iniciando pruebas de borrar caja blanca");
	test_warn("Si alguna de las pruebas de borrar caja negra fallo, estas pruebas podran generar segmentation fault");
	probar_borrar_caja_blanca(stats);
	probar_recorridos(stats);
	test_anunciar("Iniciando pruebas de abb con cada elemento");
	probar_con_cada_elemento(stats);	
	test_anunciar("Iniciando pruebas de insertar y borrar en el heap");
	test_warn("Si alguna de las pruebas anteriores fallo, estas pruebas podran generar segmentation fault");
	probar_arbol_elementos_heap(stats);

	test_anunciar("Pruebas finalizadas");
	test_imprimir_stats(stats);
	test_destruir_stats(stats);
	return 0;
}
