#include "cola.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define PRUEBA_VOL_1 100
#define PRUEBA_VOL_2 1000
#define PRUEBA_VOL_3 10000

static void pruebas_condiciones_iniciales_cola(void){

    printf("\nINICIO PRUEBAS COLA\n");
    
    cola_t* cola = cola_crear(); 

    printf("\nCONDICIONES INICIALES\n");

    print_test("Cola esta vacia", cola_esta_vacia(cola) == true);
    print_test("ver primero es NULL", cola_ver_primero(cola) == NULL);
    print_test("Desencolo cola vacia", cola_desencolar(cola) == NULL);

    cola_destruir(cola, NULL);
    print_test("Se destruyo la cola", true);
}


static void pruebas_cola_destruccion_incompleta(void){  // Pasamos como parametro NULL cuadno destruimos la pila

    int* p_entero = malloc(sizeof(int));
    *p_entero = 1;
    char* p_character = malloc(sizeof(char));
    *p_character = 'a';

    printf("\nINICIO PRUEBAS COLA DESTRUCCION INCOMPLETA\n");

    cola_t* cola = cola_crear(); 
    printf("\nEncolo elementos p_entero y p_character y compruebo el correcto funcionamiento de cola_destruir \n");

    print_test("Encolo un elemento", cola_encolar(cola, p_entero) == true);
    print_test("Encolo otro elemetno", cola_encolar(cola, p_character) == true);
    cola_destruir(cola, NULL);

    printf("\nEn este caso llame a cola destruir con un 'NULL' en el seundo parametro, en este caso no se liberara el espacio pedido para las variables p_entero y p_character\n");
    printf("\nsiguen siendo accesibles '%i' y '%c'\n", *p_entero, *p_character);
    print_test("*p_entero es 1", *p_entero == 1);
    print_test("*p_character es 'a' ",*p_character == 'a');
    print_test("Se destruyo la cola", true);

    free(p_character);  //Libero la memoria manualmente para que no haya perdidad de memoria
    free(p_entero);
}

static void pruebas_cola_destruccion_completa(void){ // Pasamos como parametro una funcion cuando destruimos la pila
         
    printf("\nINICIO PRUEBAS COLA DESTRUCCION COMPLETA\n");

    cola_t* cola = cola_crear(); 
    int* p_entero = malloc(sizeof(int));
    *p_entero = 1;
    char* p_character = malloc(sizeof(char));
    *p_character = 'a';

    printf("\nEncolo elementos y me fijo que se borre toda la memoria pedida si paso 'free' por paramentro en cola_destruir\n");
    
    print_test("Encolo un elemento", cola_encolar(cola, p_entero) == true);
    print_test("Encolo otro elemetno", cola_encolar(cola, p_character) == true);
    cola_destruir(cola, free);
    print_test("La cola se destruyo completamente, junto con los datos", true);

}

static void pruebas_cola_vacia_destruccion_incompleta(){

    printf("\nINICIO PRUEBAS COLA VACIA DESTRUCCION INCOMPLETA\n");

    cola_t* cola = cola_crear();
    print_test("La cola esta vacia", cola_esta_vacia(cola) == true);
    cola_destruir(cola, NULL);
    print_test("La cola se destruyo correctamente", true);

}

static void pruebas_cola_vacia_destruccion_completa(){

    printf("\nINICIO PRUEBAS COLA VACIA DESTRUCCION COMPLETA\n");

    cola_t* cola = cola_crear();
    print_test("La cola esta vacia", cola_esta_vacia(cola) == true);
    cola_destruir(cola, free);
    print_test("La cola se destruyo correctamente", true);
}



static void pruebas_invariante_cola(void){

    int entero = 1;
    int* p_entero = &entero;
    char character = 'a';
    char* p_character = &character;
    
    cola_t* cola = cola_crear(); 

    printf("\nINICIO PRUEBAS INVARIANTE COLA\n");

    print_test("Encolo un elemento", cola_encolar(cola, p_entero) == true);
    print_test("Cola no esta vacia", cola_esta_vacia(cola) == false);
    print_test("ver primero es p_entero", cola_ver_primero(cola) == p_entero);
    print_test("Encolo otro elemetno", cola_encolar(cola, p_character) == true);
    print_test("Primero sigue estando p_entero", cola_ver_primero(cola) == p_entero);
    print_test("Desencolo p_entero", cola_desencolar(cola) == p_entero);
    print_test("ver primero es p_character", cola_ver_primero(cola) == p_character);
    print_test("Desencolo p_character", cola_desencolar(cola) == p_character);
    print_test("Cola vacia", cola_esta_vacia(cola) == true);
    cola_destruir(cola, NULL);
    print_test("Destruir cola", true);
}

