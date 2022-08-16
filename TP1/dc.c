#define _POSIX_C_SOURCE 200809L
#include "calc_helper.h"
#include "strutil.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

calc_num operar_arreglo(calc_num arr_numeros[], calc_token_t* elemento, bool* condicion_corte);
void procesar_elementos(calc_token_t** arr);
calc_token_t** examinar_archivo_postfix(char* linea);
void imprimir_resultado(pila_t* pila);
calc_num calculo_binario(calc_num num1, calc_num num2, calc_token_t* elemento, bool* condicion_corte);

int main(int argc, char* argv[]){
    FILE* mi_archivo = stdin;
    if (!mi_archivo){
        printf("Error al abrir el archivo\n");
        return 1;
    }   
    char* linea = NULL;     
    size_t tam = 0;
    while(getline(&linea, &tam, mi_archivo) != EOF){
        calc_token_t** tokens = examinar_archivo_postfix(linea);
        if(!tokens) continue;
        procesar_elementos(tokens);
        liberar_tokens(tokens);
    }
    free(linea);
    fclose(mi_archivo);
    return 0;
}


calc_token_t** examinar_archivo_postfix(char* linea){

    char** elem_cadena = dc_split(linea); 
    size_t cant_elementos = contador_elem(elem_cadena); 
    calc_token_t** arr_token = crear_arr_token(cant_elementos);

    size_t pos = 0;
    while(elem_cadena[pos]) {
        bool valido = calc_parse(elem_cadena[pos], arr_token[pos]); // valido caracteres y asigno lo que corresponda a la estructura de cada posicion
       if(valido == false){
            printf("ERROR\n");
            free_strv(elem_cadena);
            liberar_tokens(arr_token);
            return NULL;
        }
        pos++;
    } 
    free_strv(elem_cadena);
    return arr_token;
}

void procesar_elementos(calc_token_t** arr){

    pilanum_t* pila =  pilanum_crear();
    size_t pos = 0; 
    bool cond_corte = true;

    while(arr[pos] && cond_corte){
       
        calc_token_t* elemento = arr[pos++];
        if(pila_esta_vacia(pila) && elemento -> type != TOK_NUM){
            cond_corte = false;
        }

        else if (elemento -> type == TOK_NUM) apilar_num(pila, elemento -> value);

        else if (elemento -> type == TOK_OPER){
            size_t pos_arr = 0;
            size_t arity = elemento -> oper.arity;
            calc_num* arr_numeros = calloc(arity, sizeof(calc_num));
            for(int i = 0; i < arity && cond_corte; i++){
                if(pila_esta_vacia(pila)) cond_corte = false;
                calc_num numero = 0; 
                desapilar_num(pila, &numero);
                arr_numeros[pos_arr++] = numero;
            }
            calc_num resultado = (calc_num)operar_arreglo(arr_numeros, elemento, &cond_corte); 
            apilar_num(pila, resultado);
            free(arr_numeros);
            
        }   
    }

    if(cond_corte) imprimir_resultado(pila);
    else printf("ERROR\n");
    pilanum_destruir(pila); 
    
}

// Opera aritmeticamente sobre los numeros del arreglo dado el operador 'elemento' y devuelve el resultado de
// la operacion. Cambia la condicion de corte a false y devuelve 1 si algo salio mal.
calc_num operar_arreglo(calc_num arr_numeros[], calc_token_t* elemento, bool* condicion_corte){

    if(elemento -> oper.op == OP_TERN){
        if(arr_numeros[2] == 0) return arr_numeros[0];
        return arr_numeros[1];
    }

    if(elemento -> oper.op == OP_RAIZ){

        if(arr_numeros[0] < 0){
            *condicion_corte = false;
            return 1;
        }
        double resultado = sqrt((double)arr_numeros[0]);
        return (calc_num)resultado;
    } 

    else return calculo_binario(arr_numeros[0], arr_numeros[1], elemento, condicion_corte);

}

void imprimir_resultado(pila_t* pila){

    calc_num resultado;
    desapilar_num(pila, &resultado);
    if(pila_esta_vacia(pila)) printf("%li\n", resultado);
    else  printf("ERROR\n"); // Hay otro elemento en la pila, y no hay mas operandos, error.
}


calc_num calculo_binario(calc_num num1, calc_num num2, calc_token_t* elemento, bool* condicion_corte){

    if(elemento -> oper.op == OP_ADD) return (num2 + num1);

    if(elemento -> oper.op == OP_SUB) return (num2 - num1);

    if(elemento -> oper.op == OP_MUL) return (num2 * num1);

    if(elemento -> oper.op == OP_LOG){
        if(num1 < 2 || log((double)num1) == 0){
            *condicion_corte = false;
            return 1;
        } 
        double resultado = log((double)num2) / log((double)num1);
        return (calc_num)resultado; 
    }
    if(elemento -> oper.op == OP_DIV){
        if(num1 == 0){
            *condicion_corte = false;
            return 1;
        }
        return (num2 / num1);
    }

    if(elemento -> oper.op == OP_POW){
        if(num1 < 0){
            *condicion_corte = false;
            return 1;
        }
        double resultado = pow((double)num2, (double)num1);
        return (calc_num)resultado;
    }
    *condicion_corte = false;
    return 1;
}