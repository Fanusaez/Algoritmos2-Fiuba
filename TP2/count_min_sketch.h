#include <stdbool.h>
#include "hash.h"
struct count_min;
typedef struct count_min count_min_t;


//Crea count_min_sketch
//Pre: -
//Post: count min sketch fue creado, si falla se retorna NULL
count_min_t* crear_count_min(size_t tamanio);

//Destruye completamente count_min_sketch 
//Pre: Count_min_sketch fue creado
//Post: Destruyo Count_min_sketch
void count_min_destruir(count_min_t* count_min);

//Cuenta la aparcion de un const char*, devuelve true si fue exitoso, false en caso contrario
//Pre: count_min_sketch fue creado
//Post: Se conto la aparicion del const char* recibido por la funcion en caso de exito.
bool count_min_almacenar_aparicion(count_min_t* count_min, const char* hashtag);

//Devuelve  aproximadamente la cantidad de apariciones totales del const char* dado por parametro
//Pre: count_min_sketch fue creado
//Post: Devuelve  aproximadamente la cantidad de apariciones totales, en caso contrario devuelve NULL
void* count_min_cant_apariciones(count_min_t* count_min, const char* hashtag);

//Obtiene el hash de la count_min_sketch
//Pre: count_min_sketch fue creado
//Post: Devuelve el hash
hash_t* obtener_hash(count_min_t* count_min);

//Destruye el hash y lo remplaza por un hash nuevo, devuelve true de ser exitoso, false en caso contrario
//Post: El hash viejo se destruyo y se creo una nuevo 
bool destruir_y_crear_hash(count_min_t* count_min);
