#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "testing.h"
#define PRUEBA_VOL_1 100
#define PRUEBA_VOL_2 1000
#define PRUEBA_VOL_3 10000

// *****************Funciones para iterador interno****************************************

bool sumar_hasta_elem_cinco(void *dato, void *extra)
{
    if (*(int *)dato == 5)
        return false;
    *(int *)extra += *(int *)dato;
    return true;
}

bool sumar_todos(void *dato, void *extra)
{

    *(int *)extra += *(int *)dato;
    return true;
}



//******************************************************************************************

static void pruebas_condiciones_iniciales(void)
{

    printf("\nINICIO PRUEBAS CONDICIONES INCIALES\n");

    lista_t *lista = lista_crear();
    print_test("Se creo la lista", true);
    print_test("La lista esta incicialmente vacia", lista_esta_vacia(lista) == true);
    print_test("Borrar primero es NULL", lista_borrar_primero(lista) == NULL);
    print_test("Ver primero es NULL", lista_ver_primero(lista) == NULL);
    print_test("Lista ver ultimo es NULL", lista_ver_ultimo(lista) == NULL);
    print_test("Largo de la lista es cero", lista_largo(lista) == 0);
    lista_destruir(lista, NULL);
    print_test("La lista se destruyo correctamente", true);
}

static void pruebas_invariante_lista(void)
{

    int int1 = 1;
    int *p_int1 = &int1;
    int int2 = 2;
    int *p_int2 = &int2;
    char character = 'a';
    char *p_character = &character;

    printf("\n INICIO PRUEBAS INVARIANTE LISTA \n");

    lista_t *lista = lista_crear();
    print_test("La lista se creo correctamente", true);
    print_test("Inserto primero 'p_entero'  a la lista", lista_insertar_primero(lista, p_int1) == true);
    print_test("Lista no esta vacia", lista_esta_vacia(lista) == false);
    print_test("El primero es 'p_int' ", lista_ver_primero(lista) == p_int1);
    print_test("Largo es '1' ", lista_largo(lista) == 1);
    print_test("Inserto ultimo 'p_int2' ", lista_insertar_ultimo(lista, p_int2) == true);
    print_test("Primero es 'p_int1' ", lista_ver_primero(lista) == p_int1);
    print_test("Ultimo es 'p_int2' ", lista_ver_ultimo(lista) == p_int2);
    print_test("Inserto primero 'p_character'", lista_insertar_primero(lista, p_character) == true);
    print_test("Primero es 'p_character", lista_ver_primero(lista) == p_character);
    print_test("Largo es 3", lista_largo(lista) == 3);
    print_test("Borro primero", lista_borrar_primero(lista) == p_character);
    print_test("Primero es 'p_int1' ", lista_ver_primero(lista) == p_int1);
    print_test("Ultimo es 'p_int2'", lista_ver_ultimo(lista) == p_int2);
    print_test("Borro primero", lista_borrar_primero(lista) == p_int1);
    print_test("Primero es 'p_int2' ", lista_ver_primero(lista) == p_int2);
    print_test("Borro primero", lista_borrar_primero(lista) == p_int2);
    print_test("La lista esta vacia", lista_esta_vacia(lista) == true);
    lista_destruir(lista, NULL);
    print_test("La lista se destruyo", true);
}

