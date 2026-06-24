#ifndef ARBOLAVL_H
#define ARBOLAVL_H

#include <string>
#include <iostream>
#include <ctime>

using std::string;

// nodo simple para hacer la lista del historial de trazabilidad
struct HistorialEstado {
    string estado;
    string timestamp;
    HistorialEstado* siguiente;

    HistorialEstado(string est, string ts) {
        estado = est;
        timestamp = ts;
        siguiente = nullptr;
    }
};   

// estructura principal del nodo avl
struct NodoAVL {
    string codigo_lote;
    string codigo_finca;
    string nombre_finca;
    int sacos;
    string tipo_cafe;
    string estado; 
    string hash_certificado; 
    
    // puntero a la lista del historial
    HistorialEstado* cabeza_historial; 

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
        izquierdo = nullptr;
        derecho = nullptr;
        altura = 1; // un nodo nuevo siempre se inserta como hoja
        
        // inicializamos el historial y metemos el primer estado
        cabeza_historial = nullptr;
        agregarAlHistorial("recibido");
    }

    // funcion interna del nodo para agregar al historial con la hora actual
    void agregarAlHistorial(string nuevo_estado) {
        time_t ahora = time(0);
        tm* tiempo = localtime(&ahora);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tiempo);
        string ts(buffer);

        HistorialEstado* nuevo = new HistorialEstado(nuevo_estado, ts);
        
        if (cabeza_historial == nullptr) {
            cabeza_historial = nuevo;
        } else {
            HistorialEstado* actual = cabeza_historial;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
    }
};

class ArbolMerkle; // forward declaration

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