
#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "strutil.h"
#include "count_min_sketch.h"
#include "hash.h"
#include "heap.h"



/**********************************
 *         ESTRUCTURAS  AUX        *
 *                                 *
 * ********************************/


typedef struct tag_cantidad{
    size_t cantidad;
    const char* tag;
}tag_cantidad_t;


//Declaracion de funciones
void analizar_archivo(FILE* mi_archivo, size_t tamanio_count,size_t k);
void** crear_arreglo(count_min_t* counting, size_t largo);
void imprimir_k_hashtag(size_t k , heap_t* heap);
tag_cantidad_t* crear_estructura(size_t cantidad , const char* nombre);
void tyg_destruir(void** arr, size_t n);
bool actualizar_tdas(count_min_t* counting, heap_t* heap);
int cmp_enteros(void *a, void *b);
void procesar_k_frecuentes(count_min_t* counting, size_t cont_ciclo, size_t k);
void printear_hash(count_min_t* counting);//borrar despues
char* copiar_tag(const char* clave);

int main(int argc, char* argv[]){

    if(argc < 2){
        printf("Valores de entrada insuficientes\n");
        return 1;
    }
    FILE* mi_archivo = stdin;
    if (!mi_archivo){
        printf("Error al abrir el archivo\n");
        return 1;
    }   
    size_t tamanio_count = atoi(argv[1]);
    size_t k = atoi(argv[2]);
    analizar_archivo(mi_archivo, tamanio_count, k);
    fclose(mi_archivo);
    return 0;
}

void analizar_archivo(FILE* mi_archivo, size_t tamanio_count, size_t k){

    count_min_t* counting = crear_count_min(tamanio_count * 1500);
    char* linea = NULL;     
    size_t tam = 0;
    size_t contador_linea = 0;
    size_t contador_ciclo = 1;
    while(getline(&linea, &tam, mi_archivo) != EOF){
        char** usuario_y_tags = split(linea, ',');
        size_t indice = 1;
        while(usuario_y_tags[indice]){
            const char* tag = usuario_y_tags[indice++];
            bool apariciones = count_min_almacenar_aparicion(counting, tag);
            if(!apariciones) break;
            void* numero = count_min_cant_apariciones(counting, tag);
            hash_guardar(obtener_hash(counting), tag, numero);
        }
        contador_linea++;
        if(contador_linea >= tamanio_count){
            procesar_k_frecuentes(counting, contador_ciclo, k);
            contador_linea = 0;
            contador_ciclo++;
        }
        free_strv(usuario_y_tags);
    }
    if(hash_cantidad(obtener_hash(counting))) procesar_k_frecuentes(counting, contador_ciclo, k);
    count_min_destruir(counting);
    free(linea);
}

void procesar_k_frecuentes(count_min_t* counting, size_t cont_ciclo ,size_t k){

    printf("--- %ld\n", cont_ciclo);
    size_t largo = hash_cantidad(obtener_hash(counting));
    void** arr = crear_arreglo(counting, largo);
    heap_t* heap = heap_crear_arr(arr, largo, cmp_enteros);
    imprimir_k_hashtag(k, heap);
    actualizar_tdas(counting, heap);
}

void** crear_arreglo(count_min_t* counting, size_t largo){

    void** arreglo = calloc(largo, sizeof(tag_cantidad_t*));
    if(!arreglo) return NULL;
    size_t pos_arr = 0;
    hash_t* hash = obtener_hash(counting);
    hash_iter_t* iter = hash_iter_crear(hash);
    while(!hash_iter_al_final(iter)){

        const char* tag = hash_iter_ver_actual(iter);
        size_t cantidad = *(size_t*)hash_obtener(hash, tag);
        tag_cantidad_t* tyg = crear_estructura(cantidad, tag);
        arreglo[pos_arr++] = (void*)tyg;
        hash_iter_avanzar(iter);
    }
    hash_iter_destruir(iter);
    return arreglo;
}

void imprimir_k_hashtag(size_t k , heap_t* heap){
    if(heap_cantidad(heap) < k) k = heap_cantidad(heap);
    for (int i = 0; i < k; i++){
        tag_cantidad_t* tyg = heap_desencolar(heap);
        printf ("%ld %s\n", tyg -> cantidad , tyg -> tag);
        free(tyg);
    }
}

tag_cantidad_t* crear_estructura(size_t cantidad , const char* nombre){
    tag_cantidad_t* tyg = malloc(sizeof(tag_cantidad_t));
    if(!tyg) return NULL;
    tyg -> cantidad = cantidad;
    tyg -> tag = nombre;
    return tyg;
}

bool actualizar_tdas(count_min_t* counting, heap_t* heap){
    heap_destruir(heap, free);
    bool dest_crear = destruir_y_crear_hash(counting);
    if(!dest_crear) return false;
    return true;
}

int cmp_enteros(void *a, void *b){

    tag_cantidad_t* dato1 = a;
    tag_cantidad_t* dato2 = b;
    if(dato1 -> cantidad > dato2 -> cantidad) return 1;
    else if(dato1 -> cantidad < dato2 -> cantidad) return -1;
    return (strcmp((const char*)dato1 -> tag, (const char*)dato2 -> tag) > 0) ? -1 : 1;
}