static void pruebas_invariante_lista_NULL(void)
{

    int int1 = 1;
    int *p_int1 = &int1;
    int int2 = 2;
    int *p_int2 = &int2;
    char character = 'a';
    char *p_character = &character;

    printf("\n INICIO PRUEBAS INVARIANTE LISTA CON NULL \n");

    lista_t *lista = lista_crear();
    print_test("La lista se creo correctamente", true);
    print_test("Inserto primero 'p_entero'  a la lista", lista_insertar_primero(lista, p_int1) == true);
    print_test("Inserto NULL ultimo", lista_insertar_ultimo(lista, NULL) == true);
    print_test("Largo es '2' ", lista_largo(lista) == 2);
    print_test("Lista no esta vacia", lista_esta_vacia(lista) == false);
    print_test("El primero es 'p_int' ", lista_ver_primero(lista) == p_int1);
    print_test("Inserto ultimo 'p_int2' ", lista_insertar_ultimo(lista, p_int2) == true);
    print_test("Primero es 'p_int1' ", lista_ver_primero(lista) == p_int1);
    print_test("Ultimo es 'p_int2' ", lista_ver_ultimo(lista) == p_int2);
    print_test("Inserto primero 'p_character'", lista_insertar_primero(lista, p_character) == true);
    print_test("Inserto NULL primero", lista_insertar_primero(lista, NULL) == true);
    print_test("Primero es NULL", lista_ver_primero(lista) == NULL);
    print_test("Longitud lista es 5", lista_largo(lista) == 5);
    print_test("Primero es NULL", lista_ver_primero(lista) == NULL);
    print_test("Borro primero", lista_borrar_primero(lista) == NULL);
    print_test("Primero es 'p_character", lista_ver_primero(lista) == p_character);
    print_test("Largo es 4", lista_largo(lista) == 4);
    print_test("Borro primero", lista_borrar_primero(lista) == p_character);
    print_test("Primero es 'p_int1' ", lista_ver_primero(lista) == p_int1);
    print_test("Ultimo es 'p_int2'", lista_ver_ultimo(lista) == p_int2);
    print_test("Borro primero", lista_borrar_primero(lista) == p_int1);
    print_test("Primero es 'NULL' ", lista_ver_primero(lista) == NULL);
    print_test("Borro primero", lista_borrar_primero(lista) == NULL);
    print_test("Borro primero", lista_borrar_primero(lista) == p_int2);
    print_test("La lista esta vacia", lista_esta_vacia(lista) == true);
    lista_destruir(lista, NULL);
    print_test("La lista se destruyo", true);
}

static void pruebas_lista_vacia_destruccion_completa()
{

    printf("\n INICIO PRUEBAS LISTA VACIA DESTRUCCION COMPLETA \n");
    lista_t *lista = lista_crear();
    print_test("La lista esta vacia", lista_esta_vacia(lista) == true);
    lista_destruir(lista, free);
    print_test("La lista se destruyo completamente", true);
}

static void pruebas_lista_vacia_destruccion_incompleta()
{

    printf("\n INICIO PRUEBAS LISTA VACIA DESTRUCCION INCOMPLETA \n");
    lista_t *lista = lista_crear();
    print_test("La lista esta vacia", lista_esta_vacia(lista) == true);
    lista_destruir(lista, NULL);
    print_test("La lista se destruyo", true);
}

static void pruebas_lista_destruccion_completa()
{

    printf("\n INICIO PRUEBAS LISTA DESTRUCCION COMPLETA \n");

    int *p_entero = malloc(sizeof(int));
    *p_entero = 1;
    char *p_character = malloc(sizeof(char));
    *p_character = 'a';

    lista_t *lista = lista_crear();
    print_test("La lista se creo correctamente", true);
    print_test("Inserto 'p_entero' primero", lista_insertar_primero(lista, p_entero) == true);
    print_test("Primero es 'p_entero' ", lista_ver_primero(lista) == p_entero);
    print_test("Inserto ultimo 'p_character'", lista_insertar_ultimo(lista, p_character) == true);
    print_test("Lista largo es 2", lista_largo(lista) == 2);
    lista_destruir(lista, free);
    print_test("La lista fue desstruida completamente", true);
}

