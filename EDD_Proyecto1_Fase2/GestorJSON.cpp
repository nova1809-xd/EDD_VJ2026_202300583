#include "GestorJSON.h"

using std::cout;
using std::endl;
using std::ifstream;

GestorJSON::GestorJSON() {
    cabeza_fincas = nullptr;
}

GestorJSON::~GestorJSON() {
    // limpieza basica de la lista de fincas
    NodoFincaTemp* actual = cabeza_fincas;
    while (actual != nullptr) {
        NodoFincaTemp* aux = actual;
        actual = actual->siguiente;
        delete aux;
    }
}

// agrega una finca a nuestra lista de validacion
void GestorJSON::registrarFincaValida(string codigo, string nombre) {
    NodoFincaTemp* nuevo = new NodoFincaTemp(codigo, nombre);
    if (cabeza_fincas == nullptr) {
        cabeza_fincas = nuevo;
    } else {
        NodoFincaTemp* actual = cabeza_fincas;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
}

// verifica si la finca que viene en el lote realmente existe
bool GestorJSON::existeFinca(string codigo) {
    NodoFincaTemp* actual = cabeza_fincas;
    while (actual != nullptr) {
        if (actual->codigo == codigo) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

// extrae texto entre comillas despues de una clave, ej: "codigo": "F001"
string GestorJSON::extraerEntreComillas(string linea, string clave) {
    size_t pos_clave = linea.find("\"" + clave + "\"");
    if (pos_clave == string::npos) return "";

    size_t pos_dos_puntos = linea.find(":", pos_clave);
    size_t primer_comilla = linea.find("\"", pos_dos_puntos);
    size_t segunda_comilla = linea.find("\"", primer_comilla + 1);

    if (primer_comilla != string::npos && segunda_comilla != string::npos) {
        return linea.substr(primer_comilla + 1, segunda_comilla - primer_comilla - 1);
    }
    return "";
}

// extrae un numero crudo despues de una clave, ej: "sacos": 45
int GestorJSON::extraerNumero(string linea, string clave) {
    size_t pos_clave = linea.find("\"" + clave + "\"");
    if (pos_clave == string::npos) return 0;

    size_t pos_dos_puntos = linea.find(":", pos_clave);
    
    // buscamos donde empieza el numero (saltando espacios)
    size_t inicio_num = pos_dos_puntos + 1;
    while (inicio_num < linea.length() && (linea[inicio_num] == ' ' || linea[inicio_num] == '\t')) {
        inicio_num++;
    }

    // buscamos donde termina (por una coma o espacio)
    size_t fin_num = inicio_num;
    while (fin_num < linea.length() && isdigit(linea[fin_num])) {
        fin_num++;
    }

    if (inicio_num != fin_num) {
        return std::stoi(linea.substr(inicio_num, fin_num - inicio_num));
    }
    return 0;
}

// el lector principal
// el lector principal (version a prueba de json multilinea)
void GestorJSON::cargarDesdeArchivo(string ruta, ArbolB& arbol_fechas) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "error: no se pudo abrir el archivo " << ruta << endl;
        return;
    }

    string linea;
    bool leyendo_fincas = false;
    bool leyendo_entregas = false;

    // variables temporales para acumular datos mientras leemos el bloque
    string tmp_codigo_finca = "";
    string tmp_nombre_finca = "";

    string tmp_fecha = "";
    string tmp_codigo_lote = "";
    string tmp_finca_lote = "";
    string tmp_tipo_cafe = "";
    int tmp_sacos = 0;

    cout << "--- procesando json ---" << endl;

    while (std::getline(archivo, linea)) {
        // detectamos en que seccion estamos
        if (linea.find("\"fincas\"") != string::npos) {
            leyendo_fincas = true;
            leyendo_entregas = false;
        } else if (linea.find("\"entregas\"") != string::npos) {
            leyendo_fincas = false;
            leyendo_entregas = true;
        }

        // --- logica para fincas ---
        if (leyendo_fincas) {
            if (linea.find("\"codigo\"") != string::npos) tmp_codigo_finca = extraerEntreComillas(linea, "codigo");
            if (linea.find("\"nombre\"") != string::npos) tmp_nombre_finca = extraerEntreComillas(linea, "nombre");
            
            // si ya tenemos ambos datos, guardamos la finca y reseteamos
            if (tmp_codigo_finca != "" && tmp_nombre_finca != "") {
                if (!existeFinca(tmp_codigo_finca)) {
                    registrarFincaValida(tmp_codigo_finca, tmp_nombre_finca);
                    cout << "[finca leida] " << tmp_codigo_finca << " - " << tmp_nombre_finca << endl;
                }
                tmp_codigo_finca = "";
                tmp_nombre_finca = "";
            }
        }

        // --- logica para entregas ---
        if (leyendo_entregas) {
            if (linea.find("\"fecha\"") != string::npos) tmp_fecha = extraerEntreComillas(linea, "fecha");
            if (linea.find("\"codigo_lote\"") != string::npos) tmp_codigo_lote = extraerEntreComillas(linea, "codigo_lote");
            if (linea.find("\"finca\"") != string::npos) tmp_finca_lote = extraerEntreComillas(linea, "finca");
            if (linea.find("\"tipo_cafe\"") != string::npos) tmp_tipo_cafe = extraerEntreComillas(linea, "tipo_cafe");
            if (linea.find("\"sacos\"") != string::npos) tmp_sacos = extraerNumero(linea, "sacos");

            // cuando detectamos el cierre del objeto, procesamos el lote
            if (linea.find("}") != string::npos && tmp_codigo_lote != "" && tmp_finca_lote != "") {
                string fecha_corta = "";
                if (tmp_fecha.length() >= 10) {
                    fecha_corta = tmp_fecha.substr(0, 10);
                }

                // validacion de integridad referencial
                if (!existeFinca(tmp_finca_lote)) {
                    cout << "[error] la finca " << tmp_finca_lote << " no existe. ignorando el lote " << tmp_codigo_lote << "." << endl;
                } else {
                    arbol_fechas.insertarLote(fecha_corta, tmp_codigo_lote, tmp_finca_lote, "finca registrada", tmp_sacos, tmp_tipo_cafe);
                    cout << "[lote insertado] lote " << tmp_codigo_lote << " en la fecha " << fecha_corta << endl;
                }

                // limpiamos las variables para el siguiente lote
                tmp_fecha = "";
                tmp_codigo_lote = "";
                tmp_finca_lote = "";
                tmp_tipo_cafe = "";
                tmp_sacos = 0;
            }
        }
    }

    archivo.close();
    cout << "--- carga finalizada ---" << endl;
}