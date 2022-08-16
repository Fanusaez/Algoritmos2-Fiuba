/*
 * prueba_abb.c
 * Pruebas para el tipo de dato abstracto Tabla de abb
 * Copyright: (2011) Margarita Manterola, Maximiliano Curia
 * Licencia: CC-BY-SA 2.5 (ar) ó CC-BY-SA 3.0
 */

#include "abb.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);


/* ******************************************************************
 *                 FUNCIONES VISITAR ITERADOR INTERNO
 * *****************************************************************/

bool suma(const char* clave, void* dato, void* extra){

    *(int *)extra += *(int *)dato;
    return true;
}

bool sumar_hasta_elem_e(const char* clave, void *dato, void *extra)
{
    const char* clave_stop = "e";
    if(strcmp(clave, clave_stop) == 0) return false;
    *(int *)extra += *(int *)dato;
    return true;
}





/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

static void prueba_crear_abb_vacio()
{
    abb_t* abb = abb_crear(strcmp,NULL);

    print_test("Prueba abb crear abb vacio", abb);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
    print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
    print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

    abb_destruir(abb);
}

static void prueba_iterar_abb_vacio()
{
    abb_t* abb = abb_crear(strcmp,NULL);
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iter crear iterador abb vacio", iter);
    print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));
    print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

static void prueba_abb_insertar()
{
    abb_t* abb = abb_crear(strcmp,NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
  
    /* Inserta otros 2 valores y no los borra (se destruyen con el abb) */
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb pertenece clave2, es true", abb_pertenece(abb, clave2));

    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));

    abb_destruir(abb);

}

static void prueba_abb_reemplazar()
{
    abb_t* abb = abb_crear(strcmp,NULL);

    char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

    /* Inserta 2 valores y luego los reemplaza */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    abb_destruir(abb);
}

static void prueba_abb_reemplazar_con_destruir()
{
    abb_t* abb = abb_crear(strcmp,NULL);

    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    /* Pide memoria para 4 valores */
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    /* Se destruye el abb (se debe liberar lo que quedó dentro) */
    abb_destruir(abb);
    free(valor1a);
    free(valor1b);
    free(valor2a);
    free(valor2b);
}

static void prueba_abb_borrar()
{
    abb_t* abb = abb_crear(strcmp,NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";
    /* Inserta 3 valores y luego los borra */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));

    /* Al borrar cada elemento comprueba que ya no está pero los otros sí. */
    print_test("Prueba abb pertenece clave3, es verdadero", abb_pertenece(abb, clave3));
    print_test("Prueba abb borrar clave3, es valor3", abb_borrar(abb, clave3) == valor3);
    print_test("Prueba abb borrar clave3, es NULL", !abb_borrar(abb, clave3));
    print_test("Prueba abb pertenece clave3, es falso", !abb_pertenece(abb, clave3));
    print_test("Prueba abb obtener clave3, es NULL", !abb_obtener(abb, clave3));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb pertenece clave1, es verdadero", abb_pertenece(abb, clave1));
   
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    
    print_test("Prueba abb borrar clave1, es NULL", !abb_borrar(abb, clave1));
    print_test("Prueba abb pertenece clave1, es falso", !abb_pertenece(abb, clave1));
    print_test("Prueba abb obtener clave1, es NULL", !abb_obtener(abb, clave1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);

    print_test("Prueba abb pertenece clave2, es verdadero", abb_pertenece(abb, clave2));
    print_test("Prueba abb borrar clave2, es valor2", abb_borrar(abb, clave2) == valor2);
    print_test("Prueba abb borrar clave2, es NULL", !abb_borrar(abb, clave2));
    print_test("Prueba abb pertenece clave2, es falso", !abb_pertenece(abb, clave2));
    print_test("Prueba abb obtener clave2, es NULL", !abb_obtener(abb, clave2));
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    abb_destruir(abb);
}

