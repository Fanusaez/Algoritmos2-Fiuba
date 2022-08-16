#include "cola.h"
#include <stdlib.h>
#include <stdio.h>

// Definicion del struct Nodo y struct cola

typedef struct nodo{

    void* dato;
    struct nodo* prox;
} nodo_t;


typedef struct cola{

    nodo_t* primero;
    nodo_t* ultimo;
} cola_t;


/* *****************************************************************
 *                    PRIMITIVAS DE NODO
 * *****************************************************************/

nodo_t* crear_nodo(void* valor){

    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (!nodo) return NULL;
    nodo -> dato = valor;
    nodo -> prox = NULL;
    return nodo;
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

// Crea una cola.
// Post: devuelve una nueva cola vacía.
cola_t *cola_crear(void){

    cola_t* cola = malloc(sizeof(cola_t));
    if (!cola) return NULL;
    cola -> primero = NULL;
    cola -> ultimo = NULL;
    return cola; 
}


// Destruye la cola. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la cola llama a destruir_dato.
// Pre: la cola fue creada. destruir_dato es una función capaz de destruir
// los datos de la cola, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la cola.
void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){
    
    while (!cola_esta_vacia(cola)){

        void* dato = cola_desencolar(cola);
        if (destruir_dato) destruir_dato(dato);
    }
    free(cola);
}


// Devuelve verdadero si la cola no tiene elementos encolados, false en caso contrario.
// Pre: la cola fue creada.
bool cola_esta_vacia(const cola_t *cola){
    return (cola -> primero == NULL && cola -> ultimo == NULL);
}

// Agrega un nuevo elemento a la cola. Devuelve falso en caso de error.
// Pre: la cola fue creada.
// Post: se agregó un nuevo elemento a la cola, valor se encuentra al final
// de la cola.
bool cola_encolar(cola_t *cola, void *valor){

    nodo_t* nodo = crear_nodo(valor);
    if (!nodo) return false;
    if (cola_esta_vacia(cola)) cola -> primero = nodo;
    else cola -> ultimo -> prox = nodo;
    cola -> ultimo = nodo;
    return true;
}


// Obtiene el valor del primer elemento de la cola. Si la cola tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvió el primer elemento de la cola, cuando no está vacía.
void *cola_ver_primero(const cola_t *cola){

    if (cola_esta_vacia(cola)) return NULL;
    return cola -> primero -> dato;
}


// Saca el primer elemento de la cola. Si la cola tiene elementos, se quita el
// primero de la cola, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvió el valor del primer elemento anterior, la cola
// contiene un elemento menos, si la cola no estaba vacía.
void *cola_desencolar(cola_t *cola){
 
    if (cola_esta_vacia(cola)) return NULL;
    void* dato = cola -> primero -> dato; // Guardo el dato a devolver
    nodo_t* nodo_desencolado = cola -> primero; // Guardo el nodo que desencolo
    cola -> primero = cola -> primero -> prox; // Hago primero al nodo siguiente al que desencole
    nodo_desencolado -> prox = NULL; // apunto a Null el nodo que desencole ( Verificar, creo que no hace falta ya que lo elimino despues)
    free(nodo_desencolado); //Borro el Nodo
    if (!cola -> primero) cola -> ultimo = NULL;
    return dato;
}