static void pruebas_invariante_cola_NULL(void){

    int entero = 1;
    int* p_entero = &entero;
    char character = 'a';
    char* p_character = &character;
    
    cola_t* cola = cola_crear(); 

    printf("\nINICIO PRUEBAS INVARIANTE COLA CON NULL APILADO\n");

    print_test("Encolo elemento NULL", cola_encolar(cola, NULL) == true);
    print_test("Cola no esta vacia", cola_esta_vacia(cola) == false);
    print_test("Primero es NULL", cola_ver_primero(cola) == NULL);
    print_test("Encolo un elemento", cola_encolar(cola, p_entero) == true);
    print_test("Encolo otro NULL", cola_encolar(cola, NULL) == true);
    print_test("Encolo otro elemetno", cola_encolar(cola, p_character) == true);
    print_test("Desencolo primer NULL", cola_desencolar(cola) == NULL);
    print_test("Primero ahora es p_entero", cola_ver_primero(cola) == p_entero);
    print_test("Desencolo p_entero", cola_desencolar(cola) == p_entero);
    print_test("Primero ahora es segundo NULL", cola_ver_primero(cola) == NULL);
    print_test("Desencolo segundo NULL", cola_desencolar(cola) == NULL);
    print_test("Primero es p_character", cola_ver_primero(cola) == p_character);
    print_test("Desencolo p_character", cola_desencolar(cola) == p_character);
    print_test("Cola vacia", cola_esta_vacia(cola) == true);
    cola_destruir(cola, NULL);
    print_test("Destruir cola", true);
}

static void pruebas_volumen_cola(int cantidad){
    printf("\nINICIO PRUEBAS VOLUMEN COLA CON %i ELEMENTOS\n", cantidad);
    cola_t* cola = cola_crear(); // creo cola para realizar pruebas

    int vector[cantidad]; //Le doy capacidad de sobra para futuras pruebas
    bool ok = true;

    for (int i = 0; i < cantidad; i++){ // creo vector con 100 elementos y encolo los punteros a enteros
        vector[i] = i; 
        ok &= cola_encolar(cola, &vector[i]);
    }
    print_test("Encolo todos los punteros a los elementos del vector", ok);

    for (int i = 0; i < cantidad; i++){ // Desencolo y verifico que coincida con los elementos del vector
        ok &= (cola_desencolar(cola) == &vector[i]);
    }
    print_test("Todos los punteros desencolados con posicion 'i' eran &vector[i]", ok);

    cola_destruir(cola, NULL);
    print_test("Destruyo la pila", true);
}

static void pruebas_cola_encolando_colas(){

    printf("\nINICIO PRUEBAS COLA CON COLAS\n");


    cola_t* cola = cola_crear(); // creo la cola sobre la cual voy a trabajar

    int entero = 1; 
    int* p_entero = &entero; // variables a encolar en las colas

    cola_t* cola_a = cola_crear(); // colas a encolar
    cola_encolar(cola_a, p_entero);
    cola_t* cola_b = cola_crear();
    cola_encolar(cola_b, p_entero);
    cola_t* cola_c = cola_crear();
    cola_encolar(cola_c, p_entero);

    print_test("Encolo cola_a", cola_encolar(cola, cola_a) == true);
    print_test("Encolo cola_b", cola_encolar(cola, cola_b) == true);
    print_test("Encolo cola_c", cola_encolar(cola, cola_c) == true);
    print_test("Primero es cola_a", cola_ver_primero(cola) == cola_a);
    print_test("Desencolo cola_a", cola_desencolar(cola) == cola_a);
    print_test("primero es cola_b", cola_ver_primero(cola) == cola_b);
    print_test("Desencolo cola_b", cola_desencolar(cola) == cola_b);
    print_test("Primero es cola_c" , cola_ver_primero(cola) == cola_c);
    print_test("Desencolo cola_c", cola_desencolar(cola) == cola_c);
    print_test("Cola esta vacia", cola_esta_vacia(cola) == true);
    
    cola_destruir(cola, NULL);
    cola_destruir(cola_a, NULL);
    cola_destruir(cola_b, NULL);
    cola_destruir(cola_c, NULL);

}

void pruebas_cola_estudiante() {
    pruebas_condiciones_iniciales_cola();
    pruebas_cola_destruccion_incompleta();
    pruebas_cola_destruccion_completa();
    pruebas_cola_vacia_destruccion_incompleta();
    pruebas_cola_vacia_destruccion_completa();
    pruebas_invariante_cola();
    pruebas_invariante_cola_NULL();
    pruebas_volumen_cola(PRUEBA_VOL_1);
    pruebas_volumen_cola(PRUEBA_VOL_2);
    pruebas_volumen_cola(PRUEBA_VOL_3); 
    pruebas_cola_encolando_colas();
}


#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_cola_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
