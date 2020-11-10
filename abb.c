#include "abb.h"
#define TEMP 0

const int EXITO = 0;
const int ERROR = -1;
const int PRIMERO_MAYOR = 1;
const int SEGUNDO_MAYOR = -1;
const int IGUALES = 0;

abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){
	if(!comparador) return NULL;
	abb_t* abb = malloc(sizeof(abb_t));
	if(!abb) return NULL;

	abb->nodo_raiz = NULL;
	abb->comparador = comparador;
	abb->destructor = destructor;
	return abb;
}

/*Pre: El primer nodo recibido debe ser el nodo raiz del arbol
 *El comparador no debe ser NULL y debe corresponder con los elementos del arbol
 *Post: Dado un elemento y la raiz de un arbol buscara la posicion correspondiente para el elemento
 *reservara memoria para este y lo anadira al arbol.
 *En caso de repetidos, insertara a la derecha del elemento.
*/
int insertar_recursivo(nodo_abb_t** nodo, void* elemento, abb_comparador comparador){
	if(!comparador) return ERROR;
	if(!(*nodo)){
		*nodo = calloc(1,sizeof(nodo_abb_t));
		if(!(*nodo)) return ERROR;
		(*nodo)->elemento = elemento;
		return EXITO;
	}
	int comparacion = comparador((*nodo)->elemento,elemento);

	if(comparacion == PRIMERO_MAYOR){
		return insertar_recursivo(&(*nodo)->izquierda,elemento,comparador);
	}
	
	return insertar_recursivo(&(*nodo)->derecha,elemento,comparador);
}

int arbol_insertar(abb_t* arbol, void* elemento){
	if(!arbol) return ERROR;
	if(!arbol->comparador) return ERROR;

	return insertar_recursivo(&arbol->nodo_raiz,elemento,arbol->comparador);	
}

//Post: devuelve true si el nodo recibido no tiene ningun hijo
bool es_hoja(nodo_abb_t* nodo){
	if(!nodo) return false;
	return (!nodo->izquierda && !nodo->derecha);
}

//Post: devuelve true si el nodo recibido tiene dos hijos distintos de null
bool tiene_dos_hijos(nodo_abb_t* nodo){
	if(!nodo) return false;
	return (nodo->izquierda && nodo->derecha);
}

/*Pre:El nodo recibido debe ser la raiz del arbol/subarbol
 *Post: Devuelve un puntero al nodo ubicado a la extrema derecha del arbol recibido
*/
nodo_abb_t* maximo(nodo_abb_t* nodo){
	if(!nodo) return NULL;
	if(!nodo->derecha) return nodo;
	return maximo(nodo->derecha);
}

/*Pre: El primer nodo recibido debe ser el nodo raiz del arbol
 *El comparador no debe ser NULL y debe corresponder con los elementos del arbol
 *Post: Dado un elemento y la raiz de un arbol buscara y eliminara el elemento del arbol si existe.
 *si no existe devolvera null
 *en caso de que destructor sea distinto de null, destruira el elemento con esa funcion
 *En caso de que el nodo del elemento tenga dos hijos, este se reemplazara por el predecesor inorden
*/
nodo_abb_t* borrar_recursivo(nodo_abb_t* nodo, void* elemento, abb_comparador comparador, abb_liberar_elemento destructor, int* estado){
	if(!estado) return NULL;
	if(!nodo){
		*estado = ERROR;
		return NULL;
	}

	int comparacion = comparador(nodo->elemento,elemento);

	if(comparacion == PRIMERO_MAYOR){
		nodo->izquierda	= borrar_recursivo(nodo->izquierda,elemento,comparador,destructor,estado);
		return nodo;
	}
	if(comparacion == SEGUNDO_MAYOR){
		nodo->derecha = borrar_recursivo(nodo->derecha,elemento,comparador,destructor,estado);
		return nodo;
	}

	if(es_hoja(nodo)){
		if(destructor){
			destructor(nodo->elemento);
		}
		free(nodo);
		return NULL;
	}

	nodo_abb_t* aux;
	if(!tiene_dos_hijos(nodo)){
		if(!nodo->derecha){
			aux = nodo->izquierda;
		}else{
			aux = nodo->derecha;
		}
		if(destructor){
			destructor(nodo->elemento);
		}
		free(nodo);
		return aux;
	}

	aux = maximo(nodo->izquierda);
	if(destructor){
		destructor(nodo->elemento);
	}
	nodo->elemento = aux->elemento;
	nodo->izquierda = borrar_recursivo(nodo->izquierda,aux->elemento,comparador,NULL,estado);
	return nodo;
}


int arbol_borrar(abb_t* arbol, void* elemento){
	if(!arbol) return ERROR;
	if(!arbol->comparador) return ERROR;
	int estado = EXITO;
	arbol->nodo_raiz = borrar_recursivo(arbol->nodo_raiz,elemento,arbol->comparador,arbol->destructor,&estado);
	return estado;
}

/*Pre: El primer nodo recibido debe ser el nodo raiz del arbol
 *El comparador no debe ser NULL y debe corresponder con los elementos del arbol
 *Post: Dado un elemento y la raiz de un arbol buscara el elemento y devolvera un puntero a este
 *si no existe devolvera null
*/
void* buscar_recursivo(nodo_abb_t* nodo, void* elemento, abb_comparador comparador){
	if(!comparador) return NULL;
	if(!nodo) return NULL;

	int comparacion = comparador(nodo->elemento,elemento);

	if(comparacion == IGUALES){
		return nodo->elemento;
	}if(comparacion == PRIMERO_MAYOR){
		return buscar_recursivo(nodo->izquierda,elemento,comparador);
	}
	
	return buscar_recursivo(nodo->derecha,elemento,comparador);
}

