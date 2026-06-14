#include "ListaDoble.h"
#include "ColaDespulpado.h"
#include "PilaBitacora.h"
#include "ListaFincas.h"
#include "GestorArchivos.h"
#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;

// declaracion de los submenus para no saturar el main
void menuAdministrador(ListaDoble& inventario, ColaDespulpado& cola, PilaBitacora& bitacora, ListaFincas& fincas);
void menuOperario(ListaDoble& inventario, ColaDespulpado& cola);

int main() {
    // instanciamos todas nuestras estructuras maestras
    ListaDoble inventario;
    ColaDespulpado cola;
    PilaBitacora bitacora;
    ListaFincas fincas;

    int opcion;
    do {
        cout << "\n=== EDD COFFEE TRACK ===" << endl;
        cout << "1. Iniciar Sesion como Administrador (Jefe de Beneficio)" << endl;
        cout << "2. Iniciar Sesion como Operario" << endl;
        cout << "3. Salir del Sistema" << endl;
        cout << "Elige una opcion: ";
        cin >> opcion;

        // validacion simple para que no se cicle si metes una letra por error
        if(cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = -1;
        }

        switch(opcion) {
            case 1:
                menuAdministrador(inventario, cola, bitacora, fincas);
                break;
            case 2:
                // nota: el login del operario va adentro de esta funcion
                menuOperario(inventario, cola);
                break;
            case 3:
                cout << "saliendo del sistema... ¡nos vemos!" << endl;
                break;
            default:
                cout << "opcion invalida. intenta de nuevo." << endl;
        }
    } while (opcion != 3);

    return 0;
}

void menuAdministrador(ListaDoble& inventario, ColaDespulpado& cola, PilaBitacora& bitacora, ListaFincas& fincas) {
    int opcion;
    do {
        cout << "\n--- MENU JEFE DE BENEFICIO ---" << endl;
        cout << "1. Registrar nuevo lote manualmente" << endl;
        cout << "2. Carga Masiva de Lotes (CSV)" << endl;
        cout << "3. Gestionar Fincas Productoras" << endl;
        cout << "4. Registrar Entrega de Sacos por Finca" << endl;
        cout << "5. Procesar Lotes en Cola de Despulpado" << endl;
        cout << "6. Visualizar Inventario Completo" << endl;
        cout << "7. Consultar Historial de Transacciones (Bitacora)" << endl;
        cout << "8. Generar Reportes Graphviz" << endl;
        cout << "9. Cerrar Sesion" << endl;
        cout << "Elige una opcion: ";
        cin >> opcion;

        if(cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); opcion = -1; }

        switch(opcion) {
           case 1: {
                string cod, finca, fecha;
                int sacos, minimo;
                cout << "\n--- NUEVO LOTE ---" << endl;
                cout << "codigo de lote (ej. LOT-100): "; cin >> cod;
                cout << "finca origen: "; cin.ignore(); std::getline(cin, finca);
                cout << "cantidad de sacos: "; cin >> sacos;
                cout << "fecha (yyyy-mm-dd): "; cin >> fecha;
                cout << "nivel minimo: "; cin >> minimo;
                
                inventario.insertarOrdenado(cod, finca, sacos, fecha, minimo);
                // guardamos la accion en la bitacora
                bitacora.push("2026-06-13", "Registro Lote", "se registro manualmente el " + cod);
                cout << "lote registrado con exito y agregado al inventario." << endl;
                break;
            }
            case 2: {
                string archivo;
                cout << "\n--- CARGA MASIVA ---" << endl;
                cout << "ingresa el nombre del archivo (ej. datos.csv): ";
                cin >> archivo;
                
                cargarLotesCSV(archivo, inventario);
                bitacora.push("2026-06-13", "Carga Masiva", "se cargaron lotes desde el archivo " + archivo);
                break;
            }
            case 3: {
                string nombre_finca;
                cout << "\n--- NUEVA FINCA ---" << endl;
                cout << "nombre de la nueva finca: "; cin.ignore(); std::getline(cin, nombre_finca);
                
                fincas.registrarFinca(nombre_finca);
                bitacora.push("2026-06-13", "Registro Finca", "se creo la finca " + nombre_finca);
                cout << "finca registrada con exito en el sistema." << endl;
                break;
            }
            case 4: {
                string nombre_finca, fecha;
                int sacos;
                cout << "\n--- REGISTRO DE ENTREGA ---" << endl;
                cout << "nombre de la finca: "; cin.ignore(); std::getline(cin, nombre_finca);
                cout << "fecha de entrega (yyyy-mm-dd): "; cin >> fecha;
                cout << "cantidad de sacos: "; cin >> sacos;
                
                fincas.registrarEntregaFinca(nombre_finca, fecha, sacos);
                bitacora.push("2026-06-13", "Registro Entrega", "entrega de " + std::to_string(sacos) + " sacos de " + nombre_finca);
                cout << "entrega agregada al historial de la finca." << endl;
                break;
            }
            case 5: {
                cout << "\n--- PROCESANDO COLA DE DESPULPADO ---" << endl;
                string lote, finca, fecha;
                int sacos;
                
                if (cola.dequeue(lote, finca, sacos, fecha)) {
                    cout << "procesando lote: " << lote << " de " << finca << " (" << sacos << " sacos)" << endl;
                    bitacora.push("2026-06-13", "Procesamiento", "se despulpo el lote " + lote + " de " + finca);
                } else {
                    cout << "la cola de despulpado esta vacia. no hay lotes pendientes." << endl;
                }
                break;
            }
            case 6: 
                cout << "\n-- INVENTARIO EN BODEGA --" << endl;
                inventario.mostrar(); 
                break;
            case 7: 
                cout << "\n-- HISTORIAL DE TRANSACCIONES --" << endl;
                bitacora.mostrarHistorial(); 
                break;
            case 8:
                cout << "generando reportes..." << endl;
                inventario.generarReporteGrafo();
                cola.generarReporteGrafo();
                bitacora.generarReporteGrafo();
                fincas.generarReporteGrafo();
                break;
            case 9: cout << "cerrando sesion de administrador..." << endl; break;
            default: cout << "opcion invalida." << endl;
        }
    } while (opcion != 9);
}

