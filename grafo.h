#ifndef GRAFO_H
#define GRAFO_H
#include <string>
using std::string;

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
void bfs(const Grafo& g, int inicio, int* distancias, int* padres);
void leerArchivo(const string& nombreArchivo, Grafo& grafo, int& conductores, int& cantidadConductores);
void solicitar_uber(Grafo& g, int conductores, int cantidadConductores,
                    int origen, int destino,
                    int*& rutaFinal, int& largoRuta, int& costo);
#endif