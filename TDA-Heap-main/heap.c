#include "heap.h"
#include <stdlib.h>
#define TAM_INICIAL 100
#define REDIMENSION 2
#define EXPANSION 4

struct heap{
    void** arreglo;
    size_t cant;
    size_t tam;
    cmp_func_t cmp;
    size_t creado;
};

size_t maximo(void *elementos[], size_t tam, size_t padre, cmp_func_t cmp, size_t h_izq, size_t h_der);
bool redimensionar_heap(heap_t* heap, size_t capacidad);
void upheap(void *elementos[], size_t hijo, cmp_func_t cmp);
void downheap(void *elementos[], size_t tam, size_t padre, cmp_func_t cmp);
void heapify(void* elementos[], size_t cant, cmp_func_t cmp);
void swap(void* elementos[], size_t p_elem1, size_t p_elem2);
void _heap_sort(void *elementos[], size_t cant, cmp_func_t cmp);


heap_t* heap_crear(cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;
    heap -> tam = TAM_INICIAL;
    heap -> arreglo = calloc(heap->tam, sizeof(void*));
    if (!heap->arreglo) return NULL;
    heap -> cant = 0;
    heap -> cmp = cmp;
    return heap;
}


heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){

    heap_t* heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;
    heap -> tam = n;
    heap -> arreglo = malloc(sizeof(void*)*n);
    for(int i=0;i<n;i++){
        heap->arreglo[i]=arreglo[i];
    }
    heapify(arreglo, n, cmp);
    heap -> arreglo = arreglo;
    heap -> cant = n;
    heap -> cmp = cmp;
    return heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){
    
    if(destruir_elemento){
        for(int i = 0; i < heap -> cant; i++){
            destruir_elemento(heap -> arreglo[i]);
        }
    }
    free(heap -> arreglo);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap){
    return heap->cant;
}

bool heap_esta_vacio(const heap_t *heap){
    return heap -> cant == 0;
}

bool heap_encolar(heap_t *heap, void *elem){
    if (heap -> cant == heap -> tam){
        size_t nueva_capacidad = heap -> tam * REDIMENSION;
        if (!redimensionar_heap(heap, nueva_capacidad)) return false;
    }
    heap -> arreglo[heap -> cant] = elem;
    heap -> cant++;
    upheap(heap -> arreglo, heap -> cant - 1, heap -> cmp);
    return true;
}

void *heap_desencolar(heap_t *heap){

    if (heap_esta_vacio(heap)) return NULL;
	if ((heap -> cant * EXPANSION <= heap -> tam) && (heap -> cant > TAM_INICIAL)){
        size_t nueva_capacidad = heap -> tam / REDIMENSION;
        if(!redimensionar_heap(heap, nueva_capacidad)) return NULL;
    }
    void* desencolado = heap -> arreglo[0];
    heap -> cant--;
    swap(heap -> arreglo, 0, heap -> cant);
    downheap(heap -> arreglo, heap -> cant, 0, heap -> cmp);
    return desencolado;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    
    heapify(elementos, cant, cmp);
    _heap_sort(elementos, cant - 1, cmp);
}

void *heap_ver_max(const heap_t *heap){
    return heap -> arreglo[0];
}


//funcion auxiliar 


size_t maximo(void *elementos[], size_t tam, size_t padre, cmp_func_t cmp, size_t h_izq, size_t h_der){

    size_t h_max = 0;
    int max = 0;
    if(h_der < tam && h_izq < tam){
        max = cmp(elementos[h_der], elementos[h_izq]);
        h_max = (max > 0)? h_der : h_izq;
    } 
    else if(h_izq < tam)h_max = h_izq;
    if (cmp(elementos[padre], elementos[h_max]) < 0) return h_max;
    return padre;
}
bool redimensionar_heap(heap_t* heap, size_t capacidad){

    void** nuevos_datos = realloc(heap -> arreglo, sizeof(void*) * capacidad);
    if (!nuevos_datos) return false;
    heap -> arreglo = nuevos_datos;
    heap -> tam = capacidad;
    return true;
}

void upheap(void *elementos[], size_t hijo, cmp_func_t cmp){

    if(hijo == 0) return;
    size_t padre = (hijo - 1) / 2;
    if(cmp(elementos[hijo], elementos[padre]) > 0){
        swap(elementos, padre, hijo);
        upheap(elementos, padre, cmp);
    }
}

void downheap(void *elementos[], size_t tam, size_t padre, cmp_func_t cmp){

    size_t h_izq = 2 * padre + 1;
    size_t h_der = 2 * padre + 2;

    if((h_der >=tam) && (h_izq >= tam)) return;
    size_t max = maximo(elementos, tam, padre, cmp, h_izq, h_der);
    if(max != padre){
        swap(elementos, padre, max);
        downheap(elementos, tam, max, cmp);
    }
}


void heapify(void* elementos[], size_t cant, cmp_func_t cmp){
    for(int i = (int)cant / 2; i >= 0; i--) downheap(elementos, cant, i, cmp);
}


void swap(void* elementos[], size_t p_elem1, size_t p_elem2){

    void* elem1 = elementos[p_elem1];
    elementos[p_elem1] = elementos[p_elem2];
    elementos[p_elem2] = elem1;
}

void _heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){

    if(cant == 0) return;
    swap(elementos, 0, cant);
    downheap(elementos, cant, 0, cmp);
    _heap_sort(elementos, cant - 1, cmp);
}