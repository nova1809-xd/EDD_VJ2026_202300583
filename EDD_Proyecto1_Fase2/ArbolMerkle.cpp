#include "ArbolMerkle.h"

using std::cout;
using std::endl;
using std::ofstream;

ArbolMerkle::ArbolMerkle() {
    raiz = nullptr;
    cabeza_hashes = nullptr;
    cantidad_bloques = 0;
}

ArbolMerkle::~ArbolMerkle() {
    limpiarArbol(raiz);
    
    NodoHashList* actual = cabeza_hashes;
    while (actual != nullptr) {
        NodoHashList* aux = actual;
        actual = actual->siguiente;
        delete aux;
    }
}

void ArbolMerkle::limpiarArbol(NodoMerkle* nodo) {
    if (nodo != nullptr) {
        limpiarArbol(nodo->izquierdo);
        limpiarArbol(nodo->derecho);
        delete nodo;
    }
}

// funcion hash casera para evitar librerias externas
string ArbolMerkle::generarHashCasero(string contenido) {
    unsigned int hash = 0x811c9dc5; // semilla inicial estilo fnv-1a
    for (char c : contenido) {
        hash ^= (unsigned char)c;
        hash *= 0x01000193; // primo de multiplicacion
    }
    
    // lo convertimos a string hexadecimal
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(8) << hash;
    return ss.str();
}

// opcion 4: genera el .txt y mete su hash a la lista de bloques
string ArbolMerkle::generarCertificadoFisico(string fecha, string cod_lote, string cod_finca, string nombre_finca, int sacos, string tipo_cafe) {
    // 1. Hash del nombre (hash_nombre)
    string hash_nombre = generarHashCasero(cod_lote);
    string nombre_archivo = "certificados/" + hash_nombre + ".txt";

    // 2. Armar el contenido con el formato exacto de la rubrica
    string contenido = "CERTIFICADO DE ENTREGA - EDD COFFEETRACK\n";
    contenido += "========================================\n";
    contenido += "Fecha          : " + fecha + "\n";
    contenido += "Codigo lote    : " + cod_lote + "\n";
    contenido += "Finca          : " + nombre_finca + " (" + cod_finca + ")\n";
    contenido += "Tipo de cafe   : " + tipo_cafe + "\n";
    contenido += "Sacos          : " + std::to_string(sacos) + "\n";
    contenido += "Ruta tomada    : N/A (Fase 2 modificada)\n";
    contenido += "Distancia      : N/A\n";
    contenido += "Estado final   : certificado_emitido\n";
    contenido += "========================================\n";

    // 3. Hash del contenido (hash_contenido)
    string hash_contenido = generarHashCasero(contenido);
    contenido += "Hash de contenido: " + hash_contenido + "\n";

    ofstream archivo(nombre_archivo);
    if (archivo.is_open()) {
        archivo << contenido;
        archivo.close();
        
        NodoHashList* nuevo = new NodoHashList(hash_contenido);
        if (cabeza_hashes == nullptr) {
            cabeza_hashes = nuevo;
        } else {
            NodoHashList* actual = cabeza_hashes;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
        cantidad_bloques++;
        cout << "[certificado creado] " << nombre_archivo << " -> Hash Contenido: " << hash_contenido << endl;
        
        return hash_contenido; 
    } else {
        cout << "[error] no se pudo crear el archivo." << endl;
        return "";
    }
}

NodoHashList* ArbolMerkle::obtenerNodoEnIndice(NodoHashList* inicio, int indice) {
    NodoHashList* actual = inicio;
    for (int i = 0; i < indice && actual != nullptr; i++) {
        actual = actual->siguiente;
    }
    return actual;
}

// construye el arbol recursivamente emparejando hashes
NodoMerkle* ArbolMerkle::construirArbolRecursivo(NodoHashList* inicio, int cantidad) {
    if (cantidad == 0) return nullptr;
    if (cantidad == 1) {
        return new NodoMerkle(inicio->hash);
    }

    // calculamos la mitad para dividir
    int mitad = cantidad / 2;
    // ajustamos si la cantidad es impar para que la rama izquierda se lleve el extra
    if (cantidad % 2 != 0) {
        mitad++; 
    }

    NodoMerkle* izq = construirArbolRecursivo(inicio, mitad);
    
    NodoHashList* inicio_derecha = obtenerNodoEnIndice(inicio, mitad);
    NodoMerkle* der = construirArbolRecursivo(inicio_derecha, cantidad - mitad);

    // concatenamos los hashes hijos y les volvemos a aplicar hash
    string hash_combinado = izq->hash;
    if (der != nullptr) {
        hash_combinado += der->hash;
    }
    
    NodoMerkle* padre = new NodoMerkle(generarHashCasero(hash_combinado));
    padre->izquierdo = izq;
    padre->derecho = der;

    return padre;
}

// opcion 5: levanta la estructura del arbol a partir de los hashes guardados
void ArbolMerkle::construirArbol() {
    if (cantidad_bloques == 0) {
        cout << "no hay certificados generados para construir el arbol de merkle." << endl;
        return;
    }
    
    limpiarArbol(raiz); // limpiamos si ya habia uno anterior
    raiz = construirArbolRecursivo(cabeza_hashes, cantidad_bloques);
    cout << "arbol de merkle construido exitosamente con " << cantidad_bloques << " bloques de datos." << endl;
}

void ArbolMerkle::mostrarRootHash() {
    if (raiz != nullptr) {
        cout << ">>> ROOT HASH: " << raiz->hash << " <<<" << endl;
    } else {
        cout << "el arbol no ha sido construido." << endl;
    }
}