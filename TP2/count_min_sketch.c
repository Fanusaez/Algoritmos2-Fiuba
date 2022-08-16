#include <stdio.h>
#include <stdlib.h>
#include "count_min_sketch.h"
#include "hash.h"
#include "heap.h"
#include <string.h>

//Declaracion funciones auxiliares
size_t* obtener_posiciones(count_min_t* count_min, const char* hashtag);
void** crear_arreglo_apariciones(count_min_t* count_min, const char* hashtag, size_t* pos_hashtag);
size_t hash1(const char *str);
size_t hash2(const char *str);
size_t hash3(const char *str);
size_t hash4(const char *str);

/********************************************
 *              ESTRUCTURAS                 *
 *                                          *
 * ******************************************/

struct count_min{
    size_t* arreglo1;
    size_t* arreglo2;
    size_t* arreglo3;
    size_t* arreglo4;
    hash_t* hash;
    size_t tam; 
};


/********************************************
 *              PRIMITIVAS                  *
 *                                          *
 * ******************************************/


count_min_t* crear_count_min(size_t tamanio){
    count_min_t* count_min = malloc(sizeof(count_min_t));
    count_min -> arreglo1 = calloc(tamanio, sizeof(size_t));
    count_min -> arreglo2 = calloc(tamanio, sizeof(size_t));
    count_min -> arreglo3 = calloc(tamanio, sizeof(size_t));
    count_min -> arreglo4 = calloc(tamanio, sizeof(size_t));
    count_min -> tam = tamanio;
    hash_t* hash1 = hash_crear(NULL);

    if((!count_min -> arreglo1) || (!count_min -> arreglo2) ||(!count_min -> arreglo3)||(!count_min -> arreglo4)||!hash1 ){
        free(count_min);
        return NULL;
    } 
    count_min -> hash = hash1;
    return count_min;
}

void count_min_destruir(count_min_t* count_min){
    free(count_min -> arreglo1);
    free(count_min -> arreglo2);
    free(count_min -> arreglo3);
    free(count_min -> arreglo4);
    hash_destruir(count_min -> hash);
    free(count_min);
}

bool count_min_almacenar_aparicion(count_min_t* count_min, const char* hashtag){
    size_t* pos_hashtag = obtener_posiciones(count_min, hashtag);
    if(!pos_hashtag) return false;
    count_min -> arreglo1[pos_hashtag[0]] += 1; 
    count_min -> arreglo2[pos_hashtag[1]] += 1;
    count_min -> arreglo3[pos_hashtag[2]] += 1;
    count_min -> arreglo4[pos_hashtag[3]] += 1;
    free(pos_hashtag);
    return true;
}

void* count_min_cant_apariciones(count_min_t* count_min, const char* hashtag){
    size_t* pos_hashtag = obtener_posiciones(count_min, hashtag);
    void** apariciones = crear_arreglo_apariciones(count_min, hashtag, pos_hashtag);
    if(!pos_hashtag || !apariciones) return NULL; 
    size_t minimo = *(size_t*)apariciones[0]; 
    void* puntero_minimo = apariciones[0];
    for(int i = 0; i < 4; i++){
        if(*(size_t*)apariciones[i] < minimo){
            minimo = *(size_t*)apariciones[i];
            puntero_minimo = apariciones[i];
        } 
    }
    free(apariciones);
    free(pos_hashtag);
    return puntero_minimo;
}

hash_t* obtener_hash(count_min_t* count_min){
    return count_min -> hash;
}

bool destruir_y_crear_hash(count_min_t* count_min){
    hash_destruir(count_min -> hash);
    hash_t* hash = hash_crear(NULL);
    if(!hash) return false;
    count_min -> hash = hash;
    return true;
}


/********************************************
 *         FUNCIONES AUXILIARES             *
 *                                          *
 * ******************************************/

//Devuelve un puntero a un arreglo de enteros, cuyos enteros son posiciones para el hashtag en los arreglos
//dadas por la funcion hash.
//Pre: count_min ya fue crado 
//Post: Devuelve el puntero en caso de exito. Retorna NULL en caso contrario.
size_t* obtener_posiciones(count_min_t* count_min, const char* hashtag){
    size_t* pos = malloc(4 * sizeof(size_t));
    if(!pos) return NULL;
    pos[0] = hash1(hashtag) % count_min -> tam;
    pos[1] = hash2(hashtag) % count_min -> tam;
    pos[2] = hash3(hashtag) % count_min -> tam;
    pos[3] = hash4(hashtag) % count_min -> tam;
    return pos;
}

//Pre: count_min ya fue creado
//Post: devuelve un arreglo de punteros a enteros, cuyos enteros representan una posible
//cantidad de veces que aparecio el hashtag. Si falla retorna NULL
void** crear_arreglo_apariciones(count_min_t* count_min, const char* hashtag, size_t* pos_hashtag){
    void** arr = malloc(4 * sizeof(size_t*));
    if(!arr) return NULL;
    arr[0] = &count_min -> arreglo1[pos_hashtag[0]];
    arr[1] = &count_min -> arreglo2[pos_hashtag[1]];
    arr[2] = &count_min -> arreglo3[pos_hashtag[2]];
    arr[3] = &count_min -> arreglo4[pos_hashtag[3]];
    return arr;
}


/********************************************
 *         FUNCIONES HASHING                *
 *                                          *
 * ******************************************/
    

size_t hash1(const char *str)
    {
        char* str_new = (char*)str;
        unsigned long hash = 5381;
        int c;

        while ((c = *str_new++))
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash;
    }



size_t hash2(const char *str)
    {
        unsigned long hash = 0;
        int c;
        char* str_new = (char*)str;
        while ((c = *str_new++))
            hash = c + (hash << 6) + (hash << 16) - hash;

        return hash;
    }



size_t hash3(const char *str)
    {
	unsigned int hash = 0;
	int c;
    char* str_new = (char*)str;

	while ((c = *str_new++))
	    hash += c;

	return hash;
    }


size_t hash4(const char* key) {


    size_t length=strlen(key);
    size_t i = 0;
    size_t hash = 0;
    while (i != length) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}


