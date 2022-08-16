#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "strutil.h"
#include "hash.h"
#define MAX_CADENA 15
#define ESP_COMA 1
#define EXTRA_COUNT_SORT 1
#define RANGO_ASCII 256

//Declaracion funciones aux
char** crear_arreglo(hash_t* hash);
void analizar_archivo(FILE* mi_archivo);
char** couting_sort(char** arr, size_t maximo, size_t cant_usuarios, size_t indice, bool llamado_por_radix, size_t indice_radix);
size_t contar_cifras(size_t num);
void rellenar_barra_cero(char* nuevo_arreglo, size_t inicio, size_t largo);
char** radix_sort(char** arreglo,size_t rango,size_t cant_usuarios);
void imprimir_y_liberar_resultado(char** arr, size_t largo);
void procesar_usuarios(hash_t* hash, size_t max_hashtag);

int main(int argc, const char* argv[]){
    FILE* mi_archivo = fopen(argv[1], "r");
    if (!mi_archivo){
        printf("Error al abrir el archivo\n");
        return 1;
    }   
    analizar_archivo(mi_archivo);
    fclose(mi_archivo);
    return 0;
}

void analizar_archivo(FILE* mi_archivo){
    char* linea = NULL;     
    size_t tam = 0;
    hash_t* hash = hash_crear((hash_destruir_dato_t)hash_destruir);
    if(!hash) return;
    size_t max_hashtag = 0;
    while(getline(&linea, &tam, mi_archivo) != EOF){
        char** usuario_y_tags = split((const char*)linea, ',');
        size_t indice = 1;
        char* usuario = usuario_y_tags[0];
        while(usuario_y_tags[indice]){
            if(!hash_pertenece(hash,(const char*)usuario)){
                hash_t* diccionario_aux = hash_crear(NULL);
                if(!diccionario_aux) hash_destruir(hash);
                hash_guardar(hash, (const char*)usuario, (void*)diccionario_aux);                
            }
            hash_guardar(hash_obtener(hash, (const char*)usuario), usuario_y_tags[indice], NULL);

            if(max_hashtag < hash_cantidad(hash_obtener(hash, (const char*)usuario))){
                max_hashtag = hash_cantidad(hash_obtener(hash, (const char*)usuario));
            }
            indice++;
        }
        free_strv(usuario_y_tags);
    }
    procesar_usuarios(hash, max_hashtag);
    free(linea);
}

void procesar_usuarios(hash_t* hash, size_t max_hashtag){

    char** arr = crear_arreglo(hash);
    if(!arr) return;
    size_t cant_usuarios = hash_cantidad(hash);
    char** ordenar_por_letras = radix_sort(arr, RANGO_ASCII, cant_usuarios);
    char** resultado = couting_sort(ordenar_por_letras, max_hashtag, cant_usuarios, 0, false, 0);
    imprimir_y_liberar_resultado(resultado, cant_usuarios);
    hash_destruir(hash);
}

char** crear_arreglo(hash_t* hash){

    size_t cant_usuarios = hash_cantidad(hash);
    char** arreglo = malloc(cant_usuarios * sizeof(char*));
    if(!arreglo) return NULL;
    size_t pos_arr = 0;
    hash_iter_t* iter = hash_iter_crear(hash);
    while(!hash_iter_al_final(iter)){

        const char* usuario = hash_iter_ver_actual(iter);
        size_t cant_tags = (size_t)hash_cantidad(hash_obtener(hash, usuario));
        size_t cifras_cant = contar_cifras(cant_tags);
        size_t largo_arreglo =  cifras_cant + ESP_COMA + MAX_CADENA;
        char* nuevo_arreglo = calloc(largo_arreglo, sizeof(char*));
        if(!nuevo_arreglo) return NULL;
        sprintf(nuevo_arreglo,"%ld,%s", cant_tags, usuario);
        rellenar_barra_cero(nuevo_arreglo, (cifras_cant + ESP_COMA + strlen(usuario)), largo_arreglo);
        arreglo[pos_arr++] = nuevo_arreglo;
        hash_iter_avanzar(iter);
    }
    hash_iter_destruir(iter);
    return arreglo;
}

