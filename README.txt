Tarea 3 - Uber

Integrantes:
- Diana Gutierrez rol: 202473594-2
- Alexa Barrios rol: 202473506-3

+Para leer el data2.txt solo tiene que cambiar el "data1.txt" por "data2.txt" en el main() en la funcion. 
leerArchivo("data1.txt", g, conductores, cantidadConductores).

El programa pedirá al usuario que ingrese un viaje como dos números (origen y destino).
Ejemplo:
Ingrese viaje (origen destino): 1 10  
El programa imprimirá la ruta óptima y su costo.

Para terminar, ingresar:
-1 -1


Análisis de Complejidad - solicitar_uber(...)

Resumen de la función
La función realiza los siguientes pasos principales:

1.BFS desde el origen para encontrar el conductor más cercano.
2.BFS desde el origen al destino para verificar si existe ruta.
3.BFS desde el conductor al origen y reconstrucción de ruta.
4.BFS desde el origen al destino y reconstrucción de ruta.
5.Unión de rutas y cálculo de costo.

Análisis

1. Búsqueda del conductor más cercano
    Se realiza un BFS desde el nodo origen.
    BFS en un grafo con matriz de adyacencia y n nodos tiene complejidad O(n²).
    Luego, se recorre el arreglo de conductores (de tamaño c), lo cual es O(c).

2. Verificación de ruta origen-destino
    Se realiza otro BFS desde origen a destino: O(n²).

3. Ruta del conductor al origen
    BFS desde el nodo del conductor al origen: O(n²).
    Reconstrucción de la ruta: en el peor caso, recorre todos los nodos, O(n).

4. Ruta del origen al destino
    BFS desde el origen al destino: O(n²).
    Reconstrucción de la ruta: O(n).

5. Unión de rutas y cálculo de costo
    Copiar las rutas y calcular el costo: en el peor caso, suma las longitudes de ambas rutas, O(n).

Complejidad total:

+BFS se ejecuta 4 veces: 4 × O(n²) = O(n²)
+Recorridos de arreglos y reconstrucción de rutas: O(n) + O(c)
+Unión de rutas y cálculo de costo: O(n)

Complejidad temporal total:
O(n² + c)
  +Donde n es el número de nodos y c la cantidad de conductores.
  +Si c es mucho menor que n², la complejidad es O(n²). 
  +Por lo tanto la función solicitar_uber(...) tiene complejidad temporal O(n² + c), siendo n el número de nodos 
  del grafo y c la cantidad de conductores. 