
#include <string.h>
#include <stdlib.h>
#include "pila.h"
#include "abb.h"


/* ******************************************************************
 *                          ESTRUCTURAS                             *
 * *****************************************************************/
typedef struct abb_nodo {
    struct abb_nodo *izq;
    struct abb_nodo *der;
    char *clave;
    void *dato;
}abb_nodo_t;


struct abb {
    abb_nodo_t *raiz;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
    size_t cant;
};

struct abb_iter{
    struct abb_t* arbol;
    pila_t* pila;
};


typedef struct familia{
    abb_nodo_t* padre;
    abb_nodo_t* nodo_actual;
    abb_nodo_t* hijo_der;
    abb_nodo_t* hijo_izq;
    size_t cant_hijos;
    bool padre_hijo_der; // para saber si el padre tiene su hijo "nodo_act" a la der o izq
} familia_t;


// Declariacion funciones

void _abb_destruir(abb_t* arbol, abb_nodo_t* nodo);
abb_nodo_t* crear_nodo(const char* clave, void* valor);
char* copiar_clave(const char* clave);
pila_t* apilar_izquierdos(abb_nodo_t* nodo, pila_t* pila);
familia_t* buscar_nodo(abb_t* arbol,const char* clave);
const char* buscar_remplazo(abb_nodo_t* actual, const char* clave);
void remplazar(abb_t* arbol, const char* clave, const char* clave_nueva, void* dato_nuevo);
familia_t* crear_familia();
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);
bool _abb_in_order(abb_t* arbol, abb_nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra);
void destruir_familia(familia_t* familia);
void borrado_un_solo_hijo(familia_t* familia, abb_t* arbol);
abb_nodo_t* obtener_abb_nodo(const abb_t* abb, const char* clave);



/* ******************************************************************
 *              IMPLEMENTACION PRIMITIVAS DEl ABB                   *
 * *****************************************************************/


abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){

    abb_t* abb = malloc(sizeof(abb_t));
    if (!abb) return NULL;
    abb -> raiz = NULL;
    abb -> cmp = cmp;
    abb -> destruir_dato = destruir_dato;
    abb -> cant = 0;
    return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if(abb_pertenece(arbol, clave)){
        remplazar(arbol, clave, NULL, dato);
        return true;
    } 
    abb_nodo_t* nodo = crear_nodo(clave, dato);
    if(!nodo) return false;
    if(!arbol -> raiz){
        arbol -> raiz = nodo;
        arbol -> cant++;
        return true;
    }
    familia_t* familia = buscar_nodo(arbol, clave); // importamnte porque contiene el padre del nuevo nodo
    if(!familia) return false;
    if(arbol -> cmp(familia -> padre -> clave, clave) > 0) familia -> padre -> izq = nodo;
    else if(arbol -> cmp(familia -> padre -> clave, clave) < 0) familia -> padre -> der = nodo;
    free(familia);
    arbol -> cant++;
    return true;
}


void *abb_borrar(abb_t *arbol, const char *clave){ 

    bool pertenece = abb_pertenece(arbol, clave);
    if(!pertenece) return NULL;
    familia_t* familia = buscar_nodo(arbol, clave);
    if(!familia) return NULL;
    void* dato = familia -> nodo_actual -> dato;

    if (familia -> cant_hijos == 0 && familia -> padre){ // hoja
        if(familia -> padre_hijo_der) familia -> padre -> der = NULL;
        else familia -> padre -> izq = NULL;
    }
    else if(familia -> cant_hijos == 0){
        arbol -> raiz = NULL;
    }
    else if (familia -> cant_hijos == 1) borrado_un_solo_hijo(familia, arbol);
        
    else if(familia -> cant_hijos == 2){ // dos hijos
        const char* clave_remplazo = buscar_remplazo(familia -> nodo_actual, clave);
        void* dato_remplazo = abb_borrar(arbol, clave_remplazo);
        remplazar(arbol, clave, clave_remplazo, dato_remplazo);
        free(familia); 
        return dato; 
    }
    destruir_familia(familia);
    arbol -> cant--;
    return dato;
}