void* arbol_buscar(abb_t* arbol, void* elemento){
	if(!arbol) return NULL;
	if(!arbol->comparador) return NULL;

	return buscar_recursivo(arbol->nodo_raiz, elemento, arbol->comparador);
}

void* arbol_raiz(abb_t* arbol){
	if(!arbol) return NULL;
	if(!arbol->nodo_raiz) return NULL;
	return arbol->nodo_raiz->elemento;
}

bool arbol_vacio(abb_t* arbol){
	if(!arbol) return true;
	return (arbol->nodo_raiz == NULL);
}

void inorden_recursivo(nodo_abb_t* nodo, void** vector, int tope, int* cantidad){
	if(!nodo || !cantidad || !vector || (tope <= *cantidad)) return;

	inorden_recursivo(nodo->izquierda,vector,tope,cantidad);
	if(tope <= *cantidad) return;

	vector[*cantidad] = nodo->elemento;
	(*cantidad)++;

	inorden_recursivo(nodo->derecha,vector,tope,cantidad);
}

int arbol_recorrido_inorden(abb_t* arbol, void** array, int tamanio_array){
	if(!arbol) return 0;
	if(!arbol->comparador) return 0;

	int cantidad = 0;
	inorden_recursivo(arbol->nodo_raiz,array, tamanio_array,&cantidad);

	return cantidad;
}

void preorden_recursivo(nodo_abb_t* nodo, void** vector, int tope, int* cantidad){
	if(!nodo || !cantidad || !vector || (tope <= *cantidad)) return;


	vector[*cantidad] = nodo->elemento;
	(*cantidad)++;

	preorden_recursivo(nodo->izquierda,vector,tope,cantidad);
	preorden_recursivo(nodo->derecha,vector,tope,cantidad);
}

int arbol_recorrido_preorden(abb_t* arbol, void** array, int tamanio_array){
	if(!arbol) return 0;
	if(!arbol->comparador) return 0;

	int cantidad = 0;
	preorden_recursivo(arbol->nodo_raiz,array, tamanio_array,&cantidad);
	
	return cantidad;
}

void postorden_recursivo(nodo_abb_t* nodo, void** vector, int tope, int* cantidad){
	if(!nodo || !cantidad || !vector || (tope <= *cantidad)) return;

	postorden_recursivo(nodo->izquierda,vector,tope,cantidad);
	postorden_recursivo(nodo->derecha,vector,tope,cantidad);

	if(tope <= *cantidad) return;
	vector[*cantidad] = nodo->elemento;
	(*cantidad)++;
}

int arbol_recorrido_postorden(abb_t* arbol, void** array, int tamanio_array){
	if(!arbol) return 0;
	if(!arbol->comparador) return 0;

	int cantidad = 0;
	postorden_recursivo(arbol->nodo_raiz,array, tamanio_array,&cantidad);
	
	return cantidad;
}

void destruir_recursivo(nodo_abb_t* nodo, abb_liberar_elemento destructor){
	if(!nodo) return;
	destruir_recursivo(nodo->izquierda,destructor);
	destruir_recursivo(nodo->derecha,destructor);
	if(destructor != NULL) 
		destructor(nodo->elemento);
	free(nodo);
}


void arbol_destruir(abb_t* arbol){
	if(!arbol) return;
	destruir_recursivo(arbol->nodo_raiz,arbol->destructor);
	free(arbol);
}

/*Post: devuelve true si el entero recibido corresponde con algun tipo de recorrido de arbol donde:
 *ABB_RECORRER_INORDEN   0
 *ABB_RECORRER_PREORDEN  1
 *ABB_RECORRER_POSTORDEN 2
*/
bool recorrido_valido(int recorrido){
	return (recorrido == ABB_RECORRER_INORDEN || recorrido == ABB_RECORRER_PREORDEN || recorrido == ABB_RECORRER_POSTORDEN);
}


bool con_cada_elemento_inorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra){
	if(!nodo) return false;

	if(con_cada_elemento_inorden(nodo->izquierda,funcion,extra)) return true;
	if(funcion(nodo->elemento,extra)) return true;
	return con_cada_elemento_inorden(nodo->derecha,funcion,extra);
}

bool con_cada_elemento_preorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra){
	if(!nodo) return false;

	if(funcion(nodo->elemento,extra)) return true;
	if(con_cada_elemento_preorden(nodo->izquierda,funcion,extra)) return true;
	return con_cada_elemento_preorden(nodo->derecha,funcion,extra);
}

bool con_cada_elemento_postorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra){
	if(!nodo) return false;

	if(con_cada_elemento_postorden(nodo->izquierda,funcion,extra)) return true;
	if(con_cada_elemento_postorden(nodo->derecha,funcion,extra)) return true;
	return funcion(nodo->elemento,extra);

}

void abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){
	if(!arbol || !recorrido_valido(recorrido) || !funcion) return;
	if(!arbol->comparador) return;

	if(recorrido == ABB_RECORRER_INORDEN){
		con_cada_elemento_inorden(arbol->nodo_raiz,funcion,extra);
	}else if(recorrido == ABB_RECORRER_PREORDEN){
		con_cada_elemento_preorden(arbol->nodo_raiz,funcion,extra);
	}else{
		con_cada_elemento_postorden(arbol->nodo_raiz,funcion,extra);
	}
}



