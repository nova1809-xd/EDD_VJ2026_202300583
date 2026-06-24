#include <iostream>
#include <string>
#include "ArbolB.h"
#include "GestorJSON.h" 

using std::cout;
using std::cin;
using std::endl;
using std::string;

void menuPrincipalFase2() {
    int opcion;
    
    // instanciamos el arbol b general y el lector json
    ArbolB arbol_fechas; 
    GestorJSON lector;

    do {
        cout << "\n=== EDD COFFEETRACK FASE 2 ===" << endl;
        cout << "--- ADMINISTRADOR ---" << endl;
        cout << "1. Carga masiva desde JSON" << endl;
        cout << "2. Consultas y Trazabilidad" << endl;
        cout << "3. Rutas (Descartado)" << endl;
        cout << "4. Certificados" << endl;
        cout << "5. Arbol de Merkle" << endl;
        cout << "6. Reportes Graphviz" << endl;
        cout << "7. [TEST] Mostrar Arbol B y AVL" << endl;
        cout << "0. Salir" << endl;
        cout << "Elige una opcion: ";
        cin >> opcion;

        if(cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = -1;
        }

        switch(opcion) {
            case 1: {
                string archivo;
                cout << "\n--- CARGA MASIVA JSON ---" << endl;
                cout << "ingresa el nombre del archivo (ej. datos.json): ";
                cin >> archivo;
                // llamamos a la funcion maestra del lector
                lector.cargarDesdeArchivo(archivo, arbol_fechas);
                break;
            }
            case 2: cout << "proximamente: consultas" << endl; break;
            case 3: cout << "proximamente: rutas" << endl; break;
            case 4: cout << "proximamente: certificados" << endl; break;
            case 5: cout << "proximamente: arbol de merkle" << endl; break;
            case 6: cout << "proximamente: reportes" << endl; break;
            case 7: {
                cout << "\n--- fechas registradas en el arbol b ---" << endl;
                arbol_fechas.mostrarFechas();

                string fecha_buscar;
                cout << "\ningresa una fecha para ver sus lotes (ej. 2026-06-15): ";
                cin >> fecha_buscar;

                ArbolAVL* avl = arbol_fechas.buscarFecha(fecha_buscar);
                if(avl != nullptr) {
                    cout << "lotes encontrados:" << endl;
                    avl->mostrarInOrder();
                } else {
                    cout << "no hay lotes para esa fecha." << endl;
                }
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