#define _POSIX_C_SOURCE 200809L
#include "calc_helper.h"
#include "strutil.h"
#include "pila.h"
#include "cola.h"
#include <stdlib.h>
#include <stdio.h>


calc_token_t** examinar_archivo_infix(char* linea);
cola_t* procesar_elementos_infix(calc_token_t** arr);
void imprimir_cola(cola_t* cola);


int main(int argc, char* argv[]){
    FILE* mi_archivo = stdin;
    if (!mi_archivo){
        printf("Error al abrir el archivo\n");
        return 1;
    }   
    char* linea = NULL;     
    size_t tam = 0;
    while(getline(&linea, &tam, mi_archivo) != EOF){
        calc_token_t** tokens = examinar_archivo_infix(linea);
        if(!tokens) continue; // Si devolvio NULL examinar_archivo, paso a la sig linea
        cola_t* cola = procesar_elementos_infix(tokens);
        imprimir_cola(cola);
        liberar_tokens(tokens);
    }
    free(linea);
    fclose(mi_archivo);
    return 0;
}



// Recibe una linea de caracteres, y devuelve un arreglo de tokens.
calc_token_t** examinar_archivo_infix(char* linea){

    char** elem_cadena = infix_split(linea);  // aplico split para separar los elementos
    size_t cant_elementos = contador_elem(elem_cadena); // Cuento cant elementos
    calc_token_t** arr_token = crear_arr_token(cant_elementos);// creo arreglos de tokens (pido memoria)

    bool cond_corte = true;
    size_t pos = 0;
    while(elem_cadena[pos] && cond_corte) {
        bool parsed = calc_parse(elem_cadena[pos], arr_token[pos]); // valido caracteres y asigno lo que corresponda a la estructura de cada posicion
        pos++;
        if(!parsed){ // si no reconocio algun signo o valor NO SE SI ES NECESARIO
            printf("%s", "ERROR\n"); 
            cond_corte = false;
        }    
    }
    free_strv(elem_cadena); // pidio memoria en dc_split
    if(cond_corte) return arr_token;
    return NULL;
}


// Recibe un arreglo de tokens, y aplica los algoritmos necesarios para convertir de notacion infija a postfija;
// Pre: El arreglo de tokens no contiene elementos ivalidos
// Post: Se devuelve una pila con la conversion invertida
cola_t* procesar_elementos_infix(calc_token_t** arr){

 
    pila_t* pila_op = pila_crear(); // contiene operadores
    size_t pos = 0; 
    cola_t* cola = cola_crear();
    while(arr[pos]){
       
        calc_token_t* elemento = arr[pos++];
        calc_token_t* tope;
        if (elemento -> type == TOK_NUM) cola_encolar(cola, elemento);
        else if (pila_esta_vacia(pila_op) || elemento -> type == TOK_LPAREN) pila_apilar(pila_op, elemento); //es un operador, si no hubiera entrado arriba
        else if((tope = pila_ver_tope(pila_op)) -> oper.priority < elemento -> oper.priority || elemento -> oper.priority == 3) pila_apilar(pila_op, elemento);
        else if(elemento -> type == TOK_RPAREN){
            while(!pila_esta_vacia(pila_op) && (tope = pila_ver_tope(pila_op)) -> type != TOK_LPAREN) cola_encolar(cola, pila_desapilar(pila_op));
            if((tope = pila_ver_tope(pila_op)) -> type == TOK_LPAREN) pila_desapilar(pila_op); // deberia desapilar nomas
        }
        else{
            while(!pila_esta_vacia(pila_op) && (tope = pila_ver_tope(pila_op)) -> oper.priority >= elemento -> oper.priority){
                cola_encolar(cola, pila_desapilar(pila_op));
            }
            pila_apilar(pila_op, elemento);
        }
    }
    while(!pila_esta_vacia(pila_op)){
        calc_token_t* elemento_pila = pila_desapilar(pila_op);
        if(elemento_pila -> type != TOK_LPAREN && elemento_pila -> type != TOK_RPAREN) cola_encolar(cola, elemento_pila);
    }  
    pila_destruir(pila_op);
    return cola;
}
    

    
// Recibe una cola de tokens, imprime los valores y operadores que correspondan a cada token
// Pre: La cola no esta vacia
// Post: La cola quedo vacia
void imprimir_cola(cola_t* cola){

    while(!cola_esta_vacia(cola)){
        calc_token_t* elemento = cola_desencolar(cola);
        if(elemento -> type == TOK_NUM) printf("%li ", elemento -> value);
        else printf("%c ", (char)elemento -> operador);
    }
    printf("\n");
    cola_destruir(cola, NULL);
    
}    




