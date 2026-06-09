#include "ListaDoble.h"

int main() {
    ListaDoble inventario;

    // datos de ejemplo para probar el orden alfabetico por codigo de lote
    inventario.insertarOrdenado("L-030", "Finca la esperanza", 18, "2026-05-10", 5);
    inventario.insertarOrdenado("L-010", "Finca santa rosa", 25, "2026-05-08", 8);
    inventario.insertarOrdenado("L-020", "Finca el nogal", 14, "2026-05-09", 6);

    inventario.mostrar();

    return 0;
}