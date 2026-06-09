#include "ListaDoble.h"
#include <iostream>

using std::cout;
using std::endl;

ListaDoble::ListaDoble() {
    cabeza = nullptr;
    cola = nullptr;
}

ListaDoble::~ListaDoble() {
    // se recorre la lista y se libera cada nodo para evitar fugas de memoria
    NodoLote* actual = cabeza;
    while (actual != nullptr) {
        NodoLote* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }

    cabeza = nullptr;
    cola = nullptr;
}

bool ListaDoble::estaVacia() const {
    return cabeza == nullptr;
}

void ListaDoble::insertarOrdenado(const string& codigo_lote,
                                  const string& finca_origen,
                                  int cantidad_sacos,
                                  const string& fecha_recepcion,
                                  int nivel_minimo) {
    // se crea el nuevo nodo con memoria dinamica
    NodoLote* nuevo = new NodoLote;
    nuevo->codigo_lote = codigo_lote;
    nuevo->finca_origen = finca_origen;
    nuevo->cantidad_sacos = cantidad_sacos;
    nuevo->fecha_recepcion = fecha_recepcion;
    nuevo->nivel_minimo = nivel_minimo;
    nuevo->siguiente = nullptr;
    nuevo->anterior = nullptr;

    // si la lista esta vacia, el nodo queda como cabeza y cola al mismo tiempo
    if (estaVacia()) {
        cabeza = nuevo;
        cola = nuevo;
        return;
    }

    // si el codigo va antes que el primero, se inserta al inicio
    if (codigo_lote < cabeza->codigo_lote) {
        nuevo->siguiente = cabeza;
        cabeza->anterior = nuevo;
        cabeza = nuevo;
        return;
    }

    // si el codigo va despues que el ultimo, se inserta al final
    if (codigo_lote >= cola->codigo_lote) {
        nuevo->anterior = cola;
        cola->siguiente = nuevo;
        cola = nuevo;
        return;
    }

    // si no cae en los extremos, se busca la posicion correcta en medio
    NodoLote* actual = cabeza->siguiente;
    while (actual != nullptr && codigo_lote >= actual->codigo_lote) {
        actual = actual->siguiente;
    }

    NodoLote* anterior = actual->anterior;
    nuevo->siguiente = actual;
    nuevo->anterior = anterior;
    anterior->siguiente = nuevo;
    actual->anterior = nuevo;
}

void ListaDoble::mostrar() const {
    // se recorre la lista de inicio a fin para imprimir los datos
    NodoLote* actual = cabeza;
    while (actual != nullptr) {
        cout << "codigo lote: " << actual->codigo_lote << endl;
        cout << "finca origen: " << actual->finca_origen << endl;
        cout << "cantidad de sacos: " << actual->cantidad_sacos << endl;
        cout << "fecha de recepcion: " << actual->fecha_recepcion << endl;
        cout << "nivel minimo: " << actual->nivel_minimo << endl;
        cout << "-------------------------------" << endl;
        actual = actual->siguiente;
    }
}