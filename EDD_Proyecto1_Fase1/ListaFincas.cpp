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

#include <fstream>
#include <cstdlib>

void ListaFincas::generarReporteGrafo() const {
    std::ofstream archivo("reporte_fincas.dot");
    if (!archivo.is_open()) {
        cout << "error al crear el archivo dot de fincas." << endl;
        return;
    }

    archivo << "digraph G {\n";
    archivo << "  rankdir=TB;\n"; // de arriba hacia abajo
    // color verdecito para las fincas
    archivo << "  node [shape=record, style=filled, fillcolor=\"#d5e8d4\"];\n\n";

    NodoFinca* actualFinca = cabeza;
    int f = 0;

    // 1. declaramos todas las fincas en la misma linea horizontal (rank=same)
    archivo << "  { rank=same;\n";
    while (actualFinca != nullptr) {
        archivo << "    Finca" << f << " [label=\"{" << actualFinca->nombre_finca << " | *}\"];\n";
        actualFinca = actualFinca->siguiente;
        f++;
    }
    archivo << "    NullFinca [label=\"NULL\", shape=plaintext, style=solid];\n";
    archivo << "  }\n\n";

    // 2. enlazamos las fincas entre si (de izquierda a derecha)
    actualFinca = cabeza;
    f = 0;
    while (actualFinca != nullptr) {
        if (actualFinca->siguiente != nullptr) {
            archivo << "  Finca" << f << " -> Finca" << (f + 1) << ";\n";
        } else {
            archivo << "  Finca" << f << " -> NullFinca;\n";
        }
        actualFinca = actualFinca->siguiente;
        f++;
    }

    // 3. para cada finca, dibujamos su pila de entregas colgando hacia abajo
    actualFinca = cabeza;
    f = 0;
    while (actualFinca != nullptr) {
        // usamos el metodo getTope() que agregamos en el .h
        NodoEntrega* actualEntrega = actualFinca->historial_entregas->getTope();
        
        archivo << "\n  // pila de " << actualFinca->nombre_finca << "\n";
        archivo << "  TOP" << f << " [label=\"TOP\", shape=plaintext, style=solid];\n";
        
        // flecha punteada desde la finca hacia su TOP
        archivo << "  Finca" << f << " -> TOP" << f << " [style=dashed];\n";

        if (actualEntrega == nullptr) {
            archivo << "  NullPila" << f << " [label=\"NULL\", shape=plaintext, style=solid];\n";
            archivo << "  TOP" << f << " -> NullPila" << f << ";\n";
        } else {
            int e = 0;
            archivo << "  TOP" << f << " -> Entrega" << f << "_0;\n";
            
            while (actualEntrega != nullptr) {
                // color amarillito para los nodos de la pila
                archivo << "  Entrega" << f << "_" << e << " [label=\"{Fecha: " << actualEntrega->fecha 
                        << " | " << actualEntrega->cantidad_sacos << " Qq | *}\", fillcolor=\"#fff2cc\"];\n";
                
                if (actualEntrega->siguiente != nullptr) {
                    archivo << "  Entrega" << f << "_" << e << " -> Entrega" << f << "_" << (e + 1) << ";\n";
                } else {
                    archivo << "  NullPila" << f << " [label=\"NULL\", shape=plaintext, style=solid];\n";
                    archivo << "  Entrega" << f << "_" << e << " -> NullPila" << f << ";\n";
                }
                actualEntrega = actualEntrega->siguiente;
                e++;
            }
        }
        actualFinca = actualFinca->siguiente;
        f++;
    }

    archivo << "}\n";
    archivo.close();

    system("dot -Tpng reporte_fincas.dot -o reporte_fincas.png");
    cout << "reporte de fincas y entregas generado (reporte_fincas.png)." << endl;
}