static void prueba_abb_clave_vacia()
{
    abb_t* abb = abb_crear(strcmp,NULL);

    char *clave = "", *valor = "";

    print_test("Prueba abb insertar clave vacia", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    abb_destruir(abb);
}

static void prueba_abb_valor_null()
{
    abb_t* abb = abb_crear(strcmp,NULL);

    char *clave = "", *valor = NULL;

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave vacia valor NULL", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor NULL", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor NULL", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    abb_destruir(abb);
}

static void prueba_abb_volumen(size_t largo){ // ivan
    printf("PRUEBA ABB VOLUMEN\n");
    abb_t* abb = abb_crear(strcmp,free);

    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    unsigned* valores[largo];


    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        sprintf(claves[i], "%08d", i);
        *valores[i] = i;
    }

    while(ok && abb_cantidad(abb) != largo){
        unsigned ind = rand()%(unsigned)largo;
        if(abb_pertenece(abb, claves[ind])) continue;
        ok = abb_guardar(abb, claves[ind], valores[ind]);
    }

    print_test("Prueba abb almacenar muchos elementos", ok);
    print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    for (size_t i = 0; i < largo; i++) {
        ok = abb_pertenece(abb, claves[i]);
        if (!ok) break;
        ok = abb_obtener(abb, claves[i]) == valores[i];
        if (!ok) break;
    }

    print_test("Prueba abb pertenece y obtener muchos elementos", ok);
    print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    for (size_t i = 0; i < largo; i++) {
        ok = abb_borrar(abb, claves[i]) == valores[i];
        free(valores[i]);
        if (!ok) break;
    }

    print_test("Prueba abb borrar muchos elementos", ok);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
    free(claves);

}

  

static ssize_t buscar(const char* clave, char* claves[], size_t largo)
{
    for (size_t i = 0; i < largo; i++) {
        if (strcmp(clave, claves[i]) == 0) return (ssize_t) i;
    }
    return -1;
}

static void prueba_abb_iterar()
{
    abb_t* abb = abb_crear(strcmp,NULL);

    char *claves[] = {"perro", "gato", "vaca"};
    char *valores[] = {"guau", "miau", "mu"};

    /* Inserta 3 valores */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, claves[0], valores[0]));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, claves[1], valores[1]));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, claves[2], valores[2]));

    // Prueba de iteración sobre las claves almacenadas.
    abb_iter_t* iter = abb_iter_in_crear(abb);
    const char *clave;
    ssize_t indice;

    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Primer valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Segundo valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Tercer valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    abb_iter_in_avanzar(iter);
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    /* Vuelve a tratar de avanzar, por las dudas */
    print_test("Prueba abb iterador ver actual, es NULL", !abb_iter_in_ver_actual(iter));
    print_test("Prueba abb iterador avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

static void prueba_abb_iterar_volumen(size_t largo)// ivan
{
    abb_t* abb = abb_crear(strcmp,free);

    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    unsigned* valores[largo];

    /* Inserta 'largo' parejas en el abb */
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        sprintf(claves[i], "%08d", i);
        *valores[i] = i;
    }

    while(ok && abb_cantidad(abb) != largo){
        unsigned ind = rand()%(unsigned)largo;
        if(abb_pertenece(abb, claves[ind])) continue;
        ok = abb_guardar(abb, claves[ind], valores[ind]);
    }

    // Prueba de iteración sobre las claves almacenadas.
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    ok = true;
    unsigned i;
    const char *clave;
    size_t *valor;

    for (i = 0; i < largo; i++) {
        if ( abb_iter_in_al_final(iter) ) {
            ok = false;
            break;
        }
        clave = abb_iter_in_ver_actual(iter);
        if ( clave == NULL ) {
            ok = false;
            break;
        }
        valor = abb_obtener(abb, clave);
        if ( valor == NULL ) {
            ok = false;
            break;
        }
        abb_iter_in_avanzar(iter);
    }
    print_test("Prueba abb iteración en volumen", ok);
    print_test("Prueba abb iteración en volumen, recorrio todo el largo", i == largo);
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));



    free(claves);
    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}


