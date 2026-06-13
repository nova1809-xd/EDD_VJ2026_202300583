#ifndef LISTADOBLE_H
#define LISTADOBLE_H

#include <string>
#include <fstream>

using std::string;

// este nodo guarda la informacion de un lote de cafe y los enlaces de la lista
struct NodoLote {
    string codigo_lote;
    string finca_origen;
    int cantidad_sacos;
    string fecha_recepcion;
    int nivel_minimo;
    NodoLote* siguiente;
    NodoLote* anterior;
};

// esta clase maneja una lista doble enlazada ordenada por codigo de lote
class ListaDoble {
private:
    NodoLote* cabeza;
    NodoLote* cola;

public:
    ListaDoble();
    ~ListaDoble();

    void insertarOrdenado(const string& codigo_lote,
                          const string& finca_origen,
                          int cantidad_sacos,
                          const string& fecha_recepcion,
                          int nivel_minimo);

    void mostrar() const;
    bool estaVacia() const;
    void generarReporteGrafo() const;
};

#endif