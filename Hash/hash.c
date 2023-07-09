#include "hash.h"
#include "lista.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define CAP_INICIAL 100
#define FACTOR_CARGA 2
#define FACTOR_DESCARGA 0.6

/*********************************
 *        ESTRUCTURAS            *
 *********************************/


struct hash{
    size_t cantidad;
    size_t capacidad;
    void** arreglo;
    hash_destruir_dato_t funcion_destruir;
};

// Estructura iterador del hash
struct hash_iter{
    hash_t* hash;
    lista_iter_t* lista_iter;
    size_t pos;
};

// Estructura donde se almacena la clave el dato
typedef struct hash_campo{
    char* clave;
    void* dato;
}hash_campo_t;



// declaracion funciones auxiliares
void** rellenar_con_listas(void** arr, size_t largo);
char* copiar_clave(const char* clave);
bool hash_redimensionar(hash_t* hash, size_t capacidad);
size_t funcion_hash(const char *key);
bool remplazar_datos(hash_t* hash, const char* clave, void* dato);
hash_campo_t* buscar_nodo(const hash_t* hash, const char* clave);


/********************************************
 *         PRIMITIVAS HASH                  *
 *                                          *
 * ******************************************/


hash_t *hash_crear(hash_destruir_dato_t destruir_dato){

    hash_t* hash = malloc(sizeof(hash_t));
    if(!hash) return NULL;
    hash -> capacidad = CAP_INICIAL;
    hash -> cantidad = 0;
    void** arreglo = calloc(CAP_INICIAL, sizeof(char*));
    if(!arreglo) return NULL;
    hash -> funcion_destruir = destruir_dato;
    if (!rellenar_con_listas(arreglo, hash -> capacidad)) return NULL;
    hash -> arreglo = arreglo;
    return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){ 
    
    if ((hash -> cantidad / hash -> capacidad) >= FACTOR_CARGA){ 
        if(!hash_redimensionar(hash, hash -> capacidad * FACTOR_CARGA)) return false;
    }

    if(hash_pertenece(hash, clave)) return remplazar_datos(hash, clave, dato);
    size_t posicion = funcion_hash(clave) % hash -> capacidad;
    char* clave_copia = copiar_clave(clave);
    if(!clave_copia) return false;
    hash_campo_t* estructura = malloc (sizeof(hash_campo_t));
    if(!estructura) return false;
    estructura -> clave = clave_copia;
    estructura -> dato = dato; 

    lista_insertar_ultimo(hash -> arreglo[posicion], estructura);
    hash -> cantidad++;
    return true;
}