static void borrar_sin_hijos(){

    abb_t* abb = abb_crear(strcmp, NULL);
    char* clave1 = "a";
    char* clave2 = "b";
    char* clave3 = "c";
    char* clave4 = "d";
    char* clave5 = "e";
    char* clave6 = "f";
    char* clave7 = "g";
    size_t dato1 = 1, dato2 = 2, dato3 = 3, dato4 = 4, dato5 = 5, dato6 = 6, dato7 = 7;
    print_test("Prueba abb insertar clave4", abb_guardar(abb, clave4, &dato4));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, &dato3));
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, &dato1));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, &dato2));
    print_test("Prueba abb insertar clave5", abb_guardar(abb, clave5, &dato5));
    print_test("Prueba abb insertar clave7", abb_guardar(abb, clave7, &dato7));
    print_test("Prueba abb insertar clave6", abb_guardar(abb, clave6, &dato6));

    print_test("Prueba abb borrar clave2, es valor2", abb_borrar(abb, clave2) == &dato2);
    print_test("Prueba abb pertenece  clave2, es false", !abb_pertenece(abb, clave2));

    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == &dato1);
    print_test("Prueba abb pertenece clave1, es false", !abb_pertenece(abb, clave1));

    print_test("Prueba abb borrar clave3, es valor3", abb_borrar(abb, clave3) == &dato3);
    print_test("Prueba abb pertenece clave3, es false", !abb_pertenece(abb, clave3));

    print_test("Prueba abb borrar clave6, es valor6", abb_borrar(abb, clave6) == &dato6);
    print_test("Prueba abb pertenece clave6, es false", !abb_pertenece(abb, clave6));

    print_test("Prueba abb borrar clave7, es valor7", abb_borrar(abb, clave7) == &dato7);
    print_test("Prueba abb pertenece clave7, es false", !abb_pertenece(abb, clave7));

    print_test("Prueba abb pertenece clave4, es true", abb_pertenece(abb, clave4));
    print_test("Prueba abb borrar clave4, es valor4", abb_borrar(abb, clave4) == &dato4);
    print_test("Prueba abb pertenece clave4, es false", !abb_pertenece(abb, clave4));
    
    abb_destruir(abb);
}

static void borrar_unico_hijo(){

    char* clave1 = "a", *clave2 = "b", *clave3 = "c", *clave4 = "d", *clave5 = "e", *clave6 = "f", *clave7 = "g";
    char* dato1 = "1", *dato2 = "2", *dato3 = "3", *dato4 = "4", *dato5 = "5", *dato6 = "6", *dato7 = "7";

    abb_t* abb = abb_crear(strcmp, NULL);

    print_test("Prueba abb insertar clave4", abb_guardar(abb, clave4, dato4));
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, dato1));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, dato2));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, dato3));
    print_test("Prueba abb insertar clave7", abb_guardar(abb, clave7, dato7));
    print_test("Prueba abb insertar clave5", abb_guardar(abb, clave5, dato5));
    print_test("Prueba abb insertar clave6", abb_guardar(abb, clave6, dato6));
    print_test("Prueba abb la cantidad de elementos es 7", abb_cantidad(abb) == 7);
    print_test("Prueba abb borrar clave2, es valor2", abb_borrar(abb, clave2) == dato2);
    print_test("Prueba abb pertenece clave2, es false", !abb_pertenece(abb, clave2));
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == dato1);
    print_test("Prueba abb pertenece clave1, es false", !abb_pertenece(abb, clave1));
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));
    print_test("Prueba abb la cantidad de elementos es 5", abb_cantidad(abb) == 5);
    print_test("Prueba abb borrar clave7, es valor7", abb_borrar(abb, clave7) == dato7);
    print_test("Prueba abb pertenece clave5, es true", abb_pertenece(abb, clave5));
    print_test("Prueba abb pertenece clave6, es true", abb_pertenece(abb, clave6));
    print_test("Prueba abb borrar clave3, es valor3", abb_borrar(abb, clave3) == dato3);
    print_test("Prueba abb borrar clave4, es valor", abb_borrar(abb, clave4) == dato4);
    print_test("Prueba abb pertenece clave4, es false", !abb_pertenece(abb, clave4));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb borrar clave5, es valor5", abb_borrar(abb, clave5) == dato5);
    print_test("Prueba abb pertenece clave5, es false", !abb_pertenece(abb, clave4));
    print_test("Prueba abb pertenece clave6, es true", abb_pertenece(abb, clave6));
    print_test("Prueba abb borrar clave6, es valor6", abb_borrar(abb, clave6) == dato6);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    abb_destruir(abb);



}

