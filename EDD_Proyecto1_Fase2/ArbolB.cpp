#include "ArbolB.h"
#include "ArbolMerkle.h" // incluimos la cabecera para la emision de certificados

using std::cout;
using std::endl;

ArbolB::ArbolB() {
    raiz = nullptr;
}

ArbolB::~ArbolB() {
    // pendiente: limpieza de memoria de todos los nodosB y sus instancias de ArbolAVL
}

// busqueda recursiva de una fecha exacta
ArbolAVL* ArbolB::buscarRecursivo(NodoB* nodo, string fecha) {
    if (nodo == nullptr) {
        return nullptr;
    }

    int i = 0;
    // iteramos hasta encontrar la fecha o pasarnos
    while (i < nodo->cantidad_claves && fecha > nodo->claves[i].fecha) {
        i++;
    }

    // si la encontramos en este nodo, retornamos su arbol avl
    if (i < nodo->cantidad_claves && fecha == nodo->claves[i].fecha) {
        return nodo->claves[i].avl_lotes;
    }

    // si llegamos a una hoja y no esta, entonces no existe
    if (nodo->es_hoja) {
        return nullptr;
    }

    // si no, bajamos por el hijo correspondiente
    return buscarRecursivo(nodo->hijos[i], fecha);
}

// wrapper publico para la busqueda por fecha
ArbolAVL* ArbolB::buscarFecha(string fecha) {
    return buscarRecursivo(raiz, fecha);
}

// funcion vital: divide un hijo que ya tiene 6 claves
void ArbolB::dividirHijo(NodoB* padre, int indice, NodoB* hijo_lleno) {
    // creamos el nuevo nodo z que sera el hermano derecho de hijo_lleno
    NodoB* z = new NodoB(hijo_lleno->es_hoja);
    
    // z se queda con las ultimas 2 claves del hijo lleno (indices 4 y 5)
    z->cantidad_claves = 2;
    for (int j = 0; j < 2; j++) {
        z->claves[j] = hijo_lleno->claves[j + 4];
    }

    // si no es hoja, tambien le pasamos los ultimos 3 punteros de los hijos (indices 4, 5 y 6)
    if (!hijo_lleno->es_hoja) {
        for (int j = 0; j < 3; j++) {
            z->hijos[j] = hijo_lleno->hijos[j + 4];
        }
    }

    // el hijo lleno original se queda solo con las primeras 3 claves (indices 0, 1 y 2)
    hijo_lleno->cantidad_claves = 3;

    // movemos los hijos del padre para hacerle espacio al nuevo nodo z
    for (int j = padre->cantidad_claves; j >= indice + 1; j--) {
        padre->hijos[j + 1] = padre->hijos[j];
    }
    padre->hijos[indice + 1] = z;

    // movemos las claves del padre para subir la clave mediana (indice 3)
    for (int j = padre->cantidad_claves - 1; j >= indice; j--) {
        padre->claves[j + 1] = padre->claves[j];
    }
    
    // la clave mediana sube al padre
    padre->claves[indice] = hijo_lleno->claves[3];
    padre->cantidad_claves++;
}

// insercion cuando sabemos que el nodo actual todavia tiene espacio
void ArbolB::insertarNoLleno(NodoB* nodo, string fecha, string cod_lote, string cod_finca, string nom_finca, int sacos, string tipo) {
    int i = nodo->cantidad_claves - 1;

    // primero verificamos si la fecha ya existe en este nodo especifico
    for (int j = 0; j < nodo->cantidad_claves; j++) {
        if (nodo->claves[j].fecha == fecha) {
            // la fecha ya existe, delegamos la insercion del lote a su arbol avl
            nodo->claves[j].avl_lotes->insertar(cod_lote, cod_finca, nom_finca, sacos, tipo);
            return;
        }
    }

    if (nodo->es_hoja) {
        // si es hoja, desplazamos las claves mayores hacia la derecha para hacer espacio
        while (i >= 0 && fecha < nodo->claves[i].fecha) {
            nodo->claves[i + 1] = nodo->claves[i];
            i--;
        }
        // insertamos la nueva fecha en su lugar ordenado
        nodo->claves[i + 1].fecha = fecha;
        // inicializamos su arbol avl y metemos el primer lote de ese dia
        nodo->claves[i + 1].avl_lotes = new ArbolAVL();
        nodo->claves[i + 1].avl_lotes->insertar(cod_lote, cod_finca, nom_finca, sacos, tipo);
        nodo->cantidad_claves++;
    } else {
        // si no es hoja, buscamos a que hijo debemos bajar
        while (i >= 0 && fecha < nodo->claves[i].fecha) {
            i--;
        }
        i++; // indice del hijo donde debemos bajar

        // verificamos si la fecha ya esta en algun hijo o descendiente de esta rama
        // este pre-chequeo evita hacer splits innecesarios si la fecha ya existia antes
        ArbolAVL* avl_existente = buscarRecursivo(nodo->hijos[i], fecha);
        if (avl_existente != nullptr) {
            avl_existente->insertar(cod_lote, cod_finca, nom_finca, sacos, tipo);
            return;
        }

        // si el hijo al que vamos a bajar esta lleno (6 claves), lo dividimos primero
        if (nodo->hijos[i]->cantidad_claves == 6) {
            dividirHijo(nodo, i, nodo->hijos[i]);
            // despues de dividir, la clave mediana subio a este nodo.
            // determinamos si bajamos por el hijo original o por el nuevo hermano
            if (fecha > nodo->claves[i].fecha) {
                i++;
            }
        }
        // llamamos recursivamente en el hijo correcto
        insertarNoLleno(nodo->hijos[i], fecha, cod_lote, cod_finca, nom_finca, sacos, tipo);
    }
}

