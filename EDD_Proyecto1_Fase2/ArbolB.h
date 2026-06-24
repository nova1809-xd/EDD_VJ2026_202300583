#ifndef ARBOLB_H
#define ARBOLB_H

#include <string>
#include <iostream>
#include "ArbolAVL.h" // incluimos tu codigo anterior para poder anidarlo

using std::string;

// estructura puente que agrupa la fecha y su respectivo arbol de lotes
struct ClaveB {
    string fecha;
    ArbolAVL* avl_lotes; // el arbol avl independiente para este dia

    ClaveB() {
        fecha = "";
        avl_lotes = nullptr;
    }
};

// estructura principal del nodo del arbol b
// segun la rubrica: orden minimo 3, maximo 6 claves y 7 hijos por nodo
struct NodoB {
    ClaveB claves[6];        // arreglo estatico de hasta 6 fechas
    NodoB* hijos[7];         // arreglo estatico de hasta 7 punteros a los sub-arboles
    int cantidad_claves;     // contador de cuantas fechas tiene este nodo actualmente
    bool es_hoja;            // bandera para identificar si estamos en el nivel mas bajo

    // constructor
    NodoB(bool hoja) {
        es_hoja = hoja;
        cantidad_claves = 0;
        // inicializamos todos los punteros de los hijos en nulo por seguridad
        for (int i = 0; i < 7; i++) {
            hijos[i] = nullptr;
        }
    }
};

class ArbolMerkle; // forward declaration para evitar dependencias circulares

class ArbolB {
private:
    NodoB* raiz;

    // funciones de ingenieria interna para manejar la insercion y division
    void dividirHijo(NodoB* padre, int indice, NodoB* hijo_lleno);
    void insertarNoLleno(NodoB* nodo, string fecha, string cod_lote, string cod_finca, string nom_finca, int sacos, string tipo);
    
    // recorridos y busquedas recursivas
    void inOrderRecursivo(NodoB* nodo);
    ArbolAVL* buscarRecursivo(NodoB* nodo, string fecha);

    // puentes para el arbol de merkle y busquedas especificas
    void recorrerBParaCertificados(NodoB* nodo, ArbolMerkle& merkle);
    NodoAVL* buscarLoteGlobalRecursivo(NodoB* nodo, string cod_lote, string& fecha_salida);

    void generarDotRecursivoB(NodoB* nodo, std::ofstream& archivo); 

public:
    ArbolB();
    ~ArbolB();

    // funcion maestra que llamaremos desde el menu y la carga json
    void insertarLote(string fecha, string cod_lote, string cod_finca, string nom_finca, int sacos, string tipo);
    
    // operaciones publicas
    ArbolAVL* buscarFecha(string fecha);
    void mostrarFechas();

    // llamadas publicas para los certificados
    void generarTodosLosCertificados(ArbolMerkle& merkle);
    NodoAVL* buscarLoteGlobal(string cod_lote, string& fecha_salida);

    void generarReporteB();
};

#endif