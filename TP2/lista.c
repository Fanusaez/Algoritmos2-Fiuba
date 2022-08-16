
#include <stdlib.h>
#include <stdio.h>
#include "lista.h"
// Definicion struct Nodo struct Lista y struct Lista_Iter

typedef struct nodo {
    void* dato;
    struct nodo* prox;
}nodo_t;

struct lista {
    nodo_t* prim;
    nodo_t* ult;
    size_t largo;
};

struct lista_iter{
    nodo_t* actual;
    nodo_t* anterior;
    lista_t* lista;
};


/* *****************************************************************
 *                    PRIMITIVAS DE NODO                           *
 * *****************************************************************/


nodo_t* crear_nodo(void* valor){

    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (!nodo) return NULL;
    nodo -> dato = valor;
    nodo -> prox = NULL;
    return nodo;
}


/* ******************************************************************
 *              IMPLEMENTACION PRIMITIVAS DE LA LISTA               *
 * *****************************************************************/

lista_t *lista_crear(void){

    lista_t* lista = malloc(sizeof(lista_t));
    if (!lista) return NULL;
    lista -> prim = NULL;
    lista -> ult = NULL;
    lista -> largo = 0;
    return lista;
}

bool lista_esta_vacia(const lista_t *lista){

    return (!lista -> prim && !lista -> ult && !lista -> largo);
}

bool lista_insertar_primero(lista_t *lista, void *dato){
    nodo_t* nuevo_nodo = crear_nodo(dato);
    if (!nuevo_nodo) return false;
    if(lista_esta_vacia(lista)) lista -> ult = nuevo_nodo;  // En caso de que la lista estaba vacia.
    nuevo_nodo -> prox = lista -> prim;
    lista -> prim = nuevo_nodo;
    lista -> largo++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){

    nodo_t* nuevo_nodo = crear_nodo(dato);
    if (!nuevo_nodo) return false;
    
    if(lista_esta_vacia(lista)){
        lista -> prim =  nuevo_nodo;
    }
    else{
        lista -> ult -> prox = nuevo_nodo;
    }
    lista -> ult = nuevo_nodo;
    lista -> largo++;
    return true;
}


void *lista_borrar_primero(lista_t *lista){

    if (!lista -> largo) return NULL;
    nodo_t* nodo_eliminado = lista -> prim;
    void* dato = nodo_eliminado -> dato;
    lista -> prim = lista -> prim -> prox;
    if (lista -> largo == 1) lista -> ult = NULL;
    lista -> largo--;
    free(nodo_eliminado);
    return dato;
}


void *lista_ver_primero(const lista_t *lista){
    if (lista_esta_vacia(lista)) return NULL;
    return lista -> prim -> dato;
}

void *lista_ver_ultimo(const lista_t* lista){

    if (lista_esta_vacia(lista)) return NULL;
    return lista -> ult -> dato;
}

size_t lista_largo(const lista_t *lista){
    return lista -> largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){

    while (!lista_esta_vacia(lista)){

        void* dato = lista_borrar_primero(lista);
        if (destruir_dato) destruir_dato(dato);
    }
    free(lista);
}


/* ******************************************************************
 *               PRIMITIVAS DEL ITERADOR EXTERNO                    *
 * *****************************************************************/


lista_iter_t *lista_iter_crear(lista_t *lista){

    lista_iter_t* iter = malloc(sizeof(lista_iter_t));
    if (!iter) return NULL;
    iter -> actual = lista -> prim;
    iter -> anterior = NULL;
    iter -> lista = lista;
    return iter;
    }

bool lista_iter_avanzar(lista_iter_t *iter){
    if (lista_iter_al_final(iter)) return false;
    iter -> anterior = iter -> actual;
    iter -> actual = iter -> actual -> prox;
    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
    if (lista_iter_al_final(iter)) return NULL;
    return iter -> actual -> dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
    return (!iter -> actual);
}

void lista_iter_destruir(lista_iter_t *iter){
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){

    nodo_t* nodo_nuevo = crear_nodo(dato);
    if(!nodo_nuevo) return false;

    if (!iter -> actual && !iter -> anterior){ // Lista vacia
        iter -> lista -> prim = iter -> lista -> ult = nodo_nuevo;
    }
    else if (!iter -> actual) { // agrego al final
        iter -> anterior -> prox = nodo_nuevo;
        iter -> lista -> ult = nodo_nuevo;
    }
    else if (!iter -> anterior){ // agrego elemento  al princicio
        iter -> lista -> prim = nodo_nuevo;
        nodo_nuevo -> prox = iter -> actual;
    }
    else{
        iter -> anterior -> prox = nodo_nuevo; // caso normal
        nodo_nuevo -> prox = iter -> actual;
    }
    iter -> actual = nodo_nuevo; // siempre posiciono al actual en el nuevo_nodo
    iter -> lista -> largo++;
    return true;
}
    

void *lista_iter_borrar(lista_iter_t *iter){

    if (lista_iter_al_final(iter)) return NULL; // No hay elementos, o estamos al inal de la lista

    nodo_t* nodo_eliminar = iter -> actual;
    void* dato = iter -> actual -> dato;

    if (iter -> lista -> largo == 1){ // solo un elemento
        iter -> lista -> prim = NULL;
        iter -> lista -> ult = NULL;
    }
    else if (!iter -> anterior){ // elimino primero
        iter -> lista -> prim = iter -> actual -> prox;
    }
    else if (iter -> actual && !iter -> actual -> prox){ // elimino ultimo
        iter -> lista -> ult = iter -> anterior;
    }

    iter -> actual = iter -> actual -> prox;
    if (iter -> anterior) iter -> anterior -> prox = iter -> actual;
    free(nodo_eliminar);
    iter -> lista -> largo--;
    return dato;
}


// Iterador interno

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){

    bool cond_corte = true;
    nodo_t* nodo_actual = lista -> prim;
    while (cond_corte && nodo_actual){
        cond_corte = visitar(nodo_actual -> dato, extra);
        nodo_actual = nodo_actual -> prox;
    }
}