static void borrar_dos_hijos(){


    char* clave1 = "a", *clave2 = "b", *clave3 = "c", *clave4 = "d", *clave5 = "e", *clave6 = "f", *clave7 = "g";
    char* clave8 = "h", *clave9 = "i", *clave10 = "j", *clave11 = "k";

    char* dato1 = "1", *dato2 = "2", *dato3 = "3", *dato4 = "4", *dato5 = "5", *dato6 = "6", *dato7 = "7";
    char* dato8 = "8", *dato9 = "9", *dato10 = "10", *dato11 = "11";



    abb_t* abb = abb_crear(strcmp, NULL);

    print_test("Prueba abb insertar clave6", abb_guardar(abb, clave6, dato6));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, dato2));
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, dato1));
    print_test("Prueba abb insertar clave4", abb_guardar(abb, clave4, dato4));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, dato3));
    print_test("Prueba abb insertar clave5", abb_guardar(abb, clave5, dato5));
    print_test("Prueba abb insertar clave8", abb_guardar(abb, clave8, dato8));
    print_test("Prueba abb insertar clave7", abb_guardar(abb, clave7, dato7));
    print_test("Prueba abb insertar clave10", abb_guardar(abb, clave10, dato10));
    print_test("Prueba abb insertar clave9", abb_guardar(abb, clave9, dato9));
    print_test("Prueba abb insertar clave11", abb_guardar(abb, clave11, dato11));
    print_test("Prueba abb la cantidad de elementos es 11", abb_cantidad(abb) == 11);
    
    print_test("Prueba abb borrar clave8, es valor8", abb_borrar(abb, clave8) == dato8);
    print_test("Prueba abb pertenece clave8, es false", !abb_pertenece(abb, clave8));
    print_test("Prueba abb pertenece clave10, es true", abb_pertenece(abb, clave10));
    print_test("Prueba abb pertenece clave9, es true", abb_pertenece(abb, clave9));
    print_test("Prueba abb pertenece clave11, es true", abb_pertenece(abb, clave11));
    print_test("Prueba abb la cantidad de elementos es 10", abb_cantidad(abb) == 10);

    print_test("Prueba abb borrar clave9, es valor9", abb_borrar(abb, clave9) == dato9);
    print_test("Prueba abb pertenece clave9, es false", !abb_pertenece(abb, clave9));
    print_test("Prueba abb pertenece clave7, es true", abb_pertenece(abb, clave7));
    print_test("Prueba abb pertenece clave10, es true", abb_pertenece(abb, clave10));
    print_test("Prueba abb pertenece clave11, es true", abb_pertenece(abb, clave11));
    print_test("Prueba abb la cantidad de elementos es 9", abb_cantidad(abb) == 9);

    print_test("Prueba abb borrar clave10, es valor10", abb_borrar(abb, clave10) == dato10);
    print_test("Prueba abb pertenece clave7, es true", abb_pertenece(abb, clave7));
    print_test("Prueba abb pertenece clave11, es true", abb_pertenece(abb, clave11));
    print_test("Prueba abb la cantidad de elementos es 8", abb_cantidad(abb) == 8);

    print_test("Prueba abb borrar clave4, es valor4", abb_borrar(abb, clave4) == dato4);
    print_test("Prueba abb pertenece clave4, es false", !abb_pertenece(abb, clave4));
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));
    print_test("Prueba abb pertenece clave5, es true", abb_pertenece(abb, clave5));
    print_test("Prueba abb la cantidad de elementos es 7", abb_cantidad(abb) == 7);

    print_test("Prueba abb borrar clave2, es valor2", abb_borrar(abb, clave2) == dato2);
    print_test("Prueba abb pertenece clave2, es false", !abb_pertenece(abb, clave2));
    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));
    print_test("Prueba abb pertenece clave5, es true", abb_pertenece(abb, clave5));
    print_test("Prueba abb la cantidad de elementos es 6", abb_cantidad(abb) == 6);

    print_test("Prueba abb borrar clave6, es valor6", abb_borrar(abb, clave6) == dato6);
    print_test("Prueba abb pertenece clave6, es false", !abb_pertenece(abb, clave6));
    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));
    print_test("Prueba abb pertenece clave5, es true", abb_pertenece(abb, clave5));
    print_test("Prueba abb pertenece clave7, es true", abb_pertenece(abb, clave7));
    print_test("Prueba abb pertenece clave11, es true", abb_pertenece(abb, clave11));
    print_test("Prueba abb la cantidad de elementos es 5", abb_cantidad(abb) == 5);

    print_test("Prueba abb borrar clave7, es valor7", abb_borrar(abb, clave7) == dato7);
    print_test("Prueba abb pertenece clave7, es false", !abb_pertenece(abb, clave7));
    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));
    print_test("Prueba abb pertenece clave5, es true", abb_pertenece(abb, clave5));
    print_test("Prueba abb pertenece clave11, es true", abb_pertenece(abb, clave11));
    print_test("Prueba abb la cantidad de elementos es 4", abb_cantidad(abb) == 4);

    print_test("Prueba abb borrar clave3, es valor3", abb_borrar(abb, clave3) == dato3);
    print_test("Prueba abb pertenece clave3, es false", !abb_pertenece(abb, clave3));
    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Prueba abb pertenece clave5, es true", abb_pertenece(abb, clave5));
    print_test("Prueba abb pertenece clave11, es true", abb_pertenece(abb, clave11));
    print_test("Prueba abb la cantidad de elementos es 3", abb_cantidad(abb) == 3);

    print_test("Prueba abb borrar clave5, es valor5", abb_borrar(abb, clave5) == dato5);
    print_test("Prueba abb pertenece clave5, es false", !abb_pertenece(abb, clave5));
    print_test("Prueba abb pertenece clave11, es true", abb_pertenece(abb, clave11));
    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Destruyo el abb\n", true);
    abb_destruir(abb);

}