static void pruebas_lista_destruccion_incompleta()
{

    printf("\n INICIO PRUEBAS LISTA DESTRUCCION INCOMPLETA \n");

    int *p_entero = malloc(sizeof(int));
    *p_entero = 1;
    char *p_character = malloc(sizeof(char));
    *p_character = 'a';

    lista_t *lista = lista_crear();
    print_test("La lista se creo correctamente", true);
    print_test("Inserto 'p_entero' primero", lista_insertar_primero(lista, p_entero) == true);
    print_test("Primero es 'p_entero' ", lista_ver_primero(lista) == p_entero);
    print_test("Inserto ultimo 'p_character'", lista_insertar_ultimo(lista, p_character) == true);
    print_test("Lista largo es 2", lista_largo(lista) == 2);
    lista_destruir(lista, NULL);
    print_test("La lista fue desstruida", true);
    printf("Libero memoria pedida a mano, para que no haya perdidas de memoria\n");
    free(p_entero);
    free(p_character);
}

static void pruebas_volumen_lista(size_t cantidad)
{

    printf("\nINICIO PRUEBAS VOLUMEN LISTA CON %li ELEMENTOS\n", cantidad);

    lista_t *lista = lista_crear();
    int vector[cantidad];
    bool ok = true;

    for (size_t i = 0; i < cantidad; i++)
    {
        vector[i] = i;
        ok &= (lista_insertar_ultimo(lista, &vector[i]) == true);
    }

    print_test("Inserto todos los punteros a los elementos del vector", ok);

    for (size_t i = 0; i < cantidad; i++)
    {
        ok &= (lista_ver_primero(lista) == &vector[i]);
        ok &= (lista_borrar_primero(lista) == &vector[i]);
    }
    print_test("Todos los punteros borrados con posicion 'i' eran &vector[i]", ok);

    lista_destruir(lista, NULL);
    print_test("Destruyo la lista", true);
}

static void pruebas_volumen_lista_al_reves(size_t cantidad)
{

    printf("\nINICIO PRUEBAS VOLUMEN AL REVES LISTA CON %li ELEMENTOS\n", cantidad);
    lista_t *lista = lista_crear();
    size_t vector[cantidad];
    bool ok = true;

    for (size_t i = 0; i < cantidad; i++)
    {
        vector[i] = i;
        ok &= (lista_insertar_primero(lista, &vector[i]) == true);
    }

    print_test("Inserto todos los punteros a los elementos del vector", ok);

    for (int i = (int)cantidad - 1; i >= 0; i--)
    {
        ok &= (lista_ver_primero(lista) == &vector[i]);
        ok &= (lista_borrar_primero(lista) == &vector[i]);
    }
    print_test("Todos los punteros borrados con posicion 'i' eran &vector[i]", ok);

    lista_destruir(lista, NULL);
    print_test("Destruyo la lista", true);
}

static void pruebas_iterador_externo_condiciones_iniciales()
{

    printf("\n INICIO PRUEBAS ITERADOR EXTERNO CONDICIONES INICIALES \n");

    lista_t *lista = lista_crear();
    print_test("Se creo la lista", true);
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("Se creo el iterador", true);
    print_test("Ver actual sobre una lista vacia es NULL", lista_iter_ver_actual(iter) == NULL);
    print_test("Avanzar sobre una lista vacia es NULL", lista_iter_avanzar(iter) == false);
    print_test("Iter al final es NULL", lista_iter_al_final(iter) == true);
    print_test("Borrar sobre una lista vacia es NULL", lista_iter_borrar(iter) == NULL);
    lista_iter_destruir(iter);
    print_test("Se destruyo el iterador", true);
    lista_destruir(lista, NULL);
    print_test("Se destruyo la lista", true);
}

static void pruebas_iterador_externo_insertar_inicio()
{

    printf("\n INICIO PRUEBAS ITERADOR EXTERNO ELEMENTO INICIO \n");

    lista_t *lista = lista_crear();
    print_test("Se creo la lista", true);
    int int1 = 1;
    int *p_int1 = &int1;
    int int2 = 2;
    int *p_int2 = &int2;
    int int3 = 3;
    int *p_int3 = &int3;
    print_test("Inserto en el ultimo 'p_int1' ", lista_insertar_ultimo(lista, p_int1) == true);
    print_test("Inserto en el ultimo 'p_int2' ", lista_insertar_ultimo(lista, p_int2) == true);
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("Se creo el iterador", true);
    print_test("Inserto 'p_int3' ", lista_iter_insertar(iter, p_int3) == true);
    print_test("Primero es p_int3", lista_ver_primero(lista) == p_int3);
    lista_iter_destruir(iter);
    print_test("Se destruyo el iterador", true);
    lista_destruir(lista, NULL);
    print_test("Se destruyo la lista", true);
}

