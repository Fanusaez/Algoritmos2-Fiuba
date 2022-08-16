#ifndef TESTING_H
#define TESTING_H

#include <stdbool.h>

// La siguiente macro permite usar sentencias de debug (printf), pero evitando
// que se impriman en el corrector automático (ver abajo para excepciones). La
// macro se llama debug_print(), y se usa como printf(). Como ejemplo, si en el
// archivo abb.c, dentro de una función llamada _abb_borrar(), se escribiese:
//
//     debug_printf("3er caso, num_hijos=%d\n", num_hijos);
//
// entonces se imprimiría:
//
//     abb.c:147:_abb_borrar(): 3er caso, num_hijos=2
//
// Nota: si alguna vez puntual se necesita forzar que se impriman los mensajes
// en el corrector, se puede usar #define DEBUG 1 antes de #include "testing.h".
// En cualquier caso, las llamadas a debug_print() siempre deberían borrarse una
// vez el TP pasa las pruebas, y nunca incluirse en la entrega final.

#define debug_print(fmt, ...)                                           \
    do { if (DEBUG_PRINT) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                  __LINE__, __func__, __VA_ARGS__);     \
    } while (0)


// Imprime el mensaje seguido de OK o ERROR y el número de línea. Contabiliza el
// número total de errores en una variable interna. Ejemplo:
//
//    pila_t *p = pila_crear();
//    print_test("La pila fue creada", p != NULL);
//    print_test("La pila está vacía", pila_esta_vacia(p));
#define print_test(mensaje, result) do { \
    real_print_test(mensaje, result, __FILE__, __LINE__, #result); \
  } while (0)

// Función auxiliar para print_test(). No debería ser usada directamente.
void real_print_test(const char* mensaje, bool ok,
		     const char* file, int line, const char* failed_expr);

// Devuelve el número total de errores registrados por print_test().
int failure_count(void);

// Valor de DEBUG_PRINT para debug_print().
#if defined(DEBUG) || !defined(CORRECTOR)
#define DEBUG_PRINT 1
#else
#define DEBUG_PRINT 0
#endif

#endif // TESTING_H