static void prueba_iterdor_interno_sin_corte(){

    char* clave1 = "a", *clave2 = "b", *clave3 = "c", *clave4 = "d", *clave5 = "e", *clave6 = "f", *clave7 = "g";
    char* clave8 = "h", *clave9 = "i", *clave10 = "j", *clave11 = "k";

    int* dato1 = malloc(10 * sizeof(int));
    int* dato2 = malloc(10 * sizeof(int));
    int* dato3 = malloc(10 * sizeof(int));
    int* dato4 = malloc(10 * sizeof(int));
    int* dato5 = malloc(10 * sizeof(int));
    int* dato6 = malloc(10 * sizeof(int));
    int* dato7 = malloc(10 * sizeof(int));
    int* dato8 = malloc(10 * sizeof(int));
    int* dato9 = malloc(10 * sizeof(int));
    int* dato10 = malloc(10 * sizeof(int));
    int* dato11 = malloc(10 * sizeof(int));

    *dato1 = 1, *dato2 = 2,  *dato3 = 3, *dato4 = 4, *dato5 = 5, *dato6 = 6, *dato7 = 7, *dato8 = 8, *dato9 = 9, *dato10 = 10, *dato11 = 11;
    size_t extra = 0;
    abb_t* abb = abb_crear(strcmp, free);

    print_test("Prueba abb insertar clave6", abb_guardar(abb, clave6, dato6));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, dato2));
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, dato1));
    print_test("Prueba abb insertar clave4", abb_guardar(abb, clave4, dato4));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, dato3));
    print_test("Prueba abb insertar clave5", abb_guardar(abb, clave5, dato5));
    print_test("Prueba abb insertar clave8", abb_guardar(abb, clave8, dato8));
    print_test("Prueba abb insertar clave7", abb_guardar(abb, clave7, dato7));
    print_test("Prueba abb insertar clave10", abb_guardar(abb, clave10, dato10));
    print_test("Prueba abb insertar clave9", abb_guardar(abb, clave9, dato9));
    print_test("Prueba abb insertar clave11", abb_guardar(abb, clave11, dato11));
    print_test("Prueba abb la cantidad de elementos es 11", abb_cantidad(abb) == 11);
    print_test("Llamo al iterador internio 'abb_in_order' y por parametro a la funcion sumar" , true);
    int suma_correcta = 66;
    abb_in_order(abb, suma, &extra);
    print_test("La suma total es correcta", extra == suma_correcta);
    abb_destruir(abb);
}

