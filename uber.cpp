#include <iostream>
#include <fstream>
#include "grafo.h"

void bfs_ruta(const Grafo& g, int inicio, int* distancia, int* padre) {
    for (int i = 0; i < g.numnodos; i++) {
        distancia[i] = -1;
        padre[i] = -1;
    }

    int* cola = new int[g.numnodos];
    int frente = 0, fin = 0;

    distancia[inicio] = 0;
    cola[fin++] = inicio;

    while (frente < fin) {
        int actual = cola[frente++];
        for (int i = 0; i < g.numnodos; ++i) {
            if (g.ady[actual][i] == 1 && distancia[i] == -1) {
                distancia[i] = distancia[actual] + 1;
                padre[i] = actual;
                cola[fin++] = i;
            }
        }
    }

    delete[] cola;
}
int* reconstruirCamino(int* padre, int destino, int& largo) {
    int* temp = new int[100]; // tamaño seguro
    int idx = 0;

    while (destino != -1) {
        temp[idx++] = destino + 1;  // convertir a índice desde 1
        destino = padre[destino];
    }

    int* camino = new int[idx];
    for (int i = 0; i < idx; ++i)
        camino[i] = temp[idx - i - 1];

    largo = idx;
    delete[] temp;
    return camino;
}

int solicitar_uber(Grafo& g, int origen, int destino, int*& conductores, int nConductores) {
    origen--; destino--;

    int* dist = new int[g.numnodos];
    int* padre = new int[g.numnodos];
    int mejorDist = -1, mejorIndice = -1, mejorNodo = -1;

    // Buscar conductor más cercano
    for (int i = 0; i < nConductores; ++i) {
        int posCond = conductores[i] - 1;
        bfs_ruta(g, posCond + 1, dist, padre);

        if (dist[origen] != -1) {
            if (mejorDist == -1 || dist[origen] < mejorDist ||
               (dist[origen] == mejorDist && conductores[i] < conductores[mejorIndice])) {
                mejorDist = dist[origen];
                mejorIndice = i;
                mejorNodo = posCond;
            }
        }
    }

    if (mejorDist == -1) {
        std::cout << "No hay conductor disponible para llegar al pasajero.\n";
        delete[] dist;
        delete[] padre;
        return -1;
    }

    // Ruta conductor → pasajero
    bfs_ruta(g, mejorNodo + 1, dist, padre);
    int largo1;
    int* ruta1 = reconstruirCamino(padre, origen, largo1);
    int DConductor = dist[origen];

    // Ruta pasajero → destino
    bfs_ruta(g, origen + 1, dist, padre);
    if (dist[destino] == -1) {
        std::cout << "El destino no es alcanzable desde el origen.\n";
        delete[] dist;
        delete[] padre;
        delete[] ruta1;
        return -1;
    }

    int largo2;
    int* ruta2 = reconstruirCamino(padre, destino, largo2);
    int DViaje = dist[destino];

    std::cout << "Ruta del conductor al pasajero: ";
    for (int i = 0; i < largo1; ++i) std::cout << ruta1[i] << " ";
    std::cout << "\n";

    std::cout << "Ruta del pasajero al destino: ";
    for (int i = 0; i < largo2; ++i) std::cout << ruta2[i] << " ";
    std::cout << "\n";

    int costo = 300 * DConductor + 500 * DViaje;
    std::cout << "Costo del viaje: " << costo << "\n";

    conductores[mejorIndice] = destino + 1; // actualizar posición conductor

    delete[] dist;
    delete[] padre;
    delete[] ruta1;
    delete[] ruta2;

    return costo;
}


int main() {
    Grafo g;
    int* conductores;
    int cantidadConductores;

    leerArchivo("data1.txt", g, conductores, cantidadConductores);
    mostrarGrafo(g);

    std::cout << "Conductores iniciales: ";
    for (int i = 0; i < cantidadConductores; ++i)
        std::cout << conductores[i] << " ";
    std::cout << "\n";

    while (true) {
        int origen, destino;
        std::cout << "Ingrese origen y destino (-1 -1 para salir): ";
        std::cin >> origen >> destino;
        if (origen == -1 && destino == -1) break;

        solicitar_uber(g, origen, destino, conductores, cantidadConductores);
    }

    delete[] conductores;
    destruirGrafo(g);
    return 0;
}