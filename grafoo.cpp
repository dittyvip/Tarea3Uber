#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Estructura para representar el grafo
struct Grafo {
    int numnodos;   // Número de nodos
    int** ady;      // Matriz de adyacencia
};

// Crea un grafo con n nodos y sin aristas
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

// Agrega un arco dirigido de 'origen' a 'destino'
void agregarArco(Grafo& g, int origen, int destino) {
    if (origen >= 1 && origen <= g.numnodos && destino >= 1 && destino <= g.numnodos)
        g.ady[origen - 1][destino - 1] = 1;
}

// Libera la memoria ocupada por el grafo
void destruirGrafo(Grafo& g) {
    for (int i = 0; i < g.numnodos; ++i) {
        delete[] g.ady[i];
    }
    delete[] g.ady;
    g.ady = nullptr;
    g.numnodos = 0;
}

// Lee el grafo y los conductores desde un archivo
void leerArchivo(const char* nombreArchivo, Grafo& g, int*& conductores, int& cantidadConductores) {
    ifstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "No se pudo abrir el archivo.\n";
        g.ady = nullptr;
        conductores = nullptr;
        cantidadConductores = 0;
        return;
    }

    int nodos, aristas, nConductores;
    archivo >> nodos >> aristas >> nConductores; // Lee la cabecera
    crearGrafo(g, nodos); // Inicializa el grafo

    // Lee las aristas y las agrega al grafo
    for (int i = 0; i < aristas; ++i) {
        int origen, destino;
        archivo >> origen >> destino;
        agregarArco(g, origen, destino);
    }

    // Lee los conductores
    cantidadConductores = nConductores;
    conductores = new int[cantidadConductores];
    for (int i = 0; i < cantidadConductores; ++i)
        archivo >> conductores[i];

    archivo.close();
}

// Algoritmo BFS para calcular distancias y padres desde un nodo de inicio
void bfs(const Grafo& g, int inicio, int* distancias, int* padres) {
    for (int i = 0; i < g.numnodos; ++i) {
        distancias[i] = -1; // Marca todos los nodos como no visitados
        padres[i] = -1;     // Inicializa los padres como -1
    }

    int* cola = new int[g.numnodos]; // Cola para BFS
    int frente = 0, final = 0;

    int idxInicio = inicio - 1;      // Convierte a índice base 0
    distancias[idxInicio] = 0;       // Distancia al nodo de inicio es 0
    cola[final++] = idxInicio;       // Encola el nodo de inicio

    while (frente < final) {
        int actual = cola[frente++]; // Toma el nodo actual de la cola
        for (int vecino = 0; vecino < g.numnodos; ++vecino) {
            // Si hay un arco y el vecino no ha sido visitado
            if (g.ady[actual][vecino] == 1 && distancias[vecino] == -1) {
                distancias[vecino] = distancias[actual] + 1; // Actualiza la distancia
                padres[vecino] = actual;                     // Guarda el padre
                cola[final++] = vecino;                      // Encola el vecino
            }
        }
    }

    delete[] cola; // Libera la memoria de la cola
}

// Reconstruye la ruta desde 'inicio' hasta 'fin' usando el arreglo de padres
void reconstruir_ruta(int* padres, int inicio, int fin, int*& ruta, int& largo) {
    int temp[1000]; // Arreglo temporal para la ruta (máx 1000 nodos)
    int idx = 0;
    int actual = fin - 1; // Índice base 0 del destino

    // Recorre hacia atrás usando los padres hasta llegar al inicio
    while (actual != -1 && actual != inicio - 1) {
        temp[idx++] = actual;
        actual = padres[actual];
    }

    // Si no se llegó al nodo de inicio, no hay ruta
    if (actual == -1) {
        ruta = nullptr;
        largo = 0;
        return;
    }

    temp[idx++] = inicio - 1; // Agrega el nodo de inicio
    ruta = new int[idx];      // Reserva memoria para la ruta final
    largo = idx;              // Guarda la longitud de la ruta

    // Copia la ruta en orden correcto (de inicio a fin)
    for (int i = 0; i < idx; ++i) {
        ruta[i] = temp[idx - i - 1] + 1; // Convierte a base 1
    }
}

void solicitar_uber(Grafo& g, int origen, int destino, int*& ruta, int& largo, int& costo) {
    int* dist = new int[g.numnodos];    // Arreglo para distancias
    int* padres = new int[g.numnodos];  // Arreglo para padres

    bfs(g, origen, dist, padres);       // Realiza BFS desde el nodo origen

    if (dist[destino - 1] == -1) {     // Si el destino no es alcanzable
        ruta = nullptr;
        largo = 0;
        costo = -1;
    } else {
        reconstruir_ruta(padres, origen, destino, ruta, largo); // Reconstruye la ruta
        costo = 500 * (largo - 1); // Calcula el costo (500 por cada tramo)
    }

    delete[] dist;   // Libera la memoria de distancias
    delete[] padres; // Libera la memoria de padres
}

int main() {
    Grafo g;
    int* conductores;
    int cantidadConductores;

    leerArchivo("data1.txt", g, conductores, cantidadConductores); // Lee el grafo y conductores

    while (true) {
        int origen, destino;
        cout << "Ingrese viaje (origen destino): ";
        cin >> origen >> destino;

        if (origen == -1 && destino == -1) // Condición de salida
            break;

        int* ruta = nullptr;
        int largoRuta = 0;
        int costo;

        solicitar_uber(g, origen, destino, ruta, largoRuta, costo); // Calcula la ruta y el costo

        if (costo == -1) {
            cout << "Ruta: {} - Costo: -1" << endl; // No hay ruta posible
        } else {
            cout << "Ruta: {";
            for (int i = 0; i < largoRuta; ++i) {
                cout << ruta[i];
                if (i < largoRuta - 1) cout << ", ";
            }
            cout << "} - Costo: " << costo << endl;
        }

        delete[] ruta; // Libera la memoria de la ruta
    }

    destruirGrafo(g);      // Libera la memoria del grafo
    delete[] conductores;  // Libera la memoria de conductores
    return 0;
}
