#include <iostream>
#include <fstream>
using namespace std;

// Nodo de la lista de adyacencia
struct NodoAdyacente {
    int indiceDestino;                // Índice del nodo adyacente
    NodoAdyacente* siguiente;         // Puntero al siguiente nodo en la lista
};

// Lista ligada de nodos adyacentes
struct ListaAdyacencia {
    NodoAdyacente* primerNodo;        // Puntero al primer nodo de la lista

    ListaAdyacencia() : primerNodo(nullptr) {}

    // Agrega un nodo al final de la lista
    void agregar(int indiceDestino) {
        NodoAdyacente* nuevoNodo = new NodoAdyacente{indiceDestino, nullptr};
        if (!primerNodo) {
            primerNodo = nuevoNodo;
        } else {
            NodoAdyacente* actual = primerNodo;
            while (actual->siguiente)
                actual = actual->siguiente;
            actual->siguiente = nuevoNodo;
        }
    }

    // Libera la memoria de la lista
    void liberar() {
        NodoAdyacente* actual = primerNodo;
        while (actual) {
            NodoAdyacente* siguienteNodo = actual->siguiente;
            delete actual;
            actual = siguienteNodo;
        }
        primerNodo = nullptr;
    }
};

// Grafo dirigido sin pesos, usando listas de adyacencia
class Grafo {
private:
    int cantidadNodos;                    // Número total de nodos
    ListaAdyacencia* listasAdyacencia;    // Arreglo de listas de adyacencia

public:
    // Constructor: inicializa el grafo con n nodos
    Grafo(int n) : cantidadNodos(n) {
        listasAdyacencia = new ListaAdyacencia[cantidadNodos + 1]; // Índices desde 1
    }

    // Destructor: libera la memoria
    ~Grafo() {
        liberar();
    }

    // Agrega un arco dirigido de 'origen' a 'destino'
    void agregarArco(int origen, int destino) {
        if (origen < 1 || origen > cantidadNodos || destino < 1 || destino > cantidadNodos) return;
        listasAdyacencia[origen].agregar(destino);
    }

    // Imprime la lista de adyacencia de cada nodo
    void imprimir() {
        for (int i = 1; i <= cantidadNodos; i++) {
            cout << i << ": ";
            NodoAdyacente* actual = listasAdyacencia[i].primerNodo;
            while (actual) {
                cout << actual->indiceDestino << " ";
                actual = actual->siguiente;
            }
            cout << endl;
        }
    }

    // Libera toda la memoria usada por el grafo
    void liberar() {
        for (int i = 1; i <= cantidadNodos; i++) {
            listasAdyacencia[i].liberar();
        }
        delete[] listasAdyacencia;
    }

    // BFS: calcula distancias y padres desde el nodo 'inicio'
    void bfs(int inicio, int* distancia, int* padre) {
        // Inicializa distancias y padres
        for (int i = 1; i <= cantidadNodos; i++) {
            distancia[i] = -1;
            padre[i] = -1;
        }

        int* cola = new int[cantidadNodos + 1]; // Cola para BFS
        int frente = 0, fin = 0;

        distancia[inicio] = 0;
        cola[fin++] = inicio;

        while (frente < fin) {
            int nodoActual = cola[frente++];
            NodoAdyacente* ady = listasAdyacencia[nodoActual].primerNodo;

            while (ady) {
                int vecino = ady->indiceDestino;
                if (distancia[vecino] == -1) { // Si no ha sido visitado
                    distancia[vecino] = distancia[nodoActual] + 1;
                    padre[vecino] = nodoActual;
                    cola[fin++] = vecino;
                }
                ady = ady->siguiente;
            }
        }

        delete[] cola;
    }
};
//Lee el archivo de entrada y construye el grafo

void cargarGrafo(const char* nombreArchivo, Grafo*& grafo, int*& arregloConductores, int& cantidadConductores) {
    ifstream archivoEntrada(nombreArchivo); // Abrir archivo de entrada

    if (!archivoEntrada) { // Verificar si el archivo se abrió correctamente
        cout << "No se pudo abrir el archivo " << nombreArchivo << endl;
        return;
    }

    int cantidadNodos, cantidadArcos, cantidadConductoresArchivo; // N nodos, M arcos, C conductores
    archivoEntrada >> cantidadNodos >> cantidadArcos >> cantidadConductoresArchivo; // Leer cabecera

    grafo = new Grafo(cantidadNodos); // Crear grafo con la cantidad de nodos leída

    // Leer y agregar cada arco al grafo
    for (int i = 0; i < cantidadArcos; i++) {
        int nodoOrigen, nodoDestino;
        archivoEntrada >> nodoOrigen >> nodoDestino;
        grafo->agregarArco(nodoOrigen, nodoDestino);
    }

    arregloConductores = new int[cantidadConductoresArchivo]; // Crear arreglo para conductores

    // Leer los identificadores de los conductores
    for (int i = 0; i < cantidadConductoresArchivo; i++) {
        archivoEntrada >> arregloConductores[i];
    }

    cantidadConductores = cantidadConductoresArchivo; // Asignar la cantidad de conductores leída

    archivoEntrada.close(); // Cerrar el archivo
}
int main(){
    Grafo* grafo = nullptr;
    int* conductores = nullptr;
    int cantidadConductores = 0;

    cargarGrafo("data1.txt", grafo, conductores, cantidadConductores);

    grafo->imprimir(); 

    delete[] conductores;
    delete grafo;

    return 0;
}

