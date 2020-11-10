#include "test.h"
#include <stdio.h>
#include <stdlib.h>

const char* ROJO = "\033[1;31m";
const char* VERDE = "\033[1;32m";
const char* AMARILLO = "\033[1;33m";
const char* BLANCO = "\033[1;135m";
const char* CYAN = "\033[1;36m";
const char* RESET = "\033[0m";

struct stats{
	int pruebas_total;
	int pruebas_ok;
};

stats_t* test_crear_stats(void){
	return calloc(1,sizeof(stats_t));
}

bool test_check(stats_t* stats, char* nombre_prueba, bool condicion){
	printf("%sTest: %-60s >> %s%s\n", condicion?VERDE:ROJO, nombre_prueba, condicion?"EXITO":"FALLO", RESET);
	stats->pruebas_total++;
	if(condicion) stats->pruebas_ok++;
	return condicion;
}

void test_anunciar(char* anuncio){
	printf("\n%s=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n||%-53s||\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=%s\n",CYAN,anuncio,RESET);
}

void test_anunciar_prueba(char* nombre_prueba){
	printf("%s>>%s<<%s\n",BLANCO,nombre_prueba,RESET);
}

void test_warn(char* warning){
	printf("%sWARNING: %s%s\n",AMARILLO,warning,RESET);
}

void test_imprimir_stats(stats_t* stats){
	printf("%sEl programa paso %i / %i de pruebas%s\n",(stats->pruebas_total == stats->pruebas_ok)?VERDE:ROJO, stats->pruebas_ok, stats->pruebas_total, RESET);
}

void test_destruir_stats(stats_t* stats){
	free(stats);
}

