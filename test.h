#ifndef __TEST_H__
#define __TEST_H__

#include <stdbool.h>

typedef struct stats stats_t;

//Devolvera un puntero a estadisticas si se puede reservar memoria, null si surge un error
stats_t* test_crear_stats(void);

//Imprime el numero de pruebas totales y pruebas pasadas
void test_imprimir_stats(stats_t* stats);

//Libera el espacio reservado para las estadisticas
void test_destruir_stats(stats_t* stats);

/*Pre: Las estadisticas deben ser validas
 *Post: Imprime el nombre del mini-test con un formato determinado
 *Si el test es exitoso imprime EXITO y devuelve true, sino FALLO y devuelve false
 *Modifica las estadisticas de acuerdo al resultado del test
*/
bool test_check(stats_t* stats, char* nombre_prueba, bool condicion);

//Las siguientes funciones imprimen el mensaje recibido por parametro con diferentes formatos
void test_warn(char* warning);

void test_anunciar(char* anuncio);

void test_anunciar_prueba(char* nombre_prueba);


#endif /* __TEST_H__ */