static void pruebas_iterador_externo_insertar_final()
{

    printf("\n INICIO PRUEBAS ITERADOR EXTERNO ELEMENTO FINAL \n");

    lista_t *lista = lista_crear();
    print_test("Se creo la lista", true);
    int int1 = 1;
    int *p_int1 = &int1;
    int int2 = 2;
    int *p_int2 = &int2;
    int int3 = 3;
    int *p_int3 = &int3;
    print_test("Inserto en el ultimo 'p_int1' ", lista_insertar_ultimo(lista, p_int1) == true);
    print_test("Inserto en el ultimo 'p_int2' ", lista_insertar_ultimo(lista, p_int2) == true);
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("Se creo el iterador", true);
    print_test("Itero hasta el final de la lista", lista_iter_avanzar(iter) == true);
    lista_iter_avanzar(iter);
    print_test("El iterador esta a final", lista_iter_al_final(iter) == true);
    print_test("Inserto elemento al final", lista_iter_insertar(iter, p_int3) == true);
    print_test("El ultimo elemento es p_int3", lista_ver_ultimo(lista) == p_int3);
    lista_iter_destruir(iter);
    print_test("Se destruyo el iterador", true);
    lista_iter_t *iter_2 = lista_iter_crear(lista);
    print_test("Se creo el iterador", true);
    print_test("Lista iter actual es 'p_int1", lista_iter_ver_actual(iter_2) == p_int1);
    print_test("Avanzo una posicion en la lista", lista_iter_avanzar(iter_2) == true);
    print_test("Lista iter actual es 'p_int2'", lista_iter_ver_actual(iter_2) == p_int2);
    print_test("Avanzo otra posicion", lista_iter_avanzar(iter_2) == true);
    print_test("Lista iter actual es 'p_int3", lista_iter_ver_actual(iter_2) == p_int3);
    print_test("Avanzo una vez mas", lista_iter_avanzar(iter_2) == true);
    print_test("El iterador se encuentra al final", lista_iter_al_final(iter_2) == true);
    lista_iter_destruir(iter_2);
    print_test("Se destruyo el iterador", true);
    lista_destruir(lista, NULL);
    print_test("Se destruyo la lista", true);
}

static void pruebas_iterador_externo_insertar_medio()
{

    printf("\n INICIO PRUEBAS ITERADOR EXTERNO ELEMENTO INICIO \n");

    lista_t *lista = lista_crear();
    print_test("Se creo la lista", true);
    int int1 = 1;
    int *p_int1 = &int1;
    int int2 = 2;
    int *p_int2 = &int2;
    int int3 = 3;
    int *p_int3 = &int3;
    int int4 = 4;
    int *p_int4 = &int4;
    int int5 = 5;
    int *p_int5 = &int5;
    
    print_test("Inserto en el ultimo 'p_int1' ", lista_insertar_ultimo(lista, p_int1) == true);
    print_test("Inserto en el ultimo 'p_int2' ", lista_insertar_ultimo(lista, p_int2) == true);
    print_test("Inserto en el ultimo 'p_int3' ", lista_insertar_ultimo(lista, p_int3) == true);
    print_test("Inserto en el ultimo 'p_int4' ", lista_insertar_ultimo(lista, p_int4) == true);
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("Se creo el iterador", true);
    printf("Avanzo dos lugares hasta la mitad de la lista\n");
    print_test("Avanzo un lugar", lista_iter_avanzar(iter) == true);
    print_test("Azanzo otro lugar", lista_iter_avanzar(iter) == true);
    print_test("Inserto elemento 'p_int5'", lista_iter_insertar(iter, p_int5) == true);
    print_test("Borro primer elemento", lista_borrar_primero(lista) == p_int1);
    print_test("Borro primer elemento", lista_borrar_primero(lista) == p_int2);
    print_test("Ver primer elemento, es 'p_int5'", lista_ver_primero(lista) == p_int5);
    lista_iter_destruir(iter);
    print_test("Se destruyo el iterador", true);
    lista_destruir(lista, NULL);
    print_test("Se destruyo la lista", true);
}