void menuOperario(ListaDoble& inventario, ColaDespulpado& cola) {
    // el enunciado pide inicio de sesion con codigo y password
    string usuario, password;
    cout << "\n--- LOGIN OPERARIO ---" << endl;
    cout << "Usuario: ";
    cin >> usuario;
    cout << "Password: ";
    cin >> password;

    // hardcodeamos un usuario generico para que nos deje entrar rapido
    if (usuario != "operario1" || password != "1234") {
        cout << "credenciales incorrectas. acceso denegado." << endl;
        return;
    }

    int opcion;
    do {
        cout << "\n--- MENU OPERARIO ---" << endl;
        cout << "1. Consultar Disponibilidad de Lote" << endl;
        cout << "2. Enviar Lote a Cola de Procesamiento" << endl;
        cout << "3. Visualizar Historial de Lotes Procesados" << endl;
        cout << "4. Cerrar Sesion" << endl;
        cout << "Elige una opcion: ";
        cin >> opcion;

        if(cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); opcion = -1; }

        switch(opcion) {
            case 1: {
                string cod_buscar, finca_res, fecha_res;
                int sacos_res;
                cout << "\n--- CONSULTAR LOTE ---" << endl;
                cout << "ingresa el codigo del lote (ej. LOT-001): ";
                cin >> cod_buscar;
                
                if (inventario.buscarYObtener(cod_buscar, finca_res, sacos_res, fecha_res)) {
                    cout << "Lote encontrado:" << endl;
                    cout << "- Finca: " << finca_res << endl;
                    cout << "- Sacos disponibles: " << sacos_res << endl;
                    cout << "- Fecha de recepcion: " << fecha_res << endl;
                } else {
                    cout << "error: el lote " << cod_buscar << " no se encontro en bodega." << endl;
                }
                break;
            }
            case 2: {
                string cod_enviar, finca_res, fecha_res;
                int sacos_res;
                cout << "\n--- ENVIAR A COLA DE DESPULPADO ---" << endl;
                cout << "ingresa el codigo del lote a procesar: ";
                cin >> cod_enviar;
                
                // primero verificamos si el lote existe en bodega
                if (inventario.buscarYObtener(cod_enviar, finca_res, sacos_res, fecha_res)) {
                    // si existe, lo encolamos
                    cola.enqueue(cod_enviar, finca_res, sacos_res, "2026-06-13");
                    cout << "el lote " << cod_enviar << " ha sido agregado a la cola de despulpado." << endl;
                } else {
                    cout << "error: no se puede procesar un lote que no esta en inventario." << endl;
                }
                break;
            }
            case 3: 
                // para el historial de procesados, usamos la misma funcion de mostrar inventario 
                // ya que en esta fase 1 la lista doble actua como el registro general
                cout << "\n--- HISTORIAL DE LOTES EN BODEGA ---" << endl;
                inventario.mostrar(); 
                break;
            case 4: cout << "cerrando sesion de operario..." << endl; break;
            default: cout << "opcion invalida." << endl;
        }
    } while (opcion != 4);
}