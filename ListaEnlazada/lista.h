#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>
/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct lista;
typedef struct lista lista_t;

struct lista_iter;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea la lista, si falla retorna NULL
// Post: La lista fue creada
lista_t *lista_crear(void);

// Devuelve true si la lista, no tiene elementos, false en caso contrario
// Pre: La lista fue creada
bool lista_esta_vacia(const lista_t *lista);

// Inserta en primer lugar un nuevo elemento, devuelve falso en caso de error
// Pre: La lista fue creada
// Post: Se agrego un nuevo elemento a la lista y se encuentra primero
bool lista_insertar_primero(lista_t *lista, void *dato);

// Inserta en ultimo lugar un nuevo elemento, devuelve falso en caso de error
// Pre: La lista fue creada
// Post: Se agrego un nuevo elemento a la lista y se encuentra al final
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Quita el primer elemento de la lista, y devuelve su valor.
// En caso de no tener elementos, se devuelve NULL
// Pre: La lista fue creada
// Post: Se devolvio el valor del primer elemento, la lista tiene un elemento menos si no estaba vacia
void *lista_borrar_primero(lista_t *lista);

// Obtenemos el primer valor de la lista. Si la lista tiene elementos, se
// devuelve el valor del primero, si esta vacio devuelve NULL
// Pre: La lista fue creada
// Post: Se devolvio el valor del primer elemento de la lista, si no estaba vacia;
void *lista_ver_primero(const lista_t *lista);

// Obtenemos el ultimo valor de la lista. Si la lista tiene elementos, se
// devuelve el valor del ultimo, si esta vacio devuelve NULL
// Pre: La lista fue creada
// Post: Se devolvio el valor del  ultimo elemento de la lista, si no estaba vacia;
void *lista_ver_ultimo(const lista_t *lista);

// Obtenemos el largo de la lista y lo devolvemos;
// Pre: La lista fue creada
// Post: Se devolvio el largo de la lista
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

/* ******************************************************************
 *               PRIMITIVAS DEL ITERADOR EXTERNO                    *
 * *****************************************************************/

// Crea el iterador, si falla devuelve NULL
// Post: El iterador fue creado si no fallo.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza una posicion en la lista, devuelve false en caso de que el iterador
// se encuentre al final de la lista
// Pre: La lista y el iterador fueron creados
// Post: Avanzo una posicion si el iterador no estaba al final de la lista;
bool lista_iter_avanzar(lista_iter_t *iter);

// Devuelve el dato del elemento donde se encuentra el iterador, si no existe el actual, devuelve NULL.
// Pre: La lista y el iterador fueron creados
// Post: Se devolvio el dato del elemento donde estaba el iterador
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve true si se encuentra al final de la lista, de lo contrario false
// Pre: La lista y el iterador fueron creados
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador
// Pre: El iterador fue creado
// Post: Se desteruyo el iterador
void lista_iter_destruir(lista_iter_t *iter);

// Inserta el elemento en la posicion donde se encuentra el iterador
// Pre: La lista y el iterador fueron creados
// Post: La lista tiene un elemento mas, y su longitud aumenta
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Borra el elemento de la lista en el cual se encuentra el iterador y devuelve su valor
// Si la lista esta vacia o actual es NULL, devuelve NULL.
// Pre: La lista y el iterador fueron creados
// Post: Se devolcio el dato del elemento borrado, y la lista tiene un elemento menos
void *lista_iter_borrar(lista_iter_t *iter);

// Recibe un puntero a una lista, funcion y un extra.
// Recorre la lista hasta que la condicion de corte se lo permita
// La condicion se corte sera dictada por la funcion visitar, cuando se quiera seguir iterando
// visitar devolvera true, cuando quiera detenerse devolvera false, donde se cortara la iteracion.
// Pre: La lista, y la funcion deben estar creadas
// Post: Itera la lista hasta que la condicion de corte se lo indique
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);


/* *****************************************************************
 *                      PRUEBAS UNITARIAS                          *
 * *****************************************************************/

void pruebas_lista_estudiante(void);

#endif  