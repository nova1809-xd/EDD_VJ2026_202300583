#include "PilaBitacora.h"
#include <iostream>

using std::cout;
using std::endl;

PilaBitacora::PilaBitacora() {
    tope = nullptr;
}

PilaBitacora::~PilaBitacora() {
    // se limpia la memoria recorriendo desde el tope hacia abajo
    NodoBitacora* actual = tope;
    while (actual != nullptr) {
        NodoBitacora* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    tope = nullptr;
}

bool PilaBitacora::estaVacia() const {
    return tope == nullptr;
}

void PilaBitacora::push(const string& fecha_hora, const string& tipo_accion, const string& detalle_operacion) {
    // se crea el nodo con memoria dinamica y se coloca encima (en el tope)
    NodoBitacora* nuevo = new NodoBitacora;
    nuevo->fecha_hora = fecha_hora;
    nuevo->tipo_accion = tipo_accion;
    nuevo->detalle_operacion = detalle_operacion;
    
    nuevo->siguiente = tope;
    tope = nuevo;
}

void PilaBitacora::mostrarHistorial() const {
    // se recorre la pila de arriba hacia abajo como debe ser en lifo
    NodoBitacora* actual = tope;
    while (actual != nullptr) {
        cout << "fecha y hora: " << actual->fecha_hora << endl;
        cout << "accion: " << actual->tipo_accion << endl;
        cout << "detalle: " << actual->detalle_operacion << endl;
        cout << "-------------------------------" << endl;
        actual = actual->siguiente;
    }
}