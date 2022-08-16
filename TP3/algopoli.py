#!/usr/bin/python3
from biblioteca import bfs_a_distancia_n, buscar_ciclo, camino_minimo_bfs, camino_minimo_bfs, label_propagation, page_rank, tarjan_cfc
from grafo import Grafo
import operator
import sys
sys.setrecursionlimit(5000)


def main():
    if len(sys.argv) == 2:
        analizar_parametros(sys.argv[1], sys.stdin)
    else: raise AssertionError("Insuficientes parametros")



def analizar_parametros(mensajes, comandos):
    """ Recibe los 'mensajes' para crar el grafo y una lista de comandos con la cual se llama a 'realizar_comandos' """
    grafo = crear_grafo_delincuentes(mensajes)
    realizar_comandos(grafo,comandos)

def realizar_comandos(grafo, comandos_archivo):
    """ Recibe el grafo y una lista de comandos, los procesa y llama a funcion que describe el 'comando'"""
    for linea in comandos_archivo:
        comando = linea.rstrip("\n").split(" ")
        if comando[0] == "min_seguimientos":
            min_seguimientos(grafo,comando[1],comando[2])
        elif comando[0] == "mas_imp":
            mas_imp(grafo,int(comando[1]))
        elif comando[0] == "persecucion":
            lista=comando[1].split(",")
            persecucion(grafo,lista,int(comando[2]))
        elif comando[0] == "comunidades":
            comunidades(grafo,int(comando[1]))
        elif comando[0] == "divulgar":
            divulgar(grafo,comando[1],int(comando[2]))
        elif comando[0] == "divulgar_ciclo":
            divulgar_ciclo(grafo,comando[1],int(comando[2]))
        elif comando[0] == "cfc":
            cfc(grafo)               

def crear_grafo_delincuentes(nombre_archivo):
    """ Crea a partir de un archivo un grafo de 'delincuentes' y lo devuelve """
    lista = leer_archivo(nombre_archivo)
    grafo = Grafo()
    for emisor,receptor in lista:
        grafo.agregar_vertice(emisor)
        grafo.agregar_vertice(receptor)
        grafo.agregar_arista(emisor,receptor,1)
    return grafo

def min_seguimientos(grafo, origen, destino):
    """ Imprime el camino minimo en cantidad de aristas desde el delincuente de origen hasta el delincuente de destino """

    if origen not in grafo.obtener_vertices() or destino not in grafo.obtener_vertices():
        print("Seguimiento imposible")
    else:
        camino_min, _ = camino_minimo_bfs(grafo,origen,destino)
        if len(camino_min) == 0:
            print("Seguimiento imposible")
        else:
            imprimir_camino(camino_min)


def mas_imp(grafo, k):
    """ Recibe un grafo y un numero 'k', imprime una lista de los 'k' delincuentes mas importantes de la 'red' """
    pr = page_rank(grafo)
    pr = sorted(pr.items(), key=operator.itemgetter(1), reverse=True)
    mas_imp = ""
    contador = 0
    for v, _ in pr:
        mas_imp += v + ", "
        contador += 1
        if contador == k: break
    print(mas_imp[0:-2])

    
def persecucion(grafo, agentes, k):
    """ Recibe un grafo, una lista de agentes y un numero 'k' ,imprime el camino mas corto entre algno agentes y los k delincuentes mas importantes """
    vertices_grafo = grafo.obtener_vertices()
    agentes_aux = tuple(agentes)
    for v in agentes_aux:
        if v not in vertices_grafo: agentes.remove(v)
    cam_total_min, delin_cercano = [], ""
    dic_pr_delincuentes = page_rank(grafo)
    pr_delincuentes = sorted(dic_pr_delincuentes.items(), key=operator.itemgetter(1), reverse=True)[0:k]
    for delin, pr_delin in pr_delincuentes:
        for agente in agentes:
            ruta_min, _ = camino_minimo_bfs(grafo,agente, delin)
            if len(ruta_min) == 0 or ruta_min[-1] in agentes: continue
            if len(ruta_min) < len(cam_total_min) or len(cam_total_min) == 0: cam_total_min, delin_cercano = ruta_min, pr_delin
            elif len(ruta_min) == cam_total_min and pr_delin > delin_cercano: 
                cam_total_min, delin_cercano = ruta_min, pr_delin
    imprimir_camino(cam_total_min)