static void prueba_iterador_interno_con_corte(){

    char* clave1 = "a", *clave2 = "b", *clave3 = "c", *clave4 = "d", *clave5 = "e", *clave6 = "f", *clave7 = "g";
    char* clave8 = "h", *clave9 = "i", *clave10 = "j", *clave11 = "k";

    int* dato1 = malloc(10 * sizeof(int));
    int* dato2 = malloc(10 * sizeof(int));
    int* dato3 = malloc(10 * sizeof(int));
    int* dato4 = malloc(10 * sizeof(int));
    int* dato5 = malloc(10 * sizeof(int));
    int* dato6 = malloc(10 * sizeof(int));
    int* dato7 = malloc(10 * sizeof(int));
    int* dato8 = malloc(10 * sizeof(int));
    int* dato9 = malloc(10 * sizeof(int));
    int* dato10 = malloc(10 * sizeof(int));
    int* dato11 = malloc(10 * sizeof(int));

    *dato1 = 1, *dato2 = 2,  *dato3 = 3, *dato4 = 4, *dato5 = 5, *dato6 = 6, *dato7 = 7, *dato8 = 8, *dato9 = 9, *dato10 = 10, *dato11 = 11;
    size_t extra = 0;
    abb_t* abb = abb_crear(strcmp, free);

    print_test("Prueba abb insertar clave6", abb_guardar(abb, clave6, dato6));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, dato2));
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, dato1));
    print_test("Prueba abb insertar clave4", abb_guardar(abb, clave4, dato4));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, dato3));
    print_test("Prueba abb insertar clave5", abb_guardar(abb, clave5, dato5));
    print_test("Prueba abb insertar clave8", abb_guardar(abb, clave8, dato8));
    print_test("Prueba abb insertar clave7", abb_guardar(abb, clave7, dato7));
    print_test("Prueba abb insertar clave10", abb_guardar(abb, clave10, dato10));
    print_test("Prueba abb insertar clave9", abb_guardar(abb, clave9, dato9));
    print_test("Prueba abb insertar clave11", abb_guardar(abb, clave11, dato11));
    print_test("Prueba abb la cantidad de elementos es 11", abb_cantidad(abb) == 11);
    print_test("Llamo al iterador internio 'abb_in_order' y por parametro a la funcion sumar_hasta_elem_e" , true);
    int suma_correcta = 10;
    abb_in_order(abb, sumar_hasta_elem_e, &extra);
    print_test("La suma total es correcta", extra == suma_correcta);
    abb_destruir(abb);

}





/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/


void pruebas_abb_estudiante()
{
    /* Ejecuta todas las pruebas unitarias. */
    prueba_crear_abb_vacio();
    prueba_iterar_abb_vacio();
    prueba_abb_insertar();
    prueba_abb_reemplazar();
    prueba_abb_reemplazar_con_destruir();
    prueba_abb_borrar();
    prueba_abb_clave_vacia();
    prueba_abb_valor_null();
    prueba_abb_volumen(5000);
    prueba_abb_iterar();
    prueba_abb_iterar_volumen(5000);
    borrar_sin_hijos();
    borrar_unico_hijo();
    borrar_dos_hijos();
    prueba_iterdor_interno_sin_corte();
    prueba_iterador_interno_con_corte();
}


#ifndef CORRECTOR // Para que no dé conflicto con el main() del corrector.

int main(void)
{
    pruebas_abb_estudiante();
    return failure_count() > 0; // Indica si falló alguna prueba.
}

#endif