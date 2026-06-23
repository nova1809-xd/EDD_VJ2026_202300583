#include "ArbolAVL.h"
#include <algorithm> 

using std::cout;
using std::endl;

ArbolAVL::ArbolAVL() {
    raiz = nullptr;
}

ArbolAVL::~ArbolAVL() {
    // pendiente: hacer un recorrido post-order para hacer delete de cada nodo y su historial
}

// obtener la altura de un nodo de forma segura para evitar nullpointers
int ArbolAVL::obtenerAltura(NodoAVL* nodo) {
    if (nodo == nullptr) {
        return 0;
    }
    return nodo->altura;
}

// calcular el factor de balance (altura izquierda - altura derecha)
int ArbolAVL::obtenerBalance(NodoAVL* nodo) {
    if (nodo == nullptr) {
        return 0;
    }
    return obtenerAltura(nodo->izquierdo) - obtenerAltura(nodo->derecho);
}

// rotacion simple a la derecha para el caso ll
NodoAVL* ArbolAVL::rotacionDerecha(NodoAVL* y) {
    NodoAVL* x = y->izquierdo;
    NodoAVL* T2 = x->derecho;

    // se ejecutan los cambios de punteros para rotar
    x->derecho = y;
    y->izquierdo = T2;

    // se actualiza la altura de los nodos afectados usando std::max
    y->altura = std::max(obtenerAltura(y->izquierdo), obtenerAltura(y->derecho)) + 1;
    x->altura = std::max(obtenerAltura(x->izquierdo), obtenerAltura(x->derecho)) + 1;

    // x pasa a ser la nueva raiz de este subarbol
    return x;
}

// rotacion simple a la izquierda para el caso rr
NodoAVL* ArbolAVL::rotacionIzquierda(NodoAVL* x) {
    NodoAVL* y = x->derecho;
    NodoAVL* T2 = y->izquierdo;

    // se ejecutan los cambios de punteros
    y->izquierdo = x;
    x->derecho = T2;

    // se recalculan las alturas tras el movimiento
    x->altura = std::max(obtenerAltura(x->izquierdo), obtenerAltura(x->derecho)) + 1;
    y->altura = std::max(obtenerAltura(y->izquierdo), obtenerAltura(y->derecho)) + 1;

    // y es la nueva raiz
    return y;
}

// funcion recursiva que inserta un lote y balancea el camino de regreso
NodoAVL* ArbolAVL::insertarRecursivo(NodoAVL* nodo, string cod_lote, string cod_finca, string nom_finca, int sacos, string tipo) {
    // paso 1: insercion normal como en cualquier bst tradicional
    if (nodo == nullptr) {
        return new NodoAVL(cod_lote, cod_finca, nom_finca, sacos, tipo);
    }

    // el criterio de ordenamiento que pide la rubrica es el codigo del lote
    if (cod_lote < nodo->codigo_lote) {
        nodo->izquierdo = insertarRecursivo(nodo->izquierdo, cod_lote, cod_finca, nom_finca, sacos, tipo);
    } else if (cod_lote > nodo->codigo_lote) {
        nodo->derecho = insertarRecursivo(nodo->derecho, cod_lote, cod_finca, nom_finca, sacos, tipo);
    } else {
        // si el codigo ya existe no hacemos nada porque no se permiten duplicados
        return nodo;
    }

    // paso 2: actualizamos la altura del ancestro actual
    nodo->altura = 1 + std::max(obtenerAltura(nodo->izquierdo), obtenerAltura(nodo->derecho));

    // paso 3: sacamos el factor de balance para ver si se arruino la estructura
    int balance = obtenerBalance(nodo);

    // paso 4: evaluacion de los 4 casos de desbalanceo

    // desbalanceo hacia la izquierda (ll)
    if (balance > 1 && cod_lote < nodo->izquierdo->codigo_lote) {
        return rotacionDerecha(nodo);
    }

    // desbalanceo hacia la derecha (rr)
    if (balance < -1 && cod_lote > nodo->derecho->codigo_lote) {
        return rotacionIzquierda(nodo);
    }

    // desbalanceo en zigzag izquierdo-derecho (lr)
    if (balance > 1 && cod_lote > nodo->izquierdo->codigo_lote) {
        nodo->izquierdo = rotacionIzquierda(nodo->izquierdo);
        return rotacionDerecha(nodo);
    }

    // desbalanceo en zigzag derecho-izquierdo (rl)
    if (balance < -1 && cod_lote < nodo->derecho->codigo_lote) {
        nodo->derecho = rotacionDerecha(nodo->derecho);
        return rotacionIzquierda(nodo);
    }

    // retornamos el puntero intacto si no hubo necesidad de rotar
    return nodo;
}

// wrapper publico para llamarlo facil desde el menu o el arbol b
void ArbolAVL::insertar(string cod_lote, string cod_finca, string nom_finca, int sacos, string tipo) {
    raiz = insertarRecursivo(raiz, cod_lote, cod_finca, nom_finca, sacos, tipo);
}

// busqueda binaria normalita de un lote especifico
NodoAVL* ArbolAVL::buscarLote(string codigo_lote) {
    NodoAVL* actual = raiz;
    
    // iteramos bajando por el arbol hasta topar con null o encontrarlo
    while (actual != nullptr) {
        if (codigo_lote < actual->codigo_lote) {
            actual = actual->izquierdo;
        } else if (codigo_lote > actual->codigo_lote) {
            actual = actual->derecho;
        } else {
            // si no es mayor ni menor, es este mero
            return actual; 
        }
    }
    
    // retornamos nullptr si de plano no existe
    return nullptr; 
}

// recorrido in-order para listar de menor a mayor segun pide el documento
void ArbolAVL::inOrderRecursivo(NodoAVL* nodo) {
    if (nodo != nullptr) {
        inOrderRecursivo(nodo->izquierdo);
        cout << "- Lote: " << nodo->codigo_lote << " | Finca: " << nodo->nombre_finca 
             << " | Sacos: " << nodo->sacos << " | Estado: " << nodo->estado << endl;
        inOrderRecursivo(nodo->derecho);
    }
}

// funcion publica para llamar al recorrido sin tener que pasar la raiz de parametro
void ArbolAVL::mostrarInOrder() {
    if (raiz == nullptr) {
        cout << "no hay lotes registrados en esta fecha." << endl;
        return;
    }
    inOrderRecursivo(raiz);
}