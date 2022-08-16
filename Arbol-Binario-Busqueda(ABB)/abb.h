#include <stdio.h>
#include <stdbool.h>


/* ******************************************************************
 *                          ESTRUCTURAS                             *
 * *****************************************************************/


typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);


/* ******************************************************************
 *              PRIMITIVAS DEl ABB                                  *
 * *****************************************************************/

// Crea el abb, si falla devulve NULL
// Post: El abb fue creado
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);


// Guarda la clave y el valor en el abb, devuelve true si se guardo correctamente
// Si la clave ya existe, reemplara su dato viejo por el nuevo.
// false si algo salio mal
// Pre: El abb fue creado
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);



// Borra el elemento con la clave que recibe si existe en el abb. La funcion, devuelve un puntero 
// al dato del elemento, si fallo devuelve NULL;
// Pre: El arbol fue creado
// Post: El elemento fue borrado.
void *abb_borrar(abb_t *arbol, const char *clave);

// Obtiene el elemento buscado y devuelve un puntero al dato, si no se encuentra devuelve NULL.
// Pre: El abb fue creado
// Post: Se devolvio su valor si se encontraba al abb
void *abb_obtener(const abb_t *arbol, const char *clave);

// Devuelve true si la clave se encuantra en el abb, de lo contrario false;
// Pre: El abb fue creado
bool abb_pertenece(const abb_t *arbol, const char *clave);

//Devuelve la cantidad de elementos que posee el abb
//Pre: el abb fue creado
size_t abb_cantidad(const abb_t *arbol);

//Destruye el abb y a cada dato del abb le aplica la funcion abb_destruir_dato 
//Post: El abb fue destruido
void abb_destruir(abb_t *arbol);

// Crea el abb iterador, devuelve el iterador o en caso de haber fallado NULL.
// Post: Se creo el iterador
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

//Avanza el iterador una posicion, recorre el abb de la forma Inorder. Devuelve true si se anvanzo. False en 
// caso contrario
// Pre: El iterador fue creado 
// Post: Se avanzo una posicion
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Devuelve la clave en donde se encuentra el iterador, NULL en caso de encontrarse al final del abb
// Pre: El iterador fue creado
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Devuelve true si el iterador se encuentra al final, en caso contrario false
// Pre: El iterador fue creado
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Destruye el iterador
// Post: Se destruyo el iterador
void abb_iter_in_destruir(abb_iter_t* iter);
