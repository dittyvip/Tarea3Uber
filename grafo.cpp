#include <iostream>
#include <fstream>
#include <string>
#include "grafo.h"

using namespace std;

void crearGrafo(Grafo& g, int n) {
    g.numnodos = n;
    g.ady = new int*[n];
    for (int i = 0; i < n; ++i) {
        g.ady[i] = new int[n];
        for (int j = 0; j < n; ++j) {
            g.ady[i][j] = 0; // Sin conexión
        }
    }
}

void agregarArco(Grafo& g, int origen, int destino) {
    // Ajustar a índices base 0 (archivo tiene numnumnodos desde 1)
    if (origen >= 1 && origen <= g.numnodos && destino >= 1 && destino <= g.numnodos)
        g.ady[origen - 1][destino - 1] = 1;
}

void destruirGrafo(Grafo& g) {
    for (int i = 0; i < g.numnodos; ++i) {
        delete[] g.ady[i];
    }
    delete[] g.ady;
    g.ady = nullptr;
    g.numnodos = 0;
}

void mostrarGrafo(const Grafo& g) {
    cout << "Matriz de adyacencia:\n";
    for (int i = 0; i < g.numnodos; ++i) {
        for (int j = 0; j < g.numnodos; ++j) {
            cout << g.ady[i][j] << " ";
        }
        cout << "\n";
    }
}

void leerArchivo(const char* nombreArchivo, Grafo& g, int*& conductores, int& cantidadConductores) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo) {
        std::cout << "No se pudo abrir el archivo.\n";
        g.ady = nullptr;
        conductores = nullptr;
        cantidadConductores = 0;
        return;
    }

    int nodos, aristas, nConductores;
    archivo >> nodos >> aristas >> nConductores;
    g.numnodos = nodos;

    // Crear matriz dinámica
    g.ady = new int*[nodos];
    for (int i = 0; i < nodos; ++i) {
        g.ady[i] = new int[nodos];
        for (int j = 0; j < nodos; ++j)
            g.ady[i][j] = 0;
    }

    // Leer aristas
    for (int i = 0; i < aristas; ++i) {
        int origen, destino;
        archivo >> origen >> destino;
        g.ady[origen - 1][destino - 1] = 1;
    }

    // Leer conductores
    cantidadConductores = nConductores;
    conductores = new int[cantidadConductores];
    for (int i = 0; i < cantidadConductores; ++i)
        archivo >> conductores[i];

    archivo.close();
}

void bfs(const Grafo& g, int inicio, int* distancias, int* padres) {
    // Inicializa los arreglos de distancias y padres
    for (int i = 0; i < g.numnodos; ++i) {
        distancias[i] = -1; // Marca todos los numnodos como no visitados
        padres[i] = -1;     // Inicializa los padres como -1 (sin padre)
    }

    int* cola = new int[g.numnodos];  // Reserva memoria para la cola manual (arreglo)
    int frente = 0, final = 0;     // Índices para el frente y final de la cola

    int idxInicio = inicio - 1;    // Convierte el nodo de inicio a índice base 0
    distancias[idxInicio] = 0;     // La distancia al nodo de inicio es 0
    cola[final++] = idxInicio;     // Encola el nodo de inicio

    // Mientras la cola no esté vacía
    while (frente < final) {
        int actual = cola[frente++]; // Toma el nodo actual del frente de la cola

        // Recorre todos los posibles vecinos del nodo actual
        for (int vecino = 0; vecino < g.numnodos; ++vecino) {
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

void solicitar_uber(Grafo& g, int origen, int destino, int*& ruta, int& largo, int& costo) {
    int* dist = new int[g.numnodos];    // Reserva memoria para el arreglo de distancias
    int* padres = new int[g.numnodos];  // Reserva memoria para el arreglo de padres

    bfs(g, origen, dist, padres);       // Realiza BFS desde el nodo origen

    // Si el destino no es alcanzable desde el origen
    if (dist[destino - 1] == -1) {
        ruta = nullptr; // No hay ruta posible
        largo = 0;      // Longitud de la ruta es 0
        costo = -1;     // Costo -1 indica que no hay ruta
    } else {
        reconstruir_ruta(padres, origen, destino, ruta, largo); // Reconstruye la ruta encontrada
        costo = 500 * (largo - 1); // Calcula el costo (500 por cada tramo)
    }

    delete[] dist;   // Libera la memoria del arreglo de distancias
    delete[] padres; // Libera la memoria del arreglo de padres
}


int main() {
    Grafo g;
    int* conductores;
    int cantidadConductores;

    leerArchivo("data1.txt", g, conductores, cantidadConductores);

    while (true) {
        int origen, destino;
        cout << "Ingrese viaje (origen destino): ";
        cin >> origen >> destino;

        if (origen == -1 && destino == -1)
            break;

        int* ruta = nullptr;
        int largoRuta = 0;
        int costo;

        solicitar_uber(g, origen, destino, ruta, largoRuta, costo);

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
