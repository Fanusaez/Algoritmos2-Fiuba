#include "pila.h"
#include <stdlib.h>

#define CAP_INICIAL 10
#define REDIMENSION 2
#define EXPANSION 4

bool pila_redimensionar(pila_t* pila, size_t capacidad); // declaro funcion pila_redimensionar

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/


// Crea una pila.
// Post: devuelve una nueva pila vacía.
pila_t *pila_crear(void){

    pila_t* pila = malloc(sizeof(pila_t));
    if (pila == NULL){
        return NULL;
    }
    pila -> datos = malloc(sizeof(void*) * CAP_INICIAL);

    if (pila -> datos == NULL){
        free(pila);
        return NULL;
    }
    pila -> capacidad = CAP_INICIAL;
    pila -> cantidad = 0;
    return pila;
}

// Destruye la pila.
// Pre: la pila fue creada.
// Post: se eliminaron todos los elementos de la pila.
void pila_destruir(pila_t *pila){

    free(pila -> datos);
    free(pila);
}

// Devuelve verdadero si la pila no tiene elementos apilados, false en caso contrario.
// Pre: la pila fue creada.
bool pila_esta_vacia(const pila_t *pila){

    return (pila -> cantidad == 0)? true : false;

}

// Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
// Pre: la pila fue creada.
// Post: se agregó un nuevo elemento a la pila, valor es el nuevo tope.
bool pila_apilar(pila_t *pila, void *valor){

    if (pila -> cantidad == pila -> capacidad){
        size_t nueva_capacidad = pila -> capacidad * REDIMENSION;
        if (!pila_redimensionar(pila, nueva_capacidad)) return false;
    }
    size_t posicion = pila -> cantidad;
    pila -> datos[posicion] = valor;
    pila -> cantidad++; 
    return true;
}

// Obtiene el valor del tope de la pila. Si la pila tiene elementos,
// se devuelve el valor del tope. Si está vacía devuelve NULL.
// Pre: la pila fue creada.
// Post: se devolvió el valor del tope de la pila, cuando la pila no está
// vacía, NULL en caso contrario.
void *pila_ver_tope(const pila_t *pila){

    if (pila_esta_vacia(pila)){
        return NULL;
    }
    size_t pos_tope = pila -> cantidad - 1;
    return pila -> datos[pos_tope];
}

// Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
// tope de la pila, y se devuelve ese valor. Si la pila está vacía, devuelve
// NULL.
// Pre: la pila fue creada.
// Post: si la pila no estaba vacía, se devuelve el valor del tope anterior
// y la pila contiene un elemento menos.
void *pila_desapilar(pila_t *pila){

    if (pila_esta_vacia(pila)){
        return NULL;
    }
    pila -> cantidad--;
    size_t pos_tope = pila -> cantidad;

    if ((pila -> cantidad * EXPANSION <= pila -> capacidad) && (pila -> cantidad > CAP_INICIAL)){
        size_t nueva_capacidad = pila -> capacidad / REDIMENSION;
        pila_redimensionar(pila, nueva_capacidad);

    }
    return pila -> datos[pos_tope];

}

bool pila_redimensionar(pila_t* pila, size_t capacidad){

    void** nuevos_datos = realloc(pila -> datos, sizeof(void*) * capacidad);
    if (nuevos_datos == NULL) return false;
    pila -> datos = nuevos_datos;
    pila -> capacidad = capacidad;
    return true;
}
