#ifndef ARBOLAVL_H
#define ARBOLAVL_H

#include <string>
#include <iostream>

using std::string;

// estructura para el historial de estados (una lista simple)
struct NodoEstado {
    string estado;
    string timestamp;
    NodoEstado* siguiente;
};

// estructura principal del nodo avl sin los campos de dijkstra
struct NodoAVL {
    string codigo_lote;
    string codigo_finca;
    string nombre_finca;
    int sacos;
    string tipo_cafe;
    string estado; 
    string hash_certificado; 
    
    NodoEstado* historial_estados; 

    // punteros del arbol y balance
    NodoAVL* izquierdo;
    NodoAVL* derecho;
    int altura;

    // constructor del nodo
    NodoAVL(string cod_lote, string cod_finca, string nom_finca, int cant_sacos, string tipo) {
        codigo_lote = cod_lote;
        codigo_finca = cod_finca;
        nombre_finca = nom_finca;
        sacos = cant_sacos;
        tipo_cafe = tipo;
        estado = "recibido";
        hash_certificado = "";
        historial_estados = nullptr;
        izquierdo = nullptr;
        derecho = nullptr;
        altura = 1; // un nodo nuevo siempre se inserta como hoja
    }
};

class ArbolMerkle;
class ArbolAVL {
private:
    NodoAVL* raiz;

    // funciones auxiliares internas
    int obtenerAltura(NodoAVL* nodo);
    int obtenerBalance(NodoAVL* nodo);
    
    // rotaciones
    NodoAVL* rotacionDerecha(NodoAVL* y);
    NodoAVL* rotacionIzquierda(NodoAVL* x);
    
    // insercion recursiva
    NodoAVL* insertarRecursivo(NodoAVL* nodo, string cod_lote, string cod_finca, string nom_finca, int sacos, string tipo);
    
    // recorridos
    void inOrderRecursivo(NodoAVL* nodo);

    void recorrerYGenerarCertificados(NodoAVL* nodo, ArbolMerkle& merkle, string fecha);

public:
    ArbolAVL();
    ~ArbolAVL();
    
    void insertar(string cod_lote, string cod_finca, string nom_finca, int sacos, string tipo);
    void mostrarInOrder();
    NodoAVL* buscarLote(string codigo_lote);
    void generarCertificadosLotes(ArbolMerkle& merkle, string fecha);
};

#endif