static void pruebas_iterador_externo_borrar_inicio()
{

    printf("\n INICIO PRUEBAS ITERADOR EXTERNO BORRAR ELEMENTO INICIO \n");

    lista_t *lista = lista_crear();
    print_test("Se creo la lista", true);
    int int1 = 1;
    int *p_int1 = &int1;
    int int2 = 2;
    int *p_int2 = &int2;
    print_test("Inserto en el ultimo 'p_int1' ", lista_insertar_ultimo(lista, p_int1) == true);
    print_test("Inserto en el ultimo 'p_int2' ", lista_insertar_ultimo(lista, p_int2) == true);
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("Se creo el iterador", true);
    print_test("Ver actual es 'p_int1' ", lista_iter_ver_actual(iter) == p_int1);
    print_test("Borro elemento", lista_iter_borrar(iter) == p_int1);
    print_test("Ver actual es 'p_int2' ", lista_iter_ver_actual(iter) == p_int2);
    print_test("Borro elemento", lista_iter_borrar(iter) == p_int2);
    print_test("Lista esta vacia", lista_esta_vacia(lista) == true);
    print_test("Ver actual es 'null' ", lista_iter_ver_actual(iter) == NULL);
    lista_iter_destruir(iter);
    print_test("Se destruyo el iterador", true);
    lista_destruir(lista, NULL);
    print_test("Se destruyo la lista", true);
}

static void pruebas_iterador_externo_borrar_ultimo()
{

    printf("\n INICIO PRUEBAS ITERADOR EXTERNO BORRAR ELEMENTO FINAL \n");

    lista_t *lista = lista_crear();
    print_test("Se creo la lista", true);
    int int1 = 1;
    int *p_int1 = &int1;
    int int2 = 2;
    int *p_int2 = &int2;
    int int3 = 3;
    int *p_int3 = &int3;
    print_test("Inserto en el ultimo 'p_int1' ", lista_insertar_ultimo(lista, p_int1) == true);
    print_test("Inserto en el ultimo 'p_int2' ", lista_insertar_ultimo(lista, p_int2) == true);
    print_test("Inserto en el ultimo 'p_int3' ", lista_insertar_ultimo(lista, p_int3) == true);
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("Se creo el iterador", true);
    print_test("Avanzo una posicion", lista_iter_avanzar(iter) == true);
    print_test("Avanzo una posicion", lista_iter_avanzar(iter) == true);
    print_test("Estoy en el ultimo elemento ", lista_iter_ver_actual(iter) == p_int3);
    print_test("Borro ultimo elemento", lista_iter_borrar(iter) == p_int3);
    print_test("Iter esta al final", lista_iter_al_final(iter) == true);
    print_test("Inserto a lo ultimo 'p_int3'", lista_insertar_ultimo(lista, p_int3) == true);
    print_test("Lista ultimo es p_int3", lista_ver_ultimo(lista) == p_int3);
    print_test("Borro primero 'p_int1' ", lista_borrar_primero(lista) == p_int1);
    print_test("Borro primero 'p_int2' ", lista_borrar_primero(lista) == p_int2);
    print_test("Borro primero 'p_int3'", lista_borrar_primero(lista) == p_int3);
    print_test("Lista vacia", lista_esta_vacia(lista) == true);
    lista_iter_destruir(iter);
    print_test("Se destruyo el iterador", true);
    lista_destruir(lista, NULL);
    print_test("Se destruyo la lista", true);
}

