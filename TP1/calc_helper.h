#ifndef CALC_HELPER
#define CALC_HELPER

#include "pila.h"

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// Todas estás funciones están implemnetadas en calc_helper.c, y se pueden usar
// tanto en dc.c como en infix.c.
//
// Están comentadas de tal manera que se entienda qué rol tienen en el diseño.
//
// Se declaran las funciones:
//
//   - calc_split — ignora espacios y saltos de línea
//   - calc_parse — decide si una palabra es número, operador, o no es válida
//
// Los tipos:
//
//   - token_type — una palabra podrá será bien de tipo TOK_NUM, bien TOK_OPER
//   - calc_num   - un typedef a "long int", para representar valores numéricos
//   - calc_operador - un struct para representar un operador y sus atributos
//
// Y funcionalidad adicional para pila_t:
//
//   - pilanum_crear – crea una pila para apilar enteros de la calculadora
//   - apilar_num    — apila un número, abstrayendo el uso de memoria dinámica
//   - desapilar_num — desapila un número, indicando si se pudo o no
//   - pilanum_destruir – destruye la pila creada anteriormente

/** EXPLICACIÓN */

/* Lo primero que necesitamos es una función que permita separar una línea en
 * palabras, como hace el propio split() de strutil. Sin embargo, si hiciéramos:
 *
 *   split("1  2 +\n")
 *
 * eso devolvería ["1", "", "2", "+\n"], mientras que sería más conveniente si
 * se ignoraran espacios extra, y saltos de línea, devolviendo ["1", "2", "+"].
 * Las funciones dc_split() y infix_split() aquí declaradas hacen eso.
 *
 * NOTA 1: ambas funciones devuelven un arreglo a liberar con free_strv().
 *
 * NOTA 2: si la línea solo contenía espacios, se devuelve el strv vacío {NULL}.
 */

char **dc_split(const char *linea);
char **infix_split(const char *linea);

/*
 * Una vez hemos dividido la línea en palabras, lo siguiente que necesitamos es
 * una función que pueda reconocer en estas palabras un token válido (que será
 * bien un valor numérico, bien un símbolo: operador o paréntesis). Ejemplos:
 *
 *    reconocible("17")  -> true, NUM, 17
 *    reconocible("log") -> true, OPER, LOG
 *    reconocible("1z7") -> false
 *    reconocible("-40") -> true, NUM, -40
 *    reconocible("+")   -> true, OPER, SUMA
 *    reconocible("(")   -> true, LPAREN
 *    reconocible(")")   -> true, RPAREN
 *    reconocible("]")   -> false
 *
 * Para ello, primero necesitamos poder representar el tipo reconocido, esto
 * es, si es un número o un operador; y, caso de ser operador, cuál:
 */

enum token_type
{
    TOK_NUM,    // Número
    TOK_OPER,   // Operador
    TOK_LPAREN, // Paréntesis de apertura
    TOK_RPAREN, // Paréntesis de cierre
};

enum oper_type
{
    OP_ADD,  // suma, +
    OP_SUB,  // resta, -
    OP_MUL,  // multiplicación, *
    OP_DIV,  // división entera, /
    OP_POW,  // potencia, ^
    OP_LOG,  // logaritmo, "log"
    OP_RAIZ, // raíz cuadrada, "sqrt"
    OP_TERN, // operador ternario, ?:
};

/* El tipo del valor numérico será "long int", que codificamos con un typedef.
 * También usamos un struct para definir los operadores y sus atributos.
 *
 */

typedef int64_t calc_num; // Para tokens TOK_NUM

typedef struct calc_oper
{ // Para tokens TOK_OPER
    enum oper_type op;
    // AYUDA: la implementación de dc puede simplificarse en ciertos aspectos si
    // el struct que representa un operador incluye el número de operandos que
    // precisa.Se recomienda modificar calc_helper.c para que proporcione el
    // número de operandos del operador , por ejemplo en un miembro "arity", o
    // "num_operandos":
    /**
     * int num_operandos;
     */
    // Asimismo, la implementación de infix puede generalizarse bastante si el
    // mismo struct del operador incluye también su precedencia y asociatividad,
    // para comparar los atributos del token actual con los del tope de la pila:
    /**
     * unsigned precedencia;
     * enum assoc asociatividad;  // p.ej. ASSOC_LEFT, ASSOC_RIGHT (a definir).
     */
    calc_num priority;
    calc_num arity;
} calc_operador;

