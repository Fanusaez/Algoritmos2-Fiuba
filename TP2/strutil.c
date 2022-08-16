#include <string.h>
#include <stdio.h>
#include <stdlib.h>
char *substr(const char *str, size_t n){

    char* cadena_nueva=malloc(sizeof(char)*(n+1));
    int i=0;
    
    while(i<n && str[i]!='\0' && str[i]!='\n' ){
        cadena_nueva[i]=str[i];
        i+=1;
    }

    cadena_nueva[i]='\0';

    return cadena_nueva;
}


char **split(const char *str, char sep){
    size_t largo=strlen(str);
    size_t cantidad_sep=0;
    for (int i=0;i<largo;i++){
        if(sep==str[i]) cantidad_sep+=1;
    }
    char** arreglo_palabras=malloc(sizeof(char*)*(cantidad_sep+2));
    

    size_t contador=0;
    size_t indice=0;
    size_t posicion_primer_letra=0;    

    while(indice<=cantidad_sep){

        if((contador+posicion_primer_letra)<largo && str[posicion_primer_letra+contador]!=sep) contador+=1;
        else{
            if(contador>=largo) contador+=1;
            
            arreglo_palabras[indice]=substr(str+ posicion_primer_letra , contador);
            indice+=1;
            posicion_primer_letra=posicion_primer_letra+contador+1;
            contador=0;
            }
    }
    arreglo_palabras[indice]=NULL;
    return arreglo_palabras;

}


char *join(char **strv, char sep){
    size_t total=0;
    size_t i=0;
    
    while(strv[i]!=NULL){
        total+=strlen(strv[i]);
        i+=1;
    }
    char* cadena_nueva= malloc(sizeof(char)*(total+i+1));
    i=0;
    size_t posicion_cadena_nueva=0;



    while (strv[i]!=NULL){

        size_t l=0;
        while(strv[i][l]!='\0'){

            cadena_nueva[posicion_cadena_nueva]=strv[i][l];
            posicion_cadena_nueva+=1;
            l+=1;
        }
        if (sep!='\0'){
            cadena_nueva[posicion_cadena_nueva]=sep;
            posicion_cadena_nueva+=1;
        }
        i+=1;
        
    }
    if (strv[0]==NULL) posicion_cadena_nueva+=1;
    if (sep=='\0') posicion_cadena_nueva+=1;

    cadena_nueva[posicion_cadena_nueva-1]='\0';
    return cadena_nueva;

}

void free_strv(char *strv[]){
    size_t i=0;
    while(strv[i]!=NULL){
        free(strv[i]);
        i+=1;
    }
    free(strv);
}