static void pruebas_iterador_externo_borrar_medio()
{

    printf("\n INICIO PRUEBAS ITERADOR EXTERNO BORRAR ELEMENTO MEDIO \n");

    lista_t *lista = lista_crear();
    print_test("Se creo la lista", true);
    int int1 = 1;
    int *p_int1 = &int1;
    int int2 = 2;
    int *p_int2 = &int2;
    int int3 = 3;
    int *p_int3 = &int3;
    int int4 = 4;
    int *p_int4 = &int4;
    int int5 = 5;
    int *p_int5 = &int5;
    print_test("Inserto en el ultimo 'p_int1' ", lista_insertar_ultimo(lista, p_int1) == true);
    print_test("Inserto en el ultimo 'p_int2' ", lista_insertar_ultimo(lista, p_int2) == true);
    print_test("Inserto en el ultimo 'p_int3' ", lista_insertar_ultimo(lista, p_int3) == true);
    print_test("Inserto en el ultimo 'p_int4' ", lista_insertar_ultimo(lista, p_int4) == true);
    print_test("Inserto en el ultimo 'p_int5' ", lista_insertar_ultimo(lista, p_int5) == true);
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("Se creo el iterador", true);
    print_test("Iter actual es 'p_int1'", lista_iter_ver_actual(iter) == p_int1);
    print_test("Avanzo un lugar", lista_iter_avanzar(iter) == true);
    print_test("Iter actual es 'p_int2'", lista_iter_ver_actual(iter) == p_int2);
    print_test("Azanzo un  lugar", lista_iter_avanzar(iter) == true);
    print_test("Estoy en el elemento del medio ", lista_iter_ver_actual(iter) == p_int3);
    print_test("Borro elemento del medio", lista_iter_borrar(iter) == p_int3);
    print_test("Iter actual es 'p_int4'", lista_iter_ver_actual(iter) == p_int4);
    print_test("Borro primero 'p_int1'", lista_borrar_primero(lista) == p_int1);
    print_test("Borro primero 'p_int2", lista_borrar_primero(lista) == p_int2);
    print_test("Borro primero 'p_int4", lista_borrar_primero(lista) == p_int4);
    print_test("Borro primero 'p_int5", lista_borrar_primero(lista) == p_int5);
    print_test("Lista vacia", lista_esta_vacia(lista) == true);
    lista_iter_destruir(iter);
    print_test("Se destruyo el iterador", true);
    lista_destruir(lista, NULL);
    print_test("Se destruyo la lista", true);
}


static void pruebas_iterador_interno_sin_corte()
{

    printf("\n INICIO PRUEBAS ITERADOR INTERNO SIN CORTE \n");

    lista_t *lista = lista_crear();
    print_test("Se creo la lista", true);
    int int1 = 1;
    int *p_int1 = &int1;
    int int2 = 2;
    int *p_int2 = &int2;
    int int3 = 3;
    int *p_int3 = &int3;
    int int4 = 4;
    int *p_int4 = &int4;
    int int5 = 5;
    int *p_int5 = &int5;
    print_test("Inserto en el ultimo 'p_int1' ", lista_insertar_ultimo(lista, p_int1) == true);
    print_test("Inserto en el ultimo 'p_int2' ", lista_insertar_ultimo(lista, p_int2) == true);
    print_test("Inserto en el ultimo 'p_int3' ", lista_insertar_ultimo(lista, p_int3) == true);
    print_test("Inserto en el ultimo 'p_int4' ", lista_insertar_ultimo(lista, p_int4) == true);
    print_test("Inserto en el ultimo 'p_int5' ", lista_insertar_ultimo(lista, p_int5) == true);
    printf("Llamo a la funcion lista_iterar, y por parametro la funcion sumar_todos\n");
    int suma = 0;
    int suma_correcta = int1 + int2 + int3 + int4 + int5;
    lista_iterar(lista, sumar_todos, &suma);
    print_test("La suma total es correcta ", suma == suma_correcta);
    lista_destruir(lista, NULL);
    print_test("Se destruyo la lista", true);
}

