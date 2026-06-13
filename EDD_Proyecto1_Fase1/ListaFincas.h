#ifndef LISTAFINCAS_H
#define LISTAFINCAS_H

#include <string>

using std::string;

// nodo para la pila de entregas que tendra cada finca
struct NodoEntrega {
    string fecha;
    int cantidad_sacos;
    NodoEntrega* siguiente;
};

// clase para manejar la pila interna de entregas de forma independiente
class PilaEntregas {
private:
    NodoEntrega* tope;

public:
    PilaEntregas();
    ~PilaEntregas();

    void push(const string& fecha, int cantidad_sacos);
    void mostrarEntregas() const;
};

// nodo para la lista simple de fincas
struct NodoFinca {
    string nombre_finca;
    PilaEntregas* historial_entregas; // el puntero a la pila de esta finca
    NodoFinca* siguiente;
};

// clase principal para manejar la lista de fincas
class ListaFincas {
private:
    NodoFinca* cabeza;

public:
    ListaFincas();
    ~ListaFincas();

    void registrarFinca(const string& nombre_finca);
    void registrarEntregaFinca(const string& nombre_finca, const string& fecha, int cantidad_sacos);
    void mostrarTodo() const;
};

#endif