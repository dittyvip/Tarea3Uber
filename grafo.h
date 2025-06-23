#ifndef GRAFO_H
#define GRAFO_H

struct Grafo {
    int** ady;
    int numnodos;
    int numArcos;

    Grafo() {
        ady = nullptr;
        numnodos = 0;
    }
};

void crearGrafo(Grafo& g, int n);
void agregarArco(Grafo& g, int origen, int destino);
void destruirGrafo(Grafo& g);
void mostrarGrafo(const Grafo& g);
void leerArchivo(const std::string& nombreArchivo, Grafo& grafo, int*& conductores, int& cantidadConductores);
void bfs_ruta(const Grafo& g, int inicio, int* distancias, int* padres);
int* reconstruirCamino(int* padre, int destino, int& largo);
int solicitar_uber(Grafo& g, int origen, int destino, int*& conductores, int nConductores);

#endif