void *abb_obtener(const abb_t *arbol, const char *clave){

    abb_nodo_t* abb_nodo = obtener_abb_nodo(arbol, clave);
    return (abb_nodo)? abb_nodo -> dato: NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    abb_nodo_t* abb_nodo = obtener_abb_nodo(arbol, clave);
    return abb_nodo;
}


size_t abb_cantidad(const abb_t *arbol){
    return arbol -> cant;
}


void abb_destruir(abb_t *arbol){
    _abb_destruir(arbol ,arbol -> raiz);
    free(arbol);
}

/* ******************************************************************
 *         IMPLEMENTACION PRIMITIVAS ITERADOR EXTERNO               *
 * *****************************************************************/


abb_iter_t *abb_iter_in_crear(const abb_t *arbol){

    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    iter -> arbol = (struct abb_t*)arbol;
    pila_t* pila = pila_crear();
    if(!pila) return NULL;
    iter -> pila = apilar_izquierdos(arbol -> raiz, pila);
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){

    if (abb_iter_in_al_final(iter)) return false;
    abb_nodo_t* desapilado = pila_desapilar(iter -> pila);
    if(desapilado -> der){
        pila_t* pila = apilar_izquierdos(desapilado -> der, iter -> pila);
        if(pila){
            iter -> pila = pila;
            return true;
        }
        return false;
    }
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    abb_nodo_t* tope = pila_ver_tope(iter -> pila);
    if(tope) return tope -> clave;
    return NULL;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter -> pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
    while(!pila_esta_vacia(iter -> pila)) pila_desapilar(iter -> pila);
    pila_destruir(iter -> pila);
    free(iter);
}


/* ******************************************************************
 *              ITERADOT INTERNO                                    *
 * *****************************************************************/

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    _abb_in_order(arbol,arbol->raiz,visitar,extra);
}


bool _abb_in_order(abb_t* arbol, abb_nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra){

    if (!nodo) return true;
    if (!_abb_in_order(arbol, nodo->izq,visitar,extra)) return false;

    if (!visitar(nodo->clave,nodo->dato,extra)) return false;

    if (!_abb_in_order(arbol, nodo->der,visitar,extra)) return false;
    
    return true;
}

/* ******************************************************************
 *              FUNCIONES AUXILIARES                                *
 * *****************************************************************/


void _abb_destruir(abb_t* arbol, abb_nodo_t* nodo){
    if(!nodo) return;
    _abb_destruir(arbol, nodo -> izq);
    _abb_destruir(arbol, nodo -> der);
    if(arbol -> destruir_dato)arbol -> destruir_dato(nodo -> dato);
    free(nodo -> clave);
    free(nodo);
}


abb_nodo_t* crear_nodo(const char* clave, void* valor){

    abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
    if(!nodo) return NULL;
    nodo -> clave = copiar_clave(clave);
    nodo -> dato = valor;
    nodo -> izq = NULL;
    nodo -> der = NULL;
    return nodo;
}

char* copiar_clave(const char* clave){

    char* copia = malloc(strlen(clave) + 1 * sizeof(char));
    if(!copia) return NULL;
    strcpy(copia, clave);
    return copia;
}

pila_t* apilar_izquierdos(abb_nodo_t* nodo, pila_t* pila){

    while(nodo){
        bool apilar = pila_apilar(pila, nodo);
        if(!apilar) return NULL;
        nodo = nodo -> izq;
    }
    return pila;
}