def comunidades(grafo, n):
    """ Recibe un grafo y un numero 'n', detecta comunidades dentro de la 'red' de delincuentes """
    label = label_propagation(grafo)
    comun_label = procesar_label(label, n)
    comun_n_vert = []
    for comunidad, _ in comun_label.items():
        if len(comun_label[comunidad]) >= n: comun_n_vert.append(comunidad)
    if len(comun_n_vert) > 0: imprimir_comunidades(comun_label, comun_n_vert)

def divulgar(grafo, delincuente, n):
    """ 
    Recibe un grafo y un delincuente, imprime una lista con todos los delincuentes 
    a los cuales les termina llegando un rumor que comienza en el delincuente 
    """
    chismosos = bfs_a_distancia_n(grafo, delincuente, n)
    chismosos.remove(delincuente) # elimino el delincuente que empezo el chisme
    chismosos_str = ""
    for delin in chismosos: chismosos_str += delin + ", "
    print(chismosos_str[0:-2])


def divulgar_ciclo(grafo, delincuente, n):
    """ 
    Recibe un grafo un delincuente y un numero 'n', 
    busca un ciclo que empieze y termine en el delincuente y sea de largo 'n'.
    Si existe imprime el recorrido, de lo cotrario imprimira "No se encontro recorrido"
    """
    if len(grafo.obtener_vertices()) == 0: 
        print("No se encontro recorrido")
        return 
    ciclo = buscar_ciclo(grafo, delincuente, n)
    if not ciclo:  
        print("No se encontro recorrido")
        return
    ciclo_str = ""
    for delin in ciclo:
        ciclo_str += f"{delin} -> "
    print(ciclo_str[0:-4])

def cfc(grafo):
    """ Imprime cada conjunto de vértices entre los cuales todos están conectados con todos """
    componentes = tarjan_cfc(grafo)
    contador = 1
    for componente in componentes:
        str_comp = ""
        for v in componente:
            str_comp += v + ", "
        linea = f"CFC {contador}: {str_comp}"
        contador += 1
        print(linea[:-2])
        
##########################
#                        #
#    FUNCIONES AUX       #
#                        #
##########################

def leer_archivo(archivo):
    """ Separa los id de los delincuentes en una lista y los devuelve """
    lista = []
    with open (archivo) as delincuentes:
        for linea in delincuentes:
            id=linea.rstrip("\n").split("\t")
            lista.append((id[0],id[1]))
    return lista


def imprimir_camino(camino):
    """ Recibe una lista de vertices que representa un camino, imprime el camino pasado por parametro """
    contador = 0
    for v in camino: 
        if contador < len(camino) - 1: print( f"{v} -> ",end="")
        else: print(f"{v}",end="")
        contador += 1
    print()

def procesar_label(label, n):
    """ Lee el diccionario label y lo ordena por comunidades, devuelve un diccioanrio """
    label = sorted(label.items(), key=operator.itemgetter(1), reverse=True)
    comunidad = []
    label_comunidades = {}
    comun_actual = -1

    for v, comun in label:
        if comun_actual == -1: comun_actual = comun
        if comun_actual != comun:
            label_comunidades[comun_actual] = comunidad
            comunidad = []
            comun_actual = comun
        comunidad.append(v)
        if label[len(label)-1][1] == comun_actual:
            label_comunidades[comun_actual] = comunidad
    return label_comunidades



def imprimir_comunidades(dic_comunidades, comunidades_a_imprimir):
    """Imprime un grupo de comunidades que cumple los requisitos"""
    comunidad = ""
    for num,comun in enumerate(comunidades_a_imprimir):
        for v in dic_comunidades[comun]:
            comunidad += f"{v}, " 
        print(f"Comunidad {num+1}: {comunidad[:-2]}")
        comunidad = ""

main()
