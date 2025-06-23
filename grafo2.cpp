#include <iostream>
#include <fstream>
#include <string>
#include "grafo.h"

using namespace std;

void crearGrafo(Grafo& g, int n) {
    g.nodos = n;
    g.ady = new int*[n];
    for (int i = 0; i < n; ++i) {
        g.ady[i] = new int[n];
        for (int j = 0; j < n; ++j) {
            g.ady[i][j] = 0; // Sin conexión
        }
    }
}

void agregarArco(Grafo& g, int origen, int destino) {
    // Ajustar a índices base 0 (archivo tiene nodos desde 1)
    if (origen >= 1 && origen <= g.nodos && destino >= 1 && destino <= g.nodos)
        g.ady[origen - 1][destino - 1] = 1;
}

void destruirGrafo(Grafo& g) {
    for (int i = 0; i < g.nodos; ++i) {
        delete[] g.ady[i];
    }
    delete[] g.ady;
    g.ady = nullptr;
    g.nodos = 0;
}

void mostrarGrafo(const Grafo& g) {
    cout << "Matriz de adyacencia:\n";
    for (int i = 0; i < g.nodos; ++i) {
        for (int j = 0; j < g.nodos; ++j) {
            cout << g.ady[i][j] << " ";
        }
        cout << "\n";
    }
}


void leerArchivo(const string& nombreArchivo, Grafo& grafo, int*& conductores, int& cantidadConductores) {
    ifstream archivo(nombreArchivo); // Abre el archivo de entrada

    if (!archivo) { // Verifica si el archivo se abrió correctamente
        cerr << "Error al abrir el archivo " << nombreArchivo << endl; // Muestra mensaje de error
        exit(1); // Termina el programa con error
    }

    int nodos, arcos; // Variables para la cantidad de nodos y arcos
    archivo >> nodos >> arcos >> cantidadConductores; // Lee la cabecera: nodos, arcos y conductores

    crearGrafo(grafo, nodos); // Inicializa el grafo con la cantidad de nodos

    // Lee y agrega cada arco al grafo
    for (int i = 0; i < arcos; ++i) {
        int origen, destino; // Variables para los nodos de cada arco
        archivo >> origen >> destino; // Lee el origen y destino del arco
        agregarArco(grafo, origen, destino); // Agrega el arco al grafo
    }

    // Leer los identificadores de los conductores
    conductores = new int[cantidadConductores]; // Reserva memoria para el arreglo de conductores
    for (int i = 0; i < cantidadConductores; ++i) {
        archivo >> conductores[i]; // Lee cada identificador de conductor
    }

    archivo.close(); // Cierra el archivo
}


void bfs(const Grafo& g, int inicio, int* distancias, int* padres) {
    // Inicializa los arreglos de distancias y padres
    for (int i = 0; i < g.nodos; ++i) {
        distancias[i] = -1; // Marca todos los nodos como no visitados
        padres[i] = -1;     // Inicializa los padres como -1 (sin padre)
    }

    int* cola = new int[g.nodos];  // Reserva memoria para la cola manual (arreglo)
    int frente = 0, final = 0;     // Índices para el frente y final de la cola

    int idxInicio = inicio - 1;    // Convierte el nodo de inicio a índice base 0
    distancias[idxInicio] = 0;     // La distancia al nodo de inicio es 0
    cola[final++] = idxInicio;     // Encola el nodo de inicio

    // Mientras la cola no esté vacía
    while (frente < final) {
        int actual = cola[frente++]; // Toma el nodo actual del frente de la cola

        // Recorre todos los posibles vecinos del nodo actual
        for (int vecino = 0; vecino < g.nodos; ++vecino) {
            // Si hay un arco y el vecino no ha sido visitado
            if (g.ady[actual][vecino] == 1 && distancias[vecino] == -1) {
                distancias[vecino] = distancias[actual] + 1; // Actualiza la distancia del vecino
                padres[vecino] = actual;                     // Guarda el padre del vecino
                cola[final++] = vecino;                      // Encola el vecino para seguir el recorrido
            }
        }
    }

    delete[] cola; // Libera la memoria de la cola
}

void reconstruir_ruta(int* padres, int inicio, int fin, int*& ruta, int& largo) {
    int temp[1000]; // Arreglo temporal para almacenar la ruta (máximo 1000 nodos)
    int idx = 0;    // Índice para el arreglo temporal
    int actual = fin; // Comienza desde el nodo destino (fin)

    // Recorre hacia atrás usando el arreglo de padres hasta llegar al inicio o a un nodo sin padre
    while (actual != -1 && actual != inicio - 1) {
        temp[idx++] = actual;        // Guarda el nodo actual en temp
        actual = padres[actual];     // Avanza al padre del nodo actual
    }

    // Si no se llegó al nodo de inicio, no hay ruta
    if (actual == -1) {
        ruta = nullptr;  // No hay ruta posible
        largo = 0;       // Longitud de la ruta es 0
        return;
    }

    temp[idx++] = inicio - 1; // Agrega el nodo de inicio a la ruta

    ruta = new int[idx]; // Reserva memoria para la ruta final
    largo = idx;         // Guarda la longitud de la ruta

    // Copia la ruta en orden correcto 
    for (int i = 0; i < idx; ++i) {
        ruta[i] = temp[idx - i - 1] + 1; 
    }
}

