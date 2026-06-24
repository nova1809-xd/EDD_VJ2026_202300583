#include <iostream>
#include <string>
#include "ArbolB.h"
#include "GestorJSON.h"
#include "ArbolMerkle.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

// estructura manual para guardar las fincas y validar duplicados (opcion 3.1)
struct FincaManual {
    string codigo;
    string nombre;
    string region;
    string propietario;
    FincaManual* siguiente;

    FincaManual(string c, string n, string r, string p) {
        codigo = c; nombre = n; region = r; propietario = p;
        siguiente = nullptr;
    }
};

struct ListaFincas {
    FincaManual* cabeza = nullptr;

    bool existe(string codigo) {
        FincaManual* actual = cabeza;
        while (actual != nullptr) {
            if (actual->codigo == codigo) return true;
            actual = actual->siguiente;
        }
        return false;
    }

    void insertar(string c, string n, string r, string p) {
        FincaManual* nueva = new FincaManual(c, n, r, p);
        if (cabeza == nullptr) {
            cabeza = nueva;
        } else {
            FincaManual* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nueva;
        }
    }
};

void menuPrincipalFase2() {
    int opcion;
    
    ArbolB arbol_fechas; 
    GestorJSON lector;
    ArbolMerkle arbol_merkle;
    ListaFincas lista_fincas; // instancia para manejar las fincas

    do {
        cout << "\n=== EDD COFFEETRACK FASE 2 ===" << endl;
        cout << "--- ADMINISTRADOR ---" << endl;
        cout << "1. Carga masiva desde JSON" << endl;
        cout << "2. Consultas y Trazabilidad" << endl;
        cout << "3. Rutas" << endl;
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
                lector.cargarDesdeArchivo(archivo, arbol_fechas);
                break;
            }
            case 2: {
                string lote_buscar, fecha_encontrada;
                cout << "\n--- CONSULTAS Y TRAZABILIDAD ---" << endl;
                cout << "ingresa el codigo de lote a consultar: ";
                cin >> lote_buscar;

                NodoAVL* lote = arbol_fechas.buscarLoteGlobal(lote_buscar, fecha_encontrada);
                if (lote != nullptr) {
                    cout << "\n--- DETALLES DEL LOTE ---" << endl;
                    cout << "Codigo: " << lote->codigo_lote << endl;
                    cout << "Finca: " << lote->codigo_finca << " (" << lote->nombre_finca << ")" << endl;
                    cout << "Fecha recepcion: " << fecha_encontrada << endl;
                    cout << "Estado actual: " << lote->estado << endl;
                    
                    int sub_opcion;
                    cout << "\n1. Avanzar estado del lote" << endl;
                    cout << "2. Ver historial de trazabilidad" << endl;
                    cout << "Elige una opcion: ";
                    cin >> sub_opcion;

                    if (sub_opcion == 1) {
                        string nuevo_estado;
                        cout << "ingresa el nuevo estado (ej. tostado, empacado): ";
                        cin >> nuevo_estado;
                        
                        lote->estado = nuevo_estado;
                        lote->agregarAlHistorial(nuevo_estado);
                        cout << "estado actualizado y guardado en el historial." << endl;
                    } else if (sub_opcion == 2) {
                        cout << "\n--- HISTORIAL DE TRAZABILIDAD ---" << endl;
                        HistorialEstado* actual = lote->cabeza_historial;
                        while(actual != nullptr) {
                            cout << "[" << actual->timestamp << "] -> " << actual->estado << endl;
                            actual = actual->siguiente;
                        }
                    } else {
                        cout << "opcion invalida." << endl;
                    }
                } else {
                    cout << "error: lote no encontrado en el sistema." << endl;
                }
                break;
            }
            case 3: {
                int sub_opcion;
                cout << "\n--- RUTAS Y FINCAS ---" << endl;
                cout << "1. Registrar nueva finca y sus conexiones (3.1)" << endl;
                cout << "Elige una opcion: ";
                cin >> sub_opcion;

                if (sub_opcion == 1) {
                    string cod, nom, reg, prop;
                    cout << "\n--- REGISTRO DE FINCA ---" << endl;
                    cout << "Codigo de finca: ";
                    cin >> cod;

                    if (lista_fincas.existe(cod)) {
                        cout << "[error] el codigo de finca ya existe en el sistema. no se registrara el duplicado." << endl;
                    } else {
                        cin.ignore(); // limpiar el buffer
                        cout << "Nombre: "; getline(cin, nom);
                        cout << "Region: "; getline(cin, reg);
                        cout << "Propietario: "; getline(cin, prop);
                        
                        lista_fincas.insertar(cod, nom, reg, prop);
                        cout << "[exito] finca registrada." << endl;

                        char agregar_conn;
                        do {
                            cout << "\n¿Agregar conexion? (s/n): ";
                            cin >> agregar_conn;
                            if (tolower(agregar_conn) == 's') {
                                string destino;
                                float dist;
                                cout << "Destino (codigo de finca o BENEFICIO): "; 
                                cin >> destino;
                                cout << "Distancia en km: "; 
                                cin >> dist;
                                cout << "conexion registrada en memoria." << endl;
                            }
                        } while (tolower(agregar_conn) == 's');
                    }
                } else {
                    cout << "opcion invalida." << endl;
                }
                break;
            }
            case 4: {
                int sub_opcion;
                cout << "\n--- MENU DE CERTIFICADOS ---" << endl;
                cout << "1. Generar certificado de un lote especifico (4.1)" << endl;
                cout << "2. Generar certificados masivos por fecha (4.2)" << endl;
                cout << "Elige una opcion: ";
                cin >> sub_opcion;

                if (sub_opcion == 1) {
                    string lote_buscar, fecha_encontrada;
                    cout << "ingresa el codigo de lote: ";
                    cin >> lote_buscar;

                    NodoAVL* lote = arbol_fechas.buscarLoteGlobal(lote_buscar, fecha_encontrada);
                    if (lote != nullptr) {
                        if (lote->hash_certificado == "") {
                            string hash_gen = arbol_merkle.generarCertificadoFisico(fecha_encontrada, lote->codigo_lote, lote->codigo_finca, lote->nombre_finca, lote->sacos, lote->tipo_cafe);
                            lote->hash_certificado = hash_gen;
                            lote->estado = "certificado_emitido";
                            lote->agregarAlHistorial("certificado_emitido");
                            
                            arbol_merkle.construirArbol();
                            cout << "\ncertificado generado y arbol de merkle actualizado." << endl;
                        } else {
                            cout << "este lote ya tiene un certificado emitido." << endl;
                        }
                    } else {
                        cout << "error: lote no encontrado." << endl;
                    }
                } else if (sub_opcion == 2) {
                    string fecha_cert;
                    cout << "ingresa la fecha (ej. 2026-06-15): ";
                    cin >> fecha_cert;

                    ArbolAVL* avl = arbol_fechas.buscarFecha(fecha_cert);
                    if (avl != nullptr) {
                        avl->generarCertificadosLotes(arbol_merkle, fecha_cert);
                        
                        arbol_merkle.construirArbol();
                        cout << "\ncertificados masivos generados para la fecha " << fecha_cert << " y arbol reconstruido." << endl;
                    } else {
                        cout << "no hay lotes registrados en esa fecha." << endl;
                    }
                } else {
                    cout << "opcion invalida." << endl;
                }
                break;
            }
            case 5:
                cout << "\n--- CONSTRUYENDO ARBOL DE MERKLE ---" << endl;
                arbol_merkle.construirArbol();
                arbol_merkle.mostrarRootHash();
                break;
            case 6: {
                int sub_opcion;
                cout << "\n--- REPORTES GRAPHVIZ ---" << endl;
                cout << "1. Generar reporte del Arbol B (Fechas)" << endl;
                cout << "2. Generar reporte de un Arbol AVL (Lotes por fecha)" << endl;
                cout << "Elige una opcion: ";
                cin >> sub_opcion;

                if (sub_opcion == 1) {
                    arbol_fechas.generarReporteB();
                } else if (sub_opcion == 2) {
                    string fecha_rep;
                    cout << "ingresa la fecha del arbol AVL a graficar (ej. 2026-06-15): ";
                    cin >> fecha_rep;
                    
                    ArbolAVL* avl = arbol_fechas.buscarFecha(fecha_rep);
                    if (avl != nullptr) {
                        avl->generarReporte(fecha_rep);
                    } else {
                        cout << "no hay registros para esa fecha." << endl;
                    }
                } else {
                    cout << "opcion invalida." << endl;
                }
                break;
            }
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