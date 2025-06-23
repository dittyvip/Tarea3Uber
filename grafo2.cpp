#include <iostream>
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
