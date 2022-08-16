import random

class Grafo:

    def __init__(self):
        """ Inicializa el grafo con los atributos grafo y entrada """
        self.grafo = {}
        self.entrada = {}

    def agregar_vertice(self, v):
        """ Recibe un vertiece y lo agrega al grafo si este no se encuentra en el grafo """
        if not v in self.grafo:
            self.grafo[v] = {}
            self.entrada[v] = []

    def borrar_vertice(self, v):
        """ Recibe un vertice y lo elimina si este se encuentra en el grafo """
        for diccionario in self.grafo.values():
            if v in diccionario:
                diccionario.pop(v)
        self.grafo.pop(v)
        for vertice in self.entrada:
            if v in self.entrada[vertice]: self.entrada[vertice].remove(v)


    def agregar_arista(self, v, w, peso):
        """
        Recibe un vertice 'v' y otro 'w' con un 'peso', y agrega al grafo 
        la arista que va desde 'v' a 'w' con el peso correspondiente.
        Si 'v' o 'w' no existen, aparecera un 'AssertionError'
        """
        if w not in self.grafo or v not in self.grafo:
            raise AssertionError("Uno de los vertices no esta en el grafo")
        self.grafo[v][w]=peso
        self.entrada[w].append(v)

    def borrar_arista(self, v, w):
        """
        Recibe dos vertices 'v' y 'w' y borra la arista que va desde 'v' a 'w', si alguno de los dos
        vertices no existe, se levantara un 'AssertionError' 
        """
        if v not in self.grafo:
            raise AssertionError(v,"No esta en el grafo")
        if w not in self.grafo:
            raise AssertionError(w,"No esta en el grafo")    
        if w not in self.grafo[v]:
            raise AssertionError(w,"No tiene arista con ",v)
        self.grafo[v].pop(w)
        self.entrada[w].remove(v)

    def estan_unidos(self, v, w):
        """ 
        Recibe dos vertices 'v' y 'w' y devuelve True si existe una arista de 'v' a 'w', de lo contrario develve false
        si alguno de los dos vertices no existe, se levantara un 'AssertionError' 
        """
        if v not in self.grafo:
            raise AssertionError(v,"No esta en el grafo")
        if w not in self.grafo:
            raise AssertionError(w,"No esta en el grafo")
        if w in self.grafo[v]:
            return True
        return False

    def peso_arista(self, v, w):
        """ 
        Recibe dos vertices 'v' y 'w' y devuelve el peso de la arista que va desde 'v' a 'w',
        si no existe 'v' o 'w' o no existe la arista, se levantara un 'Assertion error'
        """
        if v not in self.grafo:
            raise AssertionError(v,"No esta en el grafo")
        if w not in self.grafo:
            raise AssertionError(w,"No esta en el grafo")
        if v not in self.grafo:
            raise AssertionError(v,"No esta en el grafo")
        if w not in self.grafo[v]:
            raise AssertionError(w,"No tiene arista con ",v)
        return self.grafo[v][w]

    def obtener_vertices(self):
        """ Devuelve una lista con todos los vertices del grafo """
        return list(self.grafo.keys())

    def vertice_aleatorio(self):
        """ Devuelve un vertice aleatorio entre los vefrtices del grafo """
        lista = self.obtener_vertices()
        if len(lista) == 0: return None
        indice = random.randint(0,len(lista)-1)
        return lista[indice]

    def adyacentes(self, v):
        """
        Recibe un verice 'v' y devuelve una lista de todos los vertices adyacentes a 'v'.
        Si 'v' no existe se levantara un 'Assertion error'
        """
        if v not in self.grafo:
            raise AssertionError(v,"No esta en el grafo")
        return self.grafo[v].keys()
        
    def obtener_vertices_entrada(self, v):
        """
        Recibe un verice 'v' y devuelve una lista de todos los vertices que tienen como adyacente a 'v'.
        Si 'v' no existe se levantara un 'Assertion error'
        """
        if v not in self.grafo:
            raise AssertionError(v,"No esta en el grafo")
        return self.entrada[v]
