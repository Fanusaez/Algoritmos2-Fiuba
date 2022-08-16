from collections import deque
from grafo import Grafo
import random
COEF_AMORT = 0.75
CANT_PAGE_RANK = 20
LABEL_ITER = 10


def camino_minimo_bfs(grafo, origen, destino):
    """ 
    Recibe grafo, el origen y destino, devuelve el camino minimo partiendo desde el origen hasta el destino 
    indicado, el recorrido es BFS. Si no lo encuentra devuelve un camino vacio 
    """
    visitados = set()
    padres = {}
    q = deque()
    padres[origen] = None
    q.append(origen)
    visitados.add(origen)

    while not len(q)==0:
        v = q.popleft()
        for w in grafo.adyacentes(v):
            if w == destino:
                padres[w] = v
                ruta = crear_ruta(origen, destino, padres)    
                return ruta, padres
            
            if w not in visitados:
                visitados.add(w)
                padres[w] = v
                q.append(w)
    return [], {}

def page_rank(grafo):
    """ Funciona en conjunto con _page_rank y devuelve la importancia de cada vertice en un diccionario """
    pr_vertices = {}
    vertices = grafo.obtener_vertices()
    cant_vert = len(vertices)
    for v in vertices: pr_vertices[v] = 1
    for _ in range(CANT_PAGE_RANK):
        pr_vertices = _page_rank(grafo, pr_vertices)
    return pr_vertices


def label_propagation(grafo):
    """
    Algoritmo para detectar comunidades dentro de un grafo, devuelve un diccionario con el vertice de llave 
    y un entero de dato que representa el numero de comunidad
    """
    vertices = grafo.obtener_vertices()
    random.shuffle(vertices, random.random)
    contador = 0
    label = {}
    for v in vertices: 
        label[v] = contador
        contador += 1
    for _ in range(LABEL_ITER): 
        for v in label.keys():
            label[v] = max_freq(grafo, v, label)
    return label
    
def bfs_a_distancia_n(grafo, origen, n):
    """
    Recibe grafo, un origen y un nuermo 'n',
    devuelve todos los vertices que se encuentren a una distancia menor o igual a 'n' desde el origen 
    """
    visitados = set()
    orden = {}
    orden[origen] = 0
    visitados.add(origen)
    q = deque()
    q.append(origen)
    while len(q) != 0:
        v = q.popleft()
        for w in grafo.adyacentes(v):
            if w not in visitados:
                orden[w] = orden[v] + 1
                if orden[w] > n:
                    continue
                visitados.add(w)
                q.append(w)
    return visitados


def buscar_ciclo(grafo, v, n):
    """ Recibe un vertice v y un nuermo 'n', busca un ciclo que empiece y termine en el vertice 'v' de largo 'n' """
    if n == 1 and grafo.estan_unidos(v,v):
        return [v,v]
    dic_padre = {}
    dic_padre[v] = None
    ciclo = _buscar_ciclo(grafo, v, v, n, dic_padre)
    if ciclo: ciclo.append(v)
    return ciclo

def tarjan_cfc(grafo):
    """ Busca los conjutos de vertices que se conectan todos con todos y los devuelve """
    visitados = set()
    pila, conjuntos = [], []
    mb, orden = {}, {}
    for v in grafo.obtener_vertices():
        if v not in visitados:
            orden[v] = mb[v] = 0
            _tarjan_cfc(grafo, v, visitados, pila, mb, orden, conjuntos)
    return conjuntos


##########################
#                        #
#    FUNCIONES AUX       #
#                        #
##########################


def max_freq(grafo, v, label):
    """ Calcula el label mas grande de cada vertice entrante a 'v' y lo devuelve """
    v_entrantes = grafo.obtener_vertices_entrada(v)
    max = 0
    for w in v_entrantes:
        if label[w] > max: max = label[w]
    return max


def crear_ruta(origen, destino, padres):
    """ Recrea la ruta desde el origen al destino, devuelve una lista """
    lista = []
    lista.append(destino)
    actual = padres[destino]
    while actual != origen:
        lista.append(actual)
        actual = padres[actual]
    lista.append(origen)
    return lista[::-1]

def _page_rank(grafo, pr):
    """ Calcula en PR de cada vertice y lo devuelve """
    cant_vert = len(grafo.obtener_vertices())
    for v, _ in pr.items():
        pr[v] = (1 - COEF_AMORT)/cant_vert + COEF_AMORT * pr_vert_entrada(grafo, v, pr)
    return pr


def pr_vert_entrada(grafo, v, pr):
    """ Aplica la sefunda parte de la formula page_rank a los vertices entrantes al vertice v y devuelve el valor """
    vert_entrada = grafo.obtener_vertices_entrada(v)
    pr_vert_entrada = 0
    for w in vert_entrada:
        pr_vert_entrada += pr[w] / len(grafo.adyacentes(w))
    return pr_vert_entrada


def _tarjan_cfc(grafo, v, visitados, pila, mb, orden, conjuntos):

    if v not in visitados: pila.append(v)
    visitados.add(v)
    for w in grafo.adyacentes(v):
        if w not in visitados:
            orden[w] = orden[v] + 1
            mb[w] = orden[v] + 1
            _tarjan_cfc(grafo, w, visitados, pila, mb, orden, conjuntos)
            mb[v] = min(mb[v], mb[w])
        elif w in pila:
            mb[v] = min(mb[v], orden[w])
    
    if orden[v] == mb[v] and len(pila) > 0:
        nueva_cfc = []
        actual = pila.pop()
        while actual != v and len(pila) > 0:
            nueva_cfc.append(actual)
            actual = pila.pop()
        nueva_cfc.append(v)
        conjuntos.append(nueva_cfc)


def _buscar_ciclo(grafo,origen, v, numero, dic_padre):
    
    if len(dic_padre) == numero:
        return comprobar_ciclo(grafo, origen, v, dic_padre)
    
    for w in grafo.adyacentes(v):
        if w not in dic_padre:
            dic_padre[w] = v
            vis = _buscar_ciclo(grafo, origen, w, numero, dic_padre)
            if vis: return vis  
            if w in dic_padre:
                dic_padre.pop(w)
    
    return False


def comprobar_ciclo(grafo, origen, v, padre):
    """ Comprueba si se cumplio un ciclo y develve un booleano en base a ello """
    if origen in grafo.adyacentes(v): return crear_ruta(origen, v, padre)
    return False
    