static void pruebas_iterador_interno_con_corte()
{

    printf("\n INICIO PRUEBAS ITERADOR INTERNO CON CORTE \n");

    lista_t *lista = lista_crear();
    print_test("Se creo la lista", true);
    int int1 = 1;
    int *p_int1 = &int1;
    int int2 = 2;
    int *p_int2 = &int2;
    int int5 = 5;
    int *p_int5 = &int5;
    int int4 = 4;
    int *p_int4 = &int4;
    int int3 = 3;
    int *p_int3 = &int3;
    print_test("Inserto en el ultimo 'p_int1' ", lista_insertar_ultimo(lista, p_int1) == true);
    print_test("Inserto en el ultimo 'p_int2' ", lista_insertar_ultimo(lista, p_int2) == true);
    print_test("Inserto en el ultimo 'p_int3' ", lista_insertar_ultimo(lista, p_int5) == true);
    print_test("Inserto en el ultimo 'p_int4' ", lista_insertar_ultimo(lista, p_int4) == true);
    print_test("Inserto en el ultimo 'p_int5' ", lista_insertar_ultimo(lista, p_int3) == true);
    printf("Llamo a la funcion lista_iterar, y por parametro la funcion sumas_hasta_elem_cinco\n");
    int suma = 0;
    int resultado = int1 + int2;
    lista_iterar(lista, sumar_hasta_elem_cinco, &suma);
    print_test("La suma es correcta", resultado == suma);

    lista_iter_t* iter = lista_iter_crear(lista);
    print_test("El iterador fue creado", true);
    print_test("Avanzo posicion", lista_iter_avanzar(iter) == true);
    print_test("Avanzo posicion", lista_iter_avanzar(iter) == true);
    print_test("Acual es p_int5", lista_iter_ver_actual(iter) == p_int5);
    print_test("Borro p_int5", lista_iter_borrar(iter) == p_int5);
    int suma_sin_corte = 0;
    int resultado_sin_corte = int1 + int2 + int3 + int4;
    lista_iterar(lista, sumar_hasta_elem_cinco, &suma_sin_corte);
    print_test("La suma es correcta", resultado_sin_corte == suma_sin_corte);
    lista_iter_destruir(iter);
    print_test("El iterador fue borrado", true);
    lista_destruir(lista, NULL);
    print_test("Se destruyo la lista", true);

}

void pruebas_lista_estudiante(void)
{

    pruebas_condiciones_iniciales();
    pruebas_invariante_lista();
    pruebas_invariante_lista_NULL();
    pruebas_lista_vacia_destruccion_completa();
    pruebas_lista_vacia_destruccion_incompleta();
    pruebas_lista_destruccion_completa();
    pruebas_lista_destruccion_incompleta();
    pruebas_volumen_lista(PRUEBA_VOL_1);
    pruebas_volumen_lista(PRUEBA_VOL_2);
    pruebas_volumen_lista(PRUEBA_VOL_3);
    pruebas_volumen_lista_al_reves(PRUEBA_VOL_1);
    pruebas_volumen_lista_al_reves(PRUEBA_VOL_2);
    pruebas_volumen_lista_al_reves(PRUEBA_VOL_3);
    pruebas_iterador_externo_condiciones_iniciales();
    pruebas_iterador_externo_insertar_inicio();
    pruebas_iterador_externo_insertar_final();
    pruebas_iterador_externo_insertar_medio();
    pruebas_iterador_externo_borrar_inicio();
    pruebas_iterador_externo_borrar_ultimo();
    pruebas_iterador_externo_borrar_medio();
    pruebas_iterador_interno_sin_corte();
    pruebas_iterador_interno_con_corte();
}

#ifndef CORRECTOR // Para que no dé conflicto con el main() del corrector.

int main(void)
{
    pruebas_lista_estudiante();
    return failure_count() > 0; // Indica si falló alguna prueba.
}

#endif