char** couting_sort(char** arr, size_t rango, size_t cant_usuarios, size_t indice_split, bool llamado_por_radix, size_t indice_radix){
    
    size_t* arreglo_contador = calloc(rango + EXTRA_COUNT_SORT, sizeof(size_t));
    if(!arreglo_contador) return NULL;
    for(int i = 0; i < cant_usuarios; i++){

        char** arr_us_cant = split((const char*)arr[i], ',');
        char* user_o_tags = arr_us_cant[indice_split];
        size_t pos_arr_contador = 0;
        
        if(!llamado_por_radix) pos_arr_contador = (size_t)atoi(user_o_tags);
        else{
            if(indice_radix > strlen(user_o_tags)) pos_arr_contador = 0;
            else pos_arr_contador = (size_t)user_o_tags[indice_radix]; // numero tabla ascii
        }
        arreglo_contador[pos_arr_contador] += 1;
        free_strv(arr_us_cant); // memoria pedida por split
    }

    size_t* suma_acumulativa = calloc(rango + EXTRA_COUNT_SORT, sizeof(size_t));
    if(!suma_acumulativa) return NULL;

    size_t acumulacion = 0;
    for(size_t i = 0; i < rango; i++){
        acumulacion += arreglo_contador[i];
        suma_acumulativa[i + 1] = acumulacion;
    }

    char** resultado = calloc(cant_usuarios, sizeof(char*));
    if(!resultado) return NULL;

    for(size_t i = 0; i < cant_usuarios ;i++){
        char** arr_us_cant = split((const char*)arr[i],',');
        char* user_o_tags = arr_us_cant[indice_split];
        size_t pos_arr_contador = 0;
        if(!llamado_por_radix) pos_arr_contador = (size_t)atoi(user_o_tags);
        else{
            if(indice_radix > strlen(user_o_tags)) pos_arr_contador = 0;
            else pos_arr_contador = (size_t)user_o_tags[indice_radix]; 
        }
        resultado[suma_acumulativa[pos_arr_contador]] = arr[i];
        suma_acumulativa[pos_arr_contador] += 1;
        free_strv(arr_us_cant);
    }

    free(arreglo_contador);
    free(suma_acumulativa);
    free(arr);
    return resultado;
}


char** radix_sort(char** arreglo, size_t rango, size_t cant_usuarios){
    for (int i = MAX_CADENA - 1; i >= 0; i--) arreglo = couting_sort(arreglo, rango, cant_usuarios, 1, true, i);     
    return arreglo;
}


size_t contar_cifras(size_t num){
    size_t contador = 1;
    while(num/10>0)
    {
        num=num/10;
        contador++;
    }
    return contador;
}

void rellenar_barra_cero(char* nuevo_arreglo, size_t inicio, size_t largo){
    for(size_t i = inicio; i < largo; i++) nuevo_arreglo[i] = '\0';
}

void imprimir_y_liberar_resultado(char** arr, size_t largo){

    char** usuarios_tags_aux = split((const char*)arr[0], ',');
    size_t cant_tags_aux = atoi(usuarios_tags_aux[0]);
    bool coma = false;
    for(int i = 0; i < largo; i++){
        char** usuarios_tags = split((const char*)arr[i],',');
        size_t cant_tags = atoi(usuarios_tags[0]);
        if(cant_tags != cant_tags_aux || i == 0){
            if(i!=0) printf("\n");
            cant_tags_aux = cant_tags;
            printf("%ld: ",cant_tags_aux);
            coma = false;
        }
        if(coma == true) printf(", ");
        coma = true;
        printf("%s", usuarios_tags[1]);
        free(arr[i]);
        free_strv(usuarios_tags);
    }
    free_strv(usuarios_tags_aux);
    printf("\n");
    free(arr);
}