// funcion principal de insercion del sistema
void ArbolB::insertarLote(string fecha, string cod_lote, string cod_finca, string nom_finca, int sacos, string tipo) {
    if (raiz == nullptr) {
        // el arbol esta vacio, creamos la raiz inicial como hoja
        raiz = new NodoB(true);
        raiz->claves[0].fecha = fecha;
        raiz->claves[0].avl_lotes = new ArbolAVL();
        raiz->claves[0].avl_lotes->insertar(cod_lote, cod_finca, nom_finca, sacos, tipo);
        raiz->cantidad_claves = 1;
    } else {
        // si la raiz esta completamente llena (6 claves) se rompe hacia arriba
        if (raiz->cantidad_claves == 6) {
            // creamos una nueva raiz vacia que no es hoja
            NodoB* nueva_raiz = new NodoB(false);
            nueva_raiz->hijos[0] = raiz;
            
            // dividimos la antigua raiz pasandole el control a la nueva
            dividirHijo(nueva_raiz, 0, raiz);
            
            // insertamos el dato nuevo decidiendo a que lado del split cae
            int i = 0;
            if (nueva_raiz->claves[0].fecha < fecha) {
                i++;
            }
            insertarNoLleno(nueva_raiz->hijos[i], fecha, cod_lote, cod_finca, nom_finca, sacos, tipo);
            
            // actualizamos el puntero maestro de la raiz
            raiz = nueva_raiz;
        } else {
            // si la raiz no esta llena, insertamos normal de forma recursiva
            insertarNoLleno(raiz, fecha, cod_lote, cod_finca, nom_finca, sacos, tipo);
        }
    }
}

// recorrido para listar todas las fechas registradas
void ArbolB::inOrderRecursivo(NodoB* nodo) {
    if (nodo != nullptr) {
        int i;
        for (i = 0; i < nodo->cantidad_claves; i++) {
            // primero visitamos al hijo izquierdo
            if (!nodo->es_hoja) {
                inOrderRecursivo(nodo->hijos[i]);
            }
            // imprimimos la fecha de este nodo
            cout << "- " << nodo->claves[i].fecha << endl;
        }
        // por ultimo visitamos el hijo mas a la derecha
        if (!nodo->es_hoja) {
            inOrderRecursivo(nodo->hijos[i]);
        }
    }
}

// wrapper publico para mostrar el listado de fechas en orden cronologico
void ArbolB::mostrarFechas() {
    if (raiz == nullptr) {
        cout << "no hay registros de entregas en el sistema." << endl;
        return;
    }
    inOrderRecursivo(raiz);
}

// recorrido recursivo para delegar la generacion masiva de certificados
void ArbolB::recorrerBParaCertificados(NodoB* nodo, ArbolMerkle& merkle) {
    if (nodo != nullptr) {
        int i;
        for (i = 0; i < nodo->cantidad_claves; i++) {
            if (!nodo->es_hoja) {
                recorrerBParaCertificados(nodo->hijos[i], merkle);
            }
            // delegamos al avl de esta fecha para que genere sus certificados de forma ordenada
            nodo->claves[i].avl_lotes->generarCertificadosLotes(merkle, nodo->claves[i].fecha);
        }
        if (!nodo->es_hoja) {
            recorrerBParaCertificados(nodo->hijos[i], merkle);
        }
    }
}

// llamada publica para procesar de un solo golpe todas las entregas existentes
void ArbolB::generarTodosLosCertificados(ArbolMerkle& merkle) {
    recorrerBParaCertificados(raiz, merkle);
}

// busqueda exhaustiva en todo el arbol B (util para buscar un lote cuando no sabemos su fecha)
NodoAVL* ArbolB::buscarLoteGlobalRecursivo(NodoB* nodo, string cod_lote, string& fecha_salida) {
    if (nodo == nullptr) return nullptr;

    for (int i = 0; i < nodo->cantidad_claves; i++) {
        // revisamos los hijos izquierdos antes de procesar la clave actual
        if (!nodo->es_hoja) {
            NodoAVL* encontrado = buscarLoteGlobalRecursivo(nodo->hijos[i], cod_lote, fecha_salida);
            if (encontrado != nullptr) return encontrado;
        }
        // revisamos el avl de la clave actual en busca del codigo del lote
        NodoAVL* lote = nodo->claves[i].avl_lotes->buscarLote(cod_lote);
        if (lote != nullptr) {
            fecha_salida = nodo->claves[i].fecha; // capturamos la fecha donde se encontro
            return lote;
        }
    }
    // revisamos el ultimo hijo derecho si no es nodo hoja
    if (!nodo->es_hoja) {
        return buscarLoteGlobalRecursivo(nodo->hijos[nodo->cantidad_claves], cod_lote, fecha_salida);
    }
    return nullptr;
}

// wrapper publico para la busqueda global sin conocer la fecha de antemano
NodoAVL* ArbolB::buscarLoteGlobal(string cod_lote, string& fecha_salida) {
    return buscarLoteGlobalRecursivo(raiz, cod_lote, fecha_salida);
}