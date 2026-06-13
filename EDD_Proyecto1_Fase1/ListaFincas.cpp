#include "ListaFincas.h"
#include <iostream>

using std::cout;
using std::endl;

// --- metodos de la pila interna de entregas ---

PilaEntregas::PilaEntregas() {
    tope = nullptr;
}

PilaEntregas::~PilaEntregas() {
    NodoEntrega* actual = tope;
    while (actual != nullptr) {
        NodoEntrega* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    tope = nullptr;
}

void PilaEntregas::push(const string& fecha, int cantidad_sacos) {
    NodoEntrega* nuevo = new NodoEntrega;
    nuevo->fecha = fecha;
    nuevo->cantidad_sacos = cantidad_sacos;
    nuevo->siguiente = tope;
    tope = nuevo;
}

void PilaEntregas::mostrarEntregas() const {
    NodoEntrega* actual = tope;
    while (actual != nullptr) {
        cout << "  - fecha: " << actual->fecha << ", sacos: " << actual->cantidad_sacos << endl;
        actual = actual->siguiente;
    }
}

// --- metodos de la lista principal de fincas ---

ListaFincas::ListaFincas() {
    cabeza = nullptr;
}

ListaFincas::~ListaFincas() {
    // se libera la lista de fincas y es vital limpiar cada pila interna tambien
    NodoFinca* actual = cabeza;
    while (actual != nullptr) {
        NodoFinca* siguiente = actual->siguiente;
        delete actual->historial_entregas; // esto llama al destructor de PilaEntregas
        delete actual;
        actual = siguiente;
    }
    cabeza = nullptr;
}

void ListaFincas::registrarFinca(const string& nombre_finca) {
    // se inserta la nueva finca al inicio de la lista simple
    NodoFinca* nueva = new NodoFinca;
    nueva->nombre_finca = nombre_finca;
    nueva->historial_entregas = new PilaEntregas(); // se inicializa su pila de entregas vacia
    nueva->siguiente = cabeza;
    cabeza = nueva;
}

void ListaFincas::registrarEntregaFinca(const string& nombre_finca, const string& fecha, int cantidad_sacos) {
    // primero se busca la finca en la lista
    NodoFinca* actual = cabeza;
    while (actual != nullptr) {
        if (actual->nombre_finca == nombre_finca) {
            // al encontrarla, se hace el push directamente a su propia pila
            actual->historial_entregas->push(fecha, cantidad_sacos);
            return;
        }
        actual = actual->siguiente;
    }
    cout << "error: la finca " << nombre_finca << " no esta registrada." << endl;
}

void ListaFincas::mostrarTodo() const {
    NodoFinca* actual = cabeza;
    while (actual != nullptr) {
        cout << "finca: " << actual->nombre_finca << endl;
        cout << " entregas registradas:" << endl;
        actual->historial_entregas->mostrarEntregas();
        cout << "-------------------------------" << endl;
        actual = actual->siguiente;
    }
}