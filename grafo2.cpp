#include <iostream>
#include <fstream>
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


int main() {
    Grafo g;
    int* conductores;
    int cantidadConductores;

    leerArchivo("data1.txt", g, conductores, cantidadConductores);

    mostrarGrafo(g);

    cout << "Conductores: ";
    for (int i = 0; i < cantidadConductores; ++i)
        cout << conductores[i] << " ";
    cout << endl;

    destruirGrafo(g);
    delete[] conductores;
    return 0;
}

