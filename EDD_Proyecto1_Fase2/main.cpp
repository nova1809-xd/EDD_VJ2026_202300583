#include <iostream>
#include "ArbolAVL.h"

using std::cout;
using std::cin;
using std::endl;

// el nuevo menu principal de la fase 2
void menuPrincipalFase2() {
    int opcion;
    do {
        cout << "\n=== EDD COFFEETRACK FASE 2 ===" << endl;
        cout << "--- ADMINISTRADOR ---" << endl;
        cout << "1. Gestion de Datos" << endl;
        cout << "2. Consultas y Trazabilidad" << endl;
        cout << "3. Rutas (Descartado)" << endl;
        cout << "4. Certificados" << endl;
        cout << "5. Arbol de Merkle" << endl;
        cout << "6. Reportes Graphviz" << endl;
        cout << "7. [TEST] Probar Arbol AVL" << endl;
        cout << "0. Salir" << endl;
        cout << "Elige una opcion: ";
        cin >> opcion;

        if(cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = -1;
        }

        switch(opcion) {
            case 1: cout << "proximamente: carga de JSON" << endl; break;
            case 2: cout << "proximamente: consultas" << endl; break;
            case 3: cout << "proximamente: rutas" << endl; break;
            case 4: cout << "proximamente: certificados" << endl; break;
            case 5: cout << "proximamente: arbol de merkle" << endl; break;
            case 6: cout << "proximamente: reportes" << endl; break;
            case 7: {
                // --- inicio de la prueba del avl ---
                ArbolAVL arbol_prueba;
                cout << "\n--- insertando lotes en el arbol avl ---" << endl;
                
                arbol_prueba.insertar("LOT-001", "F001", "finca la hermosa", 45, "bourbon");
                arbol_prueba.insertar("LOT-002", "F002", "finca el roble", 30, "caturra");
                arbol_prueba.insertar("LOT-003", "F003", "finca las nubes", 55, "gesha");

                cout << "lotes insertados correctamente.\n" << endl;
                cout << "--- recorrido in-order ---" << endl;
                arbol_prueba.mostrarInOrder();
                // --- fin de la prueba ---
                break;
            }
            case 0: cout << "saliendo del sistema..." << endl; break;
            default: cout << "opcion invalida." << endl;
        }
    } while (opcion != 0);
}

int main() {
    menuPrincipalFase2();
    return 0;
}