void solicitar_uber(Grafo& g, int* conductores, int cantidadConductores,
                    int origen, int destino,
                    int*& rutaFinal, int& largoRuta, int& costo) {
    int* dist = new int[g.nodos];    // Arreglo para distancias en BFS
    int* padres = new int[g.nodos];  // Arreglo para padres en BFS

    // Paso 1: BFS desde el pasajero para encontrar el conductor más cercano
    bfs(g, origen, dist, padres);

    int mejorConductor = -1;         // Índice del mejor conductor en el arreglo
    int distanciaMin = g.nodos + 1;  // Inicializa la distancia mínima con un valor grande

    // Busca el conductor más cercano al origen
    for (int i = 0; i < cantidadConductores; ++i) {
        int idx = conductores[i] - 1; // Índice base 0 del conductor
        if (dist[idx] != -1 && dist[idx] < distanciaMin) { // Si el conductor es alcanzable y más cercano
            distanciaMin = dist[idx];
            mejorConductor = i;
        }
    }

    // Si no hay conductor alcanzable, termina y retorna sin ruta
    if (mejorConductor == -1) {
        costo = -1;
        largoRuta = 0;
        rutaFinal = nullptr;
        delete[] dist;
        delete[] padres;
        return;
    }

    int conductorNodo = conductores[mejorConductor]; // Nodo donde está el mejor conductor

    // Paso 2: BFS desde origen a destino para verificar si hay ruta
    bfs(g, origen, dist, padres);
    if (dist[destino - 1] == -1) { // Si no hay ruta al destino
        costo = -1;
        largoRuta = 0;
        rutaFinal = nullptr;
        delete[] dist;
        delete[] padres;
        return;
    }

    int* ruta1; int largo1; // Ruta del conductor al origen
    int* ruta2; int largo2; // Ruta del origen al destino

    // BFS y reconstrucción de ruta del conductor al origen
    bfs(g, conductorNodo, dist, padres);
    reconstruir_ruta(padres, conductorNodo, origen, ruta1, largo1);

    // BFS y reconstrucción de ruta del origen al destino
    bfs(g, origen, dist, padres);
    reconstruir_ruta(padres, origen, destino, ruta2, largo2);

    // Paso 3: unir rutas (evita repetir el nodo origen)
    largoRuta = largo1 + largo2 - 1;
    rutaFinal = new int[largoRuta];
    for (int i = 0; i < largo1; ++i) rutaFinal[i] = ruta1[i]; // Copia ruta del conductor al origen
    for (int i = 1; i < largo2; ++i) rutaFinal[largo1 + i - 1] = ruta2[i]; // Copia ruta del origen al destino (sin repetir el origen)

    // Paso 4: calcular costo (300 por cada tramo conductor-origen, 500 por cada tramo origen-destino)
    costo = 300 * (largo1 - 1) + 500 * (largo2 - 1);

    // Paso 5: actualizar la posición del conductor (ahora está en el destino)
    conductores[mejorConductor] = destino;

    // Libera la memoria dinámica usada
    delete[] dist;
    delete[] padres;
    delete[] ruta1;
    delete[] ruta2;
}


int main() {
    Grafo g;
    int* conductores;
    int cantidadConductores;

    // Leer el grafo y los conductores desde el archivo
    leerArchivo("data1.txt", g, conductores, cantidadConductores);
    // Si quiere probar con el otro mapa use "data2.txt"

    while (true) {
        int origen, destino;
        cout << "Ingrese viaje (origen destino): ";
        cin >> origen >> destino;

        if (origen == -1 && destino == -1)
            break;

        int* ruta = nullptr;
        int largoRuta = 0;
        int costo;

        solicitar_uber(g, conductores, cantidadConductores, origen, destino, ruta, largoRuta, costo);

        if (costo == -1) {
            cout << "Ruta: {} - Costo: -1" << endl;
        } else {
            cout << "Ruta: {";
            for (int i = 0; i < largoRuta; ++i) {
                cout << ruta[i];
                if (i < largoRuta - 1) cout << ", ";
            }
            cout << "} - Costo: " << costo << endl;
        }

        delete[] ruta;
    }

    destruirGrafo(g);
    delete[] conductores;
    return 0;
}




