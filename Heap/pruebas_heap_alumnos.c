#include "heap.h"
#include "testing.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define VOLUMEN 5000


int cmp_numeros(const void *a, const void *b){
    const int* dato1 = a;
    const int* dato2 = b;
    if(*dato1 > *dato2) return 1;
    else if(*dato1 < *dato2) return -1;
    return 0;
}


void prueba_heap_vacio(){

    heap_t* heap=heap_crear(cmp_numeros);
    print_test("veo si esta vacio un recien creado",heap_esta_vacio(heap));
    print_test("veo la cantidad de un recien creado",heap_cantidad(heap)==0);
    print_test("veo maximo de un vacio (null)",heap_ver_max(heap)==NULL);
    heap_destruir(heap,NULL);
}

void prueba_heap_encolar(){

    heap_t* heap=heap_crear(cmp_numeros);
    int elemento1 = 10;
    int elemento2 = 20;
    heap_encolar(heap,&elemento1);
    print_test("veo si esta vacio con un encolado",heap_esta_vacio(heap)==false);
    print_test("veo la cantidad con un encolado",heap_cantidad(heap)==1);
    print_test("veo el maximo con un solo encolado",*(int*)heap_ver_max(heap)==elemento1);

    heap_encolar(heap,&elemento2);
    print_test("veo si esta vacio con dos encolados",heap_esta_vacio(heap)==false);
    print_test("veo la cantidad con 2 encolados ",heap_cantidad(heap)==2);
    print_test("veo el maximo entre 10 y 20",*(int*)heap_ver_max(heap)==elemento2);


    heap_destruir(heap,NULL);
}

void prueba_heap_desencolar(){

    heap_t* heap=heap_crear(cmp_numeros);
    int elemento1 = 5;
    int elemento2 = 10;
    heap_encolar(heap,&elemento1);
    heap_encolar(heap,&elemento2);
    heap_desencolar(heap);
    print_test("desencolo y veo el nuevo maximo",*(int*)heap_ver_max(heap)==elemento1);

    heap_desencolar(heap);
    
    print_test("encolo 2 , desencolo 2 , veo si esta vacio",heap_esta_vacio(heap));
    print_test("veo si la cantidad es 0",heap_cantidad(heap)==0);
    print_test("busco maximo de heap el cual borre todos sus elementos",heap_ver_max(heap)==NULL);
    heap_destruir(heap,NULL);
}


void prueba_crear_con_arreglo(){

    int arreglo[10];

    for(int i=0;i<10;i++){
    arreglo[i]=i;
    }
    
    void* arreglo_voids[10];

    for(int i=0;i<10;i++){
        arreglo_voids[i]= &arreglo[i];
    }




    heap_t* heap=heap_crear_arr(arreglo_voids,10,cmp_numeros);
    print_test("veo el maximo , deberia ser 9",*(int*)heap_ver_max(heap)==9);

    heap_destruir(heap,NULL);

}