void *hash_borrar(hash_t *hash, const char *clave){ 

    if ((float)((float)hash -> cantidad /(float) hash -> capacidad) <= FACTOR_DESCARGA && hash -> capacidad > CAP_INICIAL){
        if(!hash_redimensionar(hash, hash -> capacidad / FACTOR_CARGA)) return NULL;
    }
    size_t pos_arr = funcion_hash(clave) % hash -> capacidad;
    lista_t* lista_hash = hash -> arreglo[pos_arr];
    lista_iter_t* iter = lista_iter_crear(lista_hash);
    if(!iter) return NULL;
    while(!lista_iter_al_final(iter)){

        hash_campo_t* actual = lista_iter_ver_actual(iter);
        if(strcmp(actual  -> clave , clave) == 0){
            hash -> cantidad--;
            void* dato = actual -> dato; 
            free(actual -> clave); 
            free(actual); 
            lista_iter_borrar(iter); 
            lista_iter_destruir(iter); 
            return dato;
        } 
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return NULL;
}


void *hash_obtener(const hash_t *hash, const char *clave){
    hash_campo_t* nodo = buscar_nodo(hash, clave);
    return (nodo)? nodo -> dato: NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
    if(buscar_nodo(hash, clave)) return true;
    return false;
}


size_t hash_cantidad(const hash_t *hash){
    return hash -> cantidad;
}

void hash_destruir(hash_t *hash){ 

    for(size_t i = 0; i < hash -> capacidad; i++){

        lista_t* lista_act = hash -> arreglo[i];
        
        while(!lista_esta_vacia(lista_act)){
            
            hash_campo_t* campo = lista_borrar_primero(lista_act); 
            
            if (hash->funcion_destruir!=NULL) hash -> funcion_destruir(campo -> dato); 
            free(campo -> clave); 
            free(campo);       
        }
        lista_destruir(lista_act, NULL); 
    }
    free(hash -> arreglo);
    free(hash); 
}

/********************************************
 *         PRIMITIVAS ITERADOR HASH         *
 *                                          *
 * ******************************************/


hash_iter_t *hash_iter_crear(const hash_t *hash){

    hash_iter_t* iter = malloc(sizeof(hash_iter_t));
    iter -> hash = (hash_t*)hash;
    iter -> pos = 0;
    while(lista_esta_vacia(hash -> arreglo[iter -> pos]) && iter -> pos < hash -> capacidad - 1) iter -> pos++;
    if(!lista_esta_vacia(hash -> arreglo[iter -> pos])) iter -> lista_iter = lista_iter_crear(hash -> arreglo[iter -> pos]);
    return iter;
}


bool hash_iter_avanzar(hash_iter_t *iter){ 

    if (hash_iter_al_final(iter)) return false;
    lista_iter_avanzar(iter -> lista_iter);
    if (lista_iter_al_final(iter -> lista_iter)){
        lista_iter_destruir(iter -> lista_iter);
        iter -> pos++;

        while(!hash_iter_al_final(iter) && lista_esta_vacia(iter->hash->arreglo[iter->pos])) iter->pos++;
        if (!hash_iter_al_final(iter)) iter -> lista_iter = lista_iter_crear(iter -> hash -> arreglo[iter -> pos]);
    }
    return true;
}

bool hash_iter_al_final(const hash_iter_t *iter){ 

    size_t posicion = iter -> pos;
    if (iter -> pos == iter -> hash -> capacidad ) return true;
    while(posicion < iter -> hash -> capacidad && lista_esta_vacia(iter->hash->arreglo[posicion]) ) posicion++;
    if (posicion == iter -> hash -> capacidad) return true;
    return false;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){ 
    if(hash_iter_al_final(iter)) return NULL;
    return ((hash_campo_t*)lista_iter_ver_actual(iter -> lista_iter)) -> clave;
}


void hash_iter_destruir(hash_iter_t *iter){ 
    if(!hash_iter_al_final(iter)) lista_iter_destruir(iter -> lista_iter);
    free(iter);
}


/********************************************
 *         FUNCIONES AUXILIARES             *
 *                                          *
 * ******************************************/


void** rellenar_con_listas(void** arr, size_t largo){

    for(size_t i = 0; i < largo; i++){
        lista_t* lista = lista_crear();
        if (!lista) NULL;
        arr[i] = lista;
    }
    return arr;
}

char* copiar_clave(const char* clave){

    char* copia = malloc(strlen(clave) + 1 * sizeof(char));
    if(!copia) return NULL;
    strcpy(copia, clave);
    return copia;
}

bool hash_redimensionar(hash_t* hash, size_t capacidad){

    void** nuevo_arreglo = calloc(capacidad, sizeof(hash_campo_t*));
    if(!nuevo_arreglo) return false;
    void* aux = rellenar_con_listas(nuevo_arreglo, capacidad);
    if(!aux) return false;
    for (size_t i = 0; i < hash -> capacidad; i++){
        lista_t* lista_hash = hash -> arreglo[i];
        while(!lista_esta_vacia(lista_hash)){
            hash_campo_t* campo = lista_borrar_primero(lista_hash);
            size_t posicion = funcion_hash(campo -> clave) % capacidad;
            lista_insertar_ultimo(nuevo_arreglo[posicion], campo);
        }
        lista_destruir(lista_hash, NULL);
    }
    free(hash -> arreglo);
    hash -> arreglo = nuevo_arreglo;
    hash -> capacidad = capacidad;
    return true;
}
    
bool remplazar_datos(hash_t* hash, const char* clave, void* dato){

    size_t pos_arr = funcion_hash(clave) % hash -> capacidad;
    lista_t* lista_hash = hash -> arreglo[pos_arr];
    lista_iter_t* iter = lista_iter_crear(lista_hash);
    if(!iter) return false;
    while(!lista_iter_al_final(iter)){
        hash_campo_t* actual = lista_iter_ver_actual(iter);
        if(strcmp(actual  -> clave , clave) == 0){
            lista_iter_destruir(iter);
            if(hash -> funcion_destruir) hash -> funcion_destruir(actual -> dato);
            actual -> dato = dato;
            return true;
        } 
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return false;
}

hash_campo_t* buscar_nodo(const hash_t* hash, const char* clave){

    size_t pos_arr = funcion_hash(clave) % hash -> capacidad;
    lista_t* lista_hash = hash -> arreglo[pos_arr];
    lista_iter_t* iter = lista_iter_crear(lista_hash);
    if(!iter) return NULL;

    while(!lista_iter_al_final(iter)){
        hash_campo_t* actual = lista_iter_ver_actual(iter);
        if(strcmp(actual  -> clave , clave) == 0){
            lista_iter_destruir(iter);
            return actual;
        } 
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return NULL;
}


// Funcion de hashing

size_t funcion_hash(const char* key) {

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