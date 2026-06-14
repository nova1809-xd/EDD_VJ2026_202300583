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

void ListaDoble::generarReporteGrafo() const {
    std::ofstream archivo("reporte_inventario.dot");
    
    if (!archivo.is_open()) {
        cout << "error: no se pudo crear el archivo dot para el reporte." << endl;
        return;
    }

    // se inicia la estructura del grafo en lenguaje dot
    archivo << "digraph G {\n";
    archivo << "  rankdir=LR;\n"; // de izquierda a derecha
    archivo << "  node [shape=record, fontname=\"Arial\"];\n\n";

    NodoLote* actual = cabeza;
    int i = 0;

    // primer ciclo: crear todos los nodos visuales
    while (actual != nullptr) {
        // codigo de colores: rojo si hay menos sacos que el minimo, verde si esta normal
        string color = (actual->cantidad_sacos < actual->nivel_minimo) ? "red" : "green";
        
        archivo << "  nodo" << i << " [label=\"{Codigo: " << actual->codigo_lote 
                << " | Finca: " << actual->finca_origen 
                << " | Sacos: " << actual->cantidad_sacos 
                << " | Fecha: " << actual->fecha_recepcion << "}\", color=\"" << color << "\", style=filled, fillcolor=\"#e8f4f8\"];\n";
        
        actual = actual->siguiente;
        i++;
    }

    archivo << "\n  // enlaces bidireccionales\n";
    
    // segundo ciclo: crear las flechas entre los nodos
    actual = cabeza;
    i = 0;
    while (actual != nullptr && actual->siguiente != nullptr) {
        archivo << "  nodo" << i << " -> nodo" << (i + 1) << " [dir=both];\n";
        actual = actual->siguiente;
        i++;
    }

    // agregar los punteros de los extremos (Frente y Fin)
    if (cabeza != nullptr) {
        archivo << "\n  // punteros de inicio y fin\n";
        archivo << "  PunteroFrente [label=\"Frente\", shape=plaintext];\n";
        archivo << "  PunteroFin [label=\"Fin\", shape=plaintext];\n";
        archivo << "  PunteroFrente -> nodo0;\n";
        archivo << "  PunteroFin -> nodo" << i << ";\n";
    }

    archivo << "}\n";
    archivo.close();

    // ejecutar el comando en la terminal para generar el PNG
    system("dot -Tpng reporte_inventario.dot -o reporte_inventario.png");
    cout << "reporte de inventario generado exitosamente (reporte_inventario.png)." << endl;
}

bool ListaDoble::buscarYObtener(const string& codigo_buscar, string& finca, int& sacos, string& fecha) const {
    NodoLote* actual = cabeza;
    while (actual != nullptr) {
        if (actual->codigo_lote == codigo_buscar) {
            finca = actual->finca_origen;
            sacos = actual->cantidad_sacos;
            fecha = actual->fecha_recepcion;
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}