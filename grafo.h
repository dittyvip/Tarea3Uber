#ifndef GRAFO_H
#define GRAFO_H

struct Grafo {
    int** ady;     // Matriz de adyacencia dinámica
    int nodos;     // Número de nodos

    // Constructor (inicializa en nullptr)
    Grafo() {
        ady = nullptr;
        nodos = 0;
    }
};

void crearGrafo(Grafo& g, int n);
void agregarArco(Grafo& g, int origen, int destino);
void destruirGrafo(Grafo& g);
void mostrarGrafo(const Grafo& g); // (opcional para debug)

#endif
