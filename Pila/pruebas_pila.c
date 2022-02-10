#include "pila.h"
#include "testing.h"
#include <stdbool.h>
#include <stdio.h>
#define prueba_vol_1 100
#define prueba_vol_2 1000
#define prueba_vol_3 10000

static void pruebas_condiciones_iniciales_pila(void){

    pila_t* pila = pila_crear();

    printf("\nINICIO PRUEBAS CONDICIONES INICIALES\n");

    print_test("La pila esta vacia", pila_esta_vacia(pila) == true);
    print_test("Desapilo y obtengo NULL", pila_desapilar(pila) == NULL);
    print_test("Ver tope es NULL", pila_ver_tope(pila) == NULL);
    pila_destruir(pila);
}


static void prueba_pila_vacia(void) {
    int entero = 1;
    int* p_entero = &entero;

    printf("INICIO PRUEBAS DE PILA_VACIA\n");

    pila_t *pila = pila_crear();
    print_test("Apilo elemento", pila_apilar(pila, p_entero) == true);
    print_test("La pila no esta vacia", pila_esta_vacia(pila) == false);
    print_test("Desapilo elemento 'p_entero' ", pila_desapilar(pila) == p_entero);
    print_test("Pila vacia nuevamente", pila_esta_vacia(pila) == true);

    printf("\nPruebas con NULL en la pila\n");

    print_test("Apilo NULL", pila_apilar(pila, NULL) == true);  //veo que sea valido apilar NULL
    print_test("La pila no esta vacia", pila_esta_vacia(pila) == false);
    print_test("Apilo elemento", pila_apilar(pila, p_entero) == true);
    print_test("Verifico tope es p_entero", pila_ver_tope(pila) == p_entero);
    print_test("Desapilo elemento 'p_entero' ", pila_desapilar(pila) == p_entero);  
    print_test("Desapilo el valor NULL", pila_desapilar(pila) == NULL);
    print_test("La pila vuelve a estar vacia", pila_esta_vacia(pila) == true);
    pila_destruir(pila);
}
// inicio pruebas de apilamiento y desapilamiento

static void pruebas_apilamiento_desapilamiento(void){

    printf("INICIO PRUEBAS DE APILAMIENTO-DESAPILAMIENTO\n");

    pila_t* pila_1 = pila_crear();
    int i = 1;
    int j = 2;
    char caracter = 'w';

    print_test("La pila esta vacia", pila_esta_vacia(pila_1) == true);
    print_test("Apilo direccion a '1' ", pila_apilar(pila_1, &i) == true);
    print_test("La pila no esta vacia" , pila_esta_vacia(pila_1) == false);
    print_test("El tope ahora es '&i' ", pila_ver_tope(pila_1) == &i);
    print_test("Apilo direccion a '2' ", pila_apilar(pila_1, &j) == true);
    print_test("El tope ahora es '&j' ", pila_ver_tope(pila_1) == &j);
    print_test("Apilo direccion a 'w' ", pila_apilar(pila_1, &caracter) == true);

    printf("CHEQUEO LOS INVARIANTES DE LA PILA\n");

    char* p_caracter = pila_ver_tope(pila_1);
    print_test("El tope ahora es 'p_caracter' ", pila_ver_tope(pila_1) == p_caracter);
    print_test("La letra es 'w' ", *p_caracter == 'w'); 
    print_test("La direccion de caracter es correcta", p_caracter == &caracter);
    print_test("Desapilo el puntero p_character", pila_desapilar(pila_1) == p_caracter);
   
    int* p_j = pila_ver_tope(pila_1);
    print_test("El tope ahora es 'p_j' ", pila_ver_tope(pila_1) == p_j);
    print_test("El valor donde apunta el puntero es '2' ", *p_j == 2);
    print_test("La direccion de 'p_j' es correcta", p_j == &j);
    print_test("Desapilo el puntero p_j", pila_desapilar(pila_1) == p_j);

    int* p_i = pila_ver_tope(pila_1);
    print_test("El tope ahora es 'p_i' ", pila_ver_tope(pila_1) == p_i);
    print_test("El valor donde apunta el puntero es '1' ", *p_i == 1);
    print_test("La direccion de 'p_i' es correcta", p_i == &i);
    print_test("Desapilo el puntero p_i", pila_desapilar(pila_1) == p_i);

    print_test("La pila quedo vacia", pila_esta_vacia(pila_1) == true); // Verifico que haya quedado vacia.

    pila_destruir(pila_1); //Destruyo pila_1
}

void pruebas_de_volumen(int cantidad){

    printf("INICIO PRUEBAS DE VOLUMEN CON %i ELEMENTOS\n", cantidad);

    pila_t* pila_2 = pila_crear();
    int vector[cantidad]; //Le doy capacidad de sobra para futuras pruebas
    bool ok = true;
    for (int i = 0; i < cantidad; i++){ // creo vector con 100 elementos
        vector[i] = i; 
    }
    for (int i = 0; i < cantidad; i++){ // apilo los 100 punteros a los elementos del vector en mi pila
        ok &= pila_apilar(pila_2, &vector[i]);
        ok &= (pila_ver_tope(pila_2) == &vector[i]); // Verifico que el nuevo tope sea correcto
    }
    print_test("Los elementos se apilaron correctamente", ok);
    print_test("El Tope siempre fue el correcto" ,ok);

    for (int i = cantidad - 1; i >= 0; i--){ // Desapilo todos los elementos de la pila
        int* p_entero = pila_ver_tope(pila_2); //obtengo puntero que apunta al entero 'i'
        ok &= (pila_desapilar(pila_2) == &vector[i]);
        ok &= (*p_entero == i); // verifico que el puntero apunte correctamente a 'i'
    }
    print_test("El elemento desapilado 'i' coincidio con elemento  vector[i]", ok);
    print_test("El puntero  'p_entero' apunta correctamente a i", ok);

    print_test("La pila esta vacia", pila_esta_vacia(pila_2) == true); 
    print_test("Apilo primer valor vector", pila_apilar(pila_2, &vector[0])); // caso borde, que se comporte como una pila nueva
    print_test("Desapilo &vector[1]", pila_desapilar(pila_2) == &vector[0]);
    print_test("Pila vacia nuevamente", pila_esta_vacia(pila_2) == true);
    pila_destruir(pila_2);
} 



void pruebas_pila_estudiante() {
    pruebas_condiciones_iniciales_pila();
    prueba_pila_vacia();
    pruebas_apilamiento_desapilamiento();
    pruebas_de_volumen(prueba_vol_1);
    pruebas_de_volumen(prueba_vol_2);
    pruebas_de_volumen(prueba_vol_3);
}


#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
