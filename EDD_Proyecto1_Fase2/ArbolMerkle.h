#ifndef ARBOLMERKLE_H
#define ARBOLMERKLE_H

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using std::string;

// nodo para el arbol de merkle
struct NodoMerkle {
    string hash;
    NodoMerkle* izquierdo;
    NodoMerkle* derecho;

    NodoMerkle(string h) {
        hash = h;
        izquierdo = nullptr;
        derecho = nullptr;
    }
};

// lista simple para guardar los hashes de los bloques de datos temporalmente
struct NodoHashList {
    string hash;
    NodoHashList* siguiente;

    NodoHashList(string h) {
        hash = h;
        siguiente = nullptr;
    }
};

class ArbolMerkle {
private:
    NodoMerkle* raiz;
    NodoHashList* cabeza_hashes;
    int cantidad_bloques;

    // funciones internas
    string generarHashCasero(string contenido);
    void limpiarArbol(NodoMerkle* nodo);
    NodoMerkle* construirArbolRecursivo(NodoHashList* inicio, int cantidad);
    NodoHashList* obtenerNodoEnIndice(NodoHashList* inicio, int indice);

public:
    ArbolMerkle();
    ~ArbolMerkle();

    // generacion de certificados (opcion 4)
    string generarCertificadoFisico(string fecha, string cod_lote, string cod_finca, string nombre_finca, int sacos, string tipo_cafe);
    
    // construccion del arbol (opcion 5)
    void construirArbol();
    void mostrarRootHash();
};

#endif