#ifndef COLADESPULPADO_H
#define COLADESPULPADO_H

#include <string>

using std::string;

// este nodo guarda los datos de la cola y el enlace al siguiente
struct NodoCola {
    string numero_lote;
    string finca_solicitante;
    int cantidad_sacos;
    string fecha_ingreso;
    NodoCola* siguiente;
};

// esta clase maneja una cola simple con punteros al frente y al final
class ColaDespulpado {
private:
    NodoCola* frente;
    NodoCola* final;

public:
    ColaDespulpado();
    ~ColaDespulpado();

    void enqueue(const string& numero_lote,
                 const string& finca_solicitante,
                 int cantidad_sacos,
                 const string& fecha_ingreso);

    bool dequeue(string& numero_lote,
                 string& finca_solicitante,
                 int& cantidad_sacos,
                 string& fecha_ingreso);

    bool estaVacia() const;
};

#endif