#define _POSIX_C_SOURCE 200809L  // strdup() stpcpy()
#define CANT_INICIAL 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *substr(const char *str, size_t n){
    if (n > strlen(str)) n = strlen(str);
    char* cadena = strndup(str, n + 1); 
    if (!cadena) return NULL;
    cadena[n] = '\0';
    return cadena;
}

char **split(const char *str, char sep){

    size_t pos_aux = 0;
    size_t pos_vector = 0;
    size_t cant_separadores = 0;
    
    for (int i = 0; i < strlen(str); i++) if (str[i] == sep) cant_separadores++; // Recorro la lista y cuento separadores
    
    char aux[strlen(str) + 1]; 
    char** vector = calloc((cant_separadores + CANT_INICIAL), sizeof(char*));
    if(!vector) return NULL;

    for (int i = 0; i <= strlen(str); i++){
        if(str[i] == sep || i == strlen(str)){
            aux[pos_aux] = '\0'; // Para que no lea caracteres posteriores si es que los hay
            char* duplicado = strdup(aux); // Pide memoria para un char* y me hago cargo del puntero
            vector[pos_vector++] = duplicado;
            pos_aux = 0;
        }
        else aux[pos_aux++] = str[i];
    }
    return vector;
}


char *join(char **strv, char sep){

    size_t long_cadena = 0;
    size_t pos_vector = 0;
    size_t pos_cadena_aux = 0;
    size_t  pos_cadena = 0;
     
    while(strv[pos_vector]) long_cadena += strlen(strv[pos_vector++]);

    pos_vector = 0;

    char* cadena  = calloc(long_cadena + 1, sizeof(char*)); 
    if (!cadena) return NULL;
    
    while(strv[pos_vector]){
        char* cadena_aux = strv[pos_vector++];
        size_t long_cadena_aux = strlen(cadena_aux);
        for(int i = 0; i < long_cadena_aux; i++) cadena[pos_cadena++] = cadena_aux[pos_cadena_aux++];
        pos_cadena_aux = 0;
        if (strv[pos_vector] && sep != '\0') cadena[pos_cadena++] = sep;
    }
    cadena[pos_cadena] = '\0'; 
    return cadena;
}

void free_strv(char *strv[]){
    size_t pos_arreglo = 0;
    while(strv[pos_arreglo]) free(strv[pos_arreglo++]);
    free(strv);
}

