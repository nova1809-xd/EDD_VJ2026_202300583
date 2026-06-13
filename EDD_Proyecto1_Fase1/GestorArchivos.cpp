#include "GestorArchivos.h"
#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;

void cargarLotesCSV(const std::string& ruta_archivo, ListaDoble& inventario) {
    ifstream archivo(ruta_archivo);
    
    // validacion por si el archivo no existe o la ruta esta mal
    if (!archivo.is_open()) {
        cout << "error: no se pudo abrir el archivo " << ruta_archivo << endl;
        return;
    }

    std::string linea;
    // se lee y se salta la primera linea porque trae los encabezados del csv
    std::getline(archivo, linea);

    // se recorre linea por linea el resto del archivo
    while (std::getline(archivo, linea)) {
        stringstream stream(linea);
        std::string codigo, finca, variedad, municipio, depto, sacos_str, peso_str, fecha, minimo_str;

        // se separan los campos por comas segun el formato del enunciado
        std::getline(stream, codigo, ',');
        std::getline(stream, finca, ',');
        std::getline(stream, variedad, ',');
        std::getline(stream, municipio, ',');
        std::getline(stream, depto, ',');
        std::getline(stream, sacos_str, ',');
        std::getline(stream, peso_str, ',');
        std::getline(stream, fecha, ',');
        std::getline(stream, minimo_str, ',');

        // se convierten los strings a enteros para los sacos y el nivel minimo
        int sacos = 0, minimo = 0;
        try {
            sacos = std::stoi(sacos_str);
            minimo = std::stoi(minimo_str);
        } catch (...) {
            // si hay alguna fila corrupta o vacia, se ignora y se pasa a la siguiente
            continue;
        }

        // se inserta el lote directamente a la lista doble para que quede ordenado
        inventario.insertarOrdenado(codigo, finca, sacos, fecha, minimo);
    }
    
    archivo.close();
    cout << "carga masiva completada con exito. lotes ingresados al inventario." << endl;
}