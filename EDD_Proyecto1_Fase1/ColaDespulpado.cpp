#include "ColaDespulpado.h"

ColaDespulpado::ColaDespulpado() {
    frente = nullptr;
    final = nullptr;
}

ColaDespulpado::~ColaDespulpado() {
    // se recorre la cola y se libera cada nodo para no dejar memoria ocupada
    NodoCola* actual = frente;
    while (actual != nullptr) {
        NodoCola* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }

    frente = nullptr;
    final = nullptr;
}

bool ColaDespulpado::estaVacia() const {
    return frente == nullptr;
}

void ColaDespulpado::enqueue(const string& numero_lote,
                             const string& finca_solicitante,
                             int cantidad_sacos,
                             const string& fecha_ingreso) {
    // se crea un nodo nuevo con memoria dinamica para meterlo al final
    NodoCola* nuevo = new NodoCola;
    nuevo->numero_lote = numero_lote;
    nuevo->finca_solicitante = finca_solicitante;
    nuevo->cantidad_sacos = cantidad_sacos;
    nuevo->fecha_ingreso = fecha_ingreso;
    nuevo->siguiente = nullptr;

    // si la cola esta vacia, el nuevo nodo queda como frente y final
    if (estaVacia()) {
        frente = nuevo;
        final = nuevo;
        return;
    }

    // si ya hay datos, se enlaza el ultimo nodo con el nuevo
    final->siguiente = nuevo;
    final = nuevo;
}

bool ColaDespulpado::dequeue(string& numero_lote,
                             string& finca_solicitante,
                             int& cantidad_sacos,
                             string& fecha_ingreso) {
    // si la cola esta vacia no hay nada que sacar
    if (estaVacia()) {
        return false;
    }

    // se toma el nodo del frente y se guardan sus datos antes de borrarlo
    NodoCola* nodo_salida = frente;
    numero_lote = nodo_salida->numero_lote;
    finca_solicitante = nodo_salida->finca_solicitante;
    cantidad_sacos = nodo_salida->cantidad_sacos;
    fecha_ingreso = nodo_salida->fecha_ingreso;

    // se mueve el frente al siguiente nodo
    frente = frente->siguiente;

    // si ya no quedan nodos, el final tambien se deja en null
    if (frente == nullptr) {
        final = nullptr;
    }

    delete nodo_salida;
    return true;
}