familia_t* buscar_nodo(abb_t* arbol, const char* clave){

    familia_t* familia = crear_familia();
    if(!arbol -> raiz || !familia) return NULL;
    abb_nodo_t* nodo_actual = arbol -> raiz;
    while(nodo_actual){

        familia -> nodo_actual = nodo_actual;
        if(arbol -> cmp(nodo_actual -> clave, clave) > 0){
            familia -> padre = nodo_actual;
            nodo_actual = nodo_actual -> izq;
            familia -> padre_hijo_der = false;
        }

        else if((arbol -> cmp(nodo_actual -> clave, clave)) < 0){
            familia -> padre = nodo_actual;
            nodo_actual = nodo_actual -> der;
            familia -> padre_hijo_der = true;
        }
        
        else{
            familia -> hijo_der = nodo_actual -> der;
            familia -> hijo_izq  = nodo_actual -> izq;
            if(nodo_actual -> der && nodo_actual -> izq) familia -> cant_hijos = 2;
            else if(nodo_actual -> der || nodo_actual -> izq)  familia -> cant_hijos = 1;
            else familia -> cant_hijos = 0;
            return familia;
        }

    }
    return familia;
}


const char* buscar_remplazo(abb_nodo_t* actual, const char* clave){

    abb_nodo_t* nodo_actual = actual -> der;
    while(nodo_actual -> izq) nodo_actual = nodo_actual -> izq;
    char* clave_remplazo = copiar_clave(nodo_actual -> clave);
    if(!clave_remplazo) return NULL;
    return (const char*) clave_remplazo;
}




void remplazar(abb_t* arbol, const char* clave, const char* clave_nueva, void* dato_nuevo){

    abb_nodo_t* nodo_actual = arbol -> raiz;

    while(nodo_actual){
        
        if(arbol -> cmp(nodo_actual -> clave, clave) > 0){
            nodo_actual = nodo_actual -> izq;
        }
        else if(arbol -> cmp(nodo_actual -> clave, clave) < 0){
            nodo_actual = nodo_actual -> der;
        }
        else{
                if(clave_nueva){ 
                    free(nodo_actual -> clave); //
                    nodo_actual -> clave = (char*)clave_nueva;
                    nodo_actual -> dato = dato_nuevo;
                }
                else{
                    if(arbol -> destruir_dato) arbol -> destruir_dato(nodo_actual -> dato); // asi hace en el hash
                    nodo_actual -> dato = dato_nuevo;
                }
                return;
        } 
    }
}

void borrado_un_solo_hijo(familia_t* familia, abb_t* arbol){
    if(!familia -> padre){
        if(familia -> nodo_actual -> der)arbol -> raiz = familia -> nodo_actual -> der;
        else arbol -> raiz = familia -> nodo_actual -> izq;
        }
    else if(familia -> nodo_actual -> der && familia -> padre_hijo_der) familia -> padre -> der = familia -> nodo_actual -> der;
    else if(familia -> nodo_actual -> izq && familia -> padre_hijo_der) familia -> padre -> der = familia -> nodo_actual -> izq;
    else if(familia -> nodo_actual -> der && !familia -> padre_hijo_der) familia -> padre -> izq = familia -> nodo_actual -> der;
    else familia -> padre -> izq = familia -> nodo_actual -> izq;
}

familia_t* crear_familia(){
    familia_t* familia = malloc(sizeof(familia_t));
    if(!familia) return NULL;
    familia -> cant_hijos = 0;
    familia -> hijo_der = familia -> hijo_izq = familia -> padre = NULL;
    return familia;
}


void destruir_familia(familia_t* familia){
    free(familia -> nodo_actual -> clave);
    free(familia -> nodo_actual);
    free(familia);
}

abb_nodo_t* obtener_abb_nodo(const abb_t* arbol, const char* clave){

    if(!(arbol -> raiz)) return NULL;
    abb_nodo_t* nodo_actual = arbol -> raiz;
    while(nodo_actual){
        
        if(arbol -> cmp(nodo_actual -> clave, clave) > 0){
            nodo_actual = nodo_actual -> izq;
        }
        else if(arbol -> cmp(nodo_actual -> clave, clave) < 0){
            nodo_actual = nodo_actual -> der;
        }
        else return nodo_actual; 
    }
    return NULL;
}