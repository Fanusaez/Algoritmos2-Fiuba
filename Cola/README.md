La entrega es muy similar a la realizada para el TDA Pila. Excepto que la función para destruir la cola recibe por parámetro una función para destruir uno a uno los elementos, que puede ser NULL en el caso de que no haya que destruirlos.

La cola debe ser enlazada, es decir que en lugar de usar un arreglo, usa nodos enlazados, de los cuales se desencola el primero y se encola a continuación del último. Por tanto, se debe implementar solamente con punteros, y no se debe guardar en un campo el tamaño de la cola.