static void prueba_heap_crear_arr(){

    int dato1 = 1, dato2 = 2, dato3 = 3, dato4 = 4 , dato5 = 5, dato6 = 6, dato7 = 7, dato8 = 8;
    void** arreglo = malloc(8 * sizeof(int*));
    arreglo[0] = &dato1;
    arreglo[1] = &dato2;
    arreglo[2] = &dato3;
    arreglo[3] = &dato4;
    arreglo[4] = &dato5;
    arreglo[5] = &dato6;
    arreglo[6] = &dato7;
    arreglo[7] = &dato8;
    
    heap_t* heap = heap_crear_arr(arreglo, 8, cmp_numeros);

    print_test("Prueba heap crear arr", true);
    print_test("Prueba cantidad es 8", heap_cantidad(heap) == 8);
    print_test("Prueba ver maximo es dato8", heap_ver_max(heap) == &dato8);
    print_test("Prueba desencolar es dato8", heap_desencolar(heap) == &dato8);
    print_test("Prueba ver maximo es dato7", heap_ver_max(heap) == &dato7);
    print_test("Prueba desencolar es dato7", heap_desencolar(heap) == &dato7);
    print_test("Prueba ver maximo es dato6", heap_ver_max(heap) == &dato6);
    print_test("Prueba desencolar es dato6", heap_desencolar(heap) == &dato6);
    print_test("Prueba ver maximo es dato5", heap_ver_max(heap) == &dato5);
    print_test("Prueba desencolar es dato5", heap_desencolar(heap) == &dato5);
    print_test("Prueba ver maximo es dato4", heap_ver_max(heap) == &dato4);
    print_test("Prueba desencolar es dato4", heap_desencolar(heap) == &dato4);
    print_test("Prueba ver maximo es dato3", heap_ver_max(heap) == &dato3);
    print_test("Prueba desencolar es dato3", heap_desencolar(heap) == &dato3);
    print_test("Prueba ver maximo es dato2", heap_ver_max(heap) == &dato2);
    print_test("Prueba desencolar es dato2", heap_desencolar(heap) == &dato2);
    print_test("Prueba ver maximo es dato1", heap_ver_max(heap) == &dato1);
    print_test("Prueba desencolar es dato1", heap_desencolar(heap) == &dato1);
    print_test("Prueba catidad es 0", heap_cantidad(heap) == 0);
    heap_destruir(heap, NULL);
    free(arreglo);
}

void prueba_crear_arr_volumen(int largo){

    int arreglo1[largo];
    void* arreglo2[largo];
    void* arreglo3[largo];
    int indice=0;

    while(indice<largo){
        unsigned random = rand()%(unsigned)largo;
        arreglo1[indice]=random;
        arreglo2[indice]=&arreglo1[indice];
        arreglo3[indice]=&arreglo1[indice];
        indice++;
    }
    bool ok=true;
    heap_sort(arreglo3,largo,cmp_numeros);


    heap_t* heap=heap_crear_arr(arreglo2,largo,cmp_numeros);

    for(int i=largo-1;i>=0;i--){
        ok &= (heap_desencolar(heap) == arreglo3[i]);
    }

    print_test("desencolo todos los elementos correctos ", ok);
    heap_destruir(heap,NULL);
}

void prueba_heap_sort_volumen(int largo){
    int arreglo1[largo];
    void* arreglo2[largo];
    int indice=0;
    while(indice<largo){
        unsigned random = rand()%(unsigned)largo;
        arreglo1[indice]=random;
        arreglo2[indice]=&arreglo1[indice];
        indice++;
    }
    heap_sort(arreglo2,largo,cmp_numeros);
    bool ok=true;
    for(int i=0;i<largo-1;i++){
        if ((*(int*)arreglo2[i+1]) < (*(int*)arreglo2[i])) ok=false;
    }
    print_test("el heap sort ordena correctamente ", ok);

}

void prueba_destruir(){

    void** arreglo = calloc(4, sizeof(int*));

    int *p_entero = malloc(sizeof(int));
    *p_entero = 1;
    arreglo[0] = p_entero;

    int *p_entero1 = malloc(sizeof(int));
    *p_entero1 = 2;
    arreglo[1] = p_entero1;

    int *p_entero2 = malloc(sizeof(int));
    *p_entero2 = 3;
    arreglo[2] = p_entero2;

    int *p_entero3 = malloc(sizeof(int));
    *p_entero3 = 4;
    arreglo[3] = p_entero3;

    heap_t* heap=heap_crear_arr(arreglo,4,cmp_numeros);
    heap_destruir(heap,free);
    free(arreglo);

}

void pruebas_heap_estudiante(){
    prueba_heap_vacio();
    prueba_heap_encolar();
    prueba_heap_desencolar();
    prueba_crear_con_arreglo();
    prueba_heap_crear_arr();
    prueba_crear_arr_volumen(VOLUMEN);
    prueba_heap_sort_volumen(VOLUMEN);
    prueba_destruir();
    
}


#ifndef CORRECTOR // Para que no dé conflicto con el main() del corrector.
int main(void){
    pruebas_heap_estudiante();
    return failure_count() > 0; // Indica si falló alguna prueba.
}

#endif