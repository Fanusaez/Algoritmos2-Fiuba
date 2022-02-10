#ifndef _VECTOR_H
#define _VECTOR_H

#include <stdbool.h>
#include <stdlib.h>

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 ******************************************************************/

typedef struct vector {
    size_t tam;
    int *datos;
} vector_t;


/*******************************************************************
 *                    PRIMITIVAS DEL VECTOR
 ******************************************************************/

// Crea un vector de tamaño tam
// Post: vector es una vector vacío de tamaño tam
vector_t *vector_crear(size_t tam); // DONE

// Destruye el vector
// Pre: el vector fue creado
// Post: se eliminaron todos los elementos del vector
void vector_destruir(vector_t *vector);

// Cambia el tamaño del vector
// Pre: el vector fue creado
// Post: el vector cambió de tamaño a nuevo_tam y devuelve true
// o el vector queda intacto y devuelve false si no se pudo cambiar el tamaño
// a nuevo_tam
bool vector_redimensionar(vector_t *vector, size_t nuevo_tam); //DONE

// Almacena en valor el dato guardado en la posición pos del vector
// Pre: el vector fue creado
// Post: se almacenó en valor el dato en la posición pos. Devuelve false si la
// posición es inválida (fuera del rango del vector, que va de 0 a tamaño-1)
bool vector_obtener(vector_t *vector, size_t pos, int *valor);

// Almacena el valor en la posición pos
// Pre: el vector fue creado
// Post: se almacenó el valor en la posición pos. Devuelve false si la posición
// es inválida (fuera del rango del vector, que va de 0 a tamaño-1) y true si
// se guardó el valor con éxito.
bool vector_guardar(vector_t *vector, size_t pos, int valor);

// Devuelve el tamaño del vector
// Pre: el vector fue creado
size_t vector_largo(vector_t *vector);

#endif  // _VECTOR_H
