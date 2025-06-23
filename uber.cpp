#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include "grafo.h"
using namespace std;

const int MAX_NODOS = 100;  // por si se amplía después

struct Grafo {
    int** ady;  // matriz de adyacencia
    int numNodos;
    int numArcos;
};

void inicializarGrafo(Grafo &g, int nodos) {
    g.numNodos = nodos;
    for (int i = 0; i < nodos; ++i)
        for (int j = 0; j < nodos; ++j)
            g.ady[i][j] = 0;  // sin conexión
}

void agregarArco(Grafo &g, int origen, int destino) {
    g.ady[origen - 1][destino - 1] = 1;  // ajustado a índices desde 0
}

void crearGrafo(Grafo& g, int n) {
    g.numNodos= n;
    g.ady = new int*[n];
    for (int i = 0; i < n; ++i) {
        g.ady[i] = new int[n];
        for (int j = 0; j < n; ++j)
            g.ady[i][j] = 0; // Inicializamos sin conexiones
    }
}

void agregarArco(Grafo& g, int origen, int destino) {
    // Se resta 1 porque los nodos vienen numerados desde 1 en el input
    g.ady[origen - 1][destino - 1] = 1;
}

void destruirGrafo(Grafo& g) {
    for (int i = 0; i < g.numNodos; ++i)
        delete[] g.ady[i];
    delete[] g.ady;
    g.ady = nullptr;
    g.numNodos = 0;
}

void mostrarGrafo(const Grafo& g) {
    std::cout << "Matriz de adyacencia:\n";
    for (int i = 0; i < g.numNodos; ++i) {
        for (int j = 0; j < g.numNodos; ++j)
            std::cout << g.ady[i][j] << " ";
        std::cout << "\n";
    }
}

void cargarDatosDesdeArchivo(const char* nombreArchivo, Grafo& g, int*& conductores, int& nConductores) {
    ifstream archivo(nombreArchivo);

    if (!archivo) {
        cout << "No se pudo abrir el archivo.\n";
        return;
    }

    int nodos, arcos;
    archivo >> nodos >> arcos >> nConductores;

    crearGrafo(g, nodos); // usamos nuestro TDA

    for (int i = 0; i < arcos; ++i) {
        int origen, destino;
        archivo >> origen >> destino;
        agregarArco(g, origen, destino); // función del TDA
    }

    // Crear arreglo dinámico de conductores
    conductores = new int[nConductores];
    for (int i = 0; i < nConductores; ++i) {
        archivo >> conductores[i];
    }

    archivo.close();
}
int main() {
    Grafo g;
    int numNodos, numArcos, numConductores;
    int* conductores = nullptr;
    int nConductores = 0;

    ifstream archivo("data.txt");
    if (!archivo) {
        cout << "Error al abrir el archivo" << endl;
        return 1;
    }

    archivo >> numNodos >> numArcos >> numConductores;
    inicializarGrafo(g, numNodos);
    g.numArcos = numArcos;

    // Leer arcos
    for (int i = 0; i < numArcos; ++i) {
        int origen, destino;
        archivo >> origen >> destino;
        agregarArco(g, origen, destino);
    }

    // Leer ubicaciones de conductores
    for (int i = 0; i < numConductores; ++i) {
        archivo >> conductores[i];
    }

    archivo.close();

    // Mostrar grafo (opcional para pruebas)
    cout << "Matriz de adyacencia:" << endl;
    for (int i = 0; i < numNodos; ++i) {
        for (int j = 0; j < numNodos; ++j)
            cout << g.ady[i][j] << " ";
        cout << endl;
    }

    cout << "Conductores en nodos: ";
    for (int i = 0; i < numConductores; ++i)
        cout << conductores[i] << " ";
    cout << endl;


    Grafo ciudad;
    crearGrafo(ciudad, 4);

    agregarArco(ciudad, 1, 2);
    agregarArco(ciudad, 2, 3);
    agregarArco(ciudad, 3, 4);

    mostrarGrafo(ciudad);

    cargarDatosDesdeArchivo("data1.txt", ciudad, conductores, nConductores);

    cout << "Datos cargados correctamente.\n";
    cout << "Cantidad de conductores: " << nConductores << "\n";

    // mostrarGrafo(ciudad); // opcional
    // mostrar ubicaciones de los conductores
    for (int i = 0; i < nConductores; ++i)
        cout << "Conductor " << i + 1 << " en nodo " << conductores[i] << "\n";
    delete[] conductores;
    destruirGrafo(g);
    
    return 0;
}