/*
 * Finalmente, combinamos ambos en la representación del tipo + valor (de esta
 * manera siempre sabemos qué tenemos entre manos). Como, de haber un valor,
 * siempre es de un tipo u otro (entero u operador, pero no ambos a la vez),
 * usamos un union anónimo, que "fusiona" ambos valores en el mismo espacio.
 *
 * Ejemplo de uso (junto con la función calc_parse que sigue a continuación):
 *
 *     struct calc_token tok;
 *
 *     if (!calc_parse(palabra, &tok)) {
 *         fprintf(stderr, "La palabra no es un token válido\n");
 *     }
 *     else if (tok.type == TOK_NUM) {
 *         printf("El valor es: %ld\n", tok.valor);  // TOK_NUM => tok.valor
 *     }
 *     else if (tok.type == TOK_OPER) {
 *         printf("El token es un operador (valor del enum: %d)\n",
 *                tok.oper.op);  // TOK_OPER => tok.oper.op, tok.oper.arity, …
 *     }
 *     else if (tok.type == TOK_LPAREN || tok.type == TOK_RPAREN) {
 *         printf("Es un paréntesis de apertura o cierre");
 *     }
 */

typedef struct calc_token
{
    enum token_type type;
    char operador;
    union
    {
        calc_num value;     // Si "type" es TOK_NUM
        calc_operador oper; // Si "type" es TOK_OPER
    };
} calc_token_t; // modificacion

/*
 * La función calc_parse() recibe una token, y devuelve verdadero si es válido,
 * y falso en caso contrario. Támbien devuelve, vía el parámetro de salida
 * "parsed_token", el valor del token en sí. (En general, siempre conviene
 * tener la verificación y la conversión juntas, para que nunca diverjan.)
 */

bool calc_parse(const char *tok_str, struct calc_token *parsed_token);

/*
 * La pila_t de que disponemos es una pila de punteros, pero es mucho más
 * cómodo si podemos trabajar con valores numéricos solamente. Si dc solamente
 * apila números, y nunca operandos, podemos crear un wrapper sobre pila_t que
 * almacene solo enteros.
 *
 * Nótese que pilanum_t *no* necesita acceso a la estructura interna de la pila,
 * pues solo hace uso de sus primitivas públicas.
 */

typedef pila_t pilanum_t;

pilanum_t *pilanum_crear(void);
void pilanum_destruir(pilanum_t *pila);

/*
 * Primitiva de pilanum_t para apilar un número (usa internamente memoria dinámica,
 * liberada por supuesto en pilanum_destruir).
 */
void apilar_num(pilanum_t *pila, calc_num num);

/*
 * Y ahora, la función que permite desapilarlo, liberando la memoria reservada.
 * Como es más cómodo desapilar sin tener que comprobar cada vez si la pila
 * está vacía, hacemos que desapilar_num() devuelva el número por parámetro, y
 * el valor de retorno indica si se pudo desapilar (no estaba vacía la pila).
 */

bool desapilar_num(pilanum_t *pila, calc_num *num);

/* *****************************************************************
 *                    MIS FUNCIONES                                *
 * *****************************************************************/


// Recibe puntero un arreglo de punteros, lo recorre y devuelve la cantidad de elemetos en el arreglo.
size_t contador_elem(char **arr);

// Recibe una cantidad 'n' y crea un arreglo de 'n' elementos con espacio para almacenar
// la esrtuctura calc_token_t.
// Post: Devuelve un puntero a un arreglo de punteros que almacena la estructura 'calc_token_t'
calc_token_t **crear_arr_token(size_t n);

// Recibe un puntero a un arreglo de punteros del tipo calc_token_t,
// recorre el areglo y aplica free a cada puntero.
// Al finalizar la iteracion aplica free sobre el arreglo
// Post: Se libero toda la memoria pedida para el arreglo.
void liberar_tokens(calc_token_t **arr);


#endif
