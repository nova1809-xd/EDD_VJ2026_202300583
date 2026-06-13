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

#include <fstream>
#include <cstdlib>
#include <iostream>

using std::cout;
using std::endl;

void PilaBitacora::generarReporteGrafo() const {
    std::ofstream archivo("reporte_bitacora.dot");
    if (!archivo.is_open()) {
        cout << "error al crear el dot de la bitacora." << endl;
        return;
    }

    archivo << "digraph G {\n";
    archivo << "  rankdir=TB;\n"; // de arriba hacia abajo (Top-Bottom)
    // color amarillito pastel como en la imagen
    archivo << "  node [shape=record, style=filled, fillcolor=\"#fff2cc\"];\n\n"; 

    archivo << "  TOP [shape=plaintext, style=solid];\n";
    archivo << "  Nulo [label=\"NULL\", shape=plaintext, style=solid];\n\n";

    if (estaVacia()) {
        archivo << "  TOP -> Nulo;\n";
    } else {
        NodoBitacora* actual = tope;
        int i = 0;

        // armamos las celdas apiladas
        while (actual != nullptr) {
            archivo << "  nodo" << i << " [label=\"{Accion: " << actual->tipo_accion 
                    << " | Fecha: " << actual->fecha_hora 
                    << " | Detalle: " << actual->detalle_operacion << "}\"];\n";
            actual = actual->siguiente;
            i++;
        }

        // TOP apunta al primer elemento
        archivo << "\n  TOP -> nodo0;\n";

        for (int j = 0; j < i - 1; j++) {
            archivo << "  nodo" << j << " -> nodo" << (j + 1) << ";\n";
        }

        // el ultimo apunta a NULL
        archivo << "  nodo" << (i - 1) << " -> Nulo;\n";
    }

    archivo << "}\n";
    archivo.close();

    system("dot -Tpng reporte_bitacora.dot -o reporte_bitacora.png");
    cout << "reporte de bitacora generado (reporte_bitacora.png)." << endl;
}