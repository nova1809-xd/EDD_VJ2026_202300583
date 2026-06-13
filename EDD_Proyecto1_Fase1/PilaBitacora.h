#ifndef PILABITACORA_H
#define PILABITACORA_H

#include <string>

using std::string;

// nodo para guardar las transacciones del jefe de beneficio
struct NodoBitacora {
    string fecha_hora;
    string tipo_accion;
    string detalle_operacion;
    NodoBitacora* siguiente;
};

// clase que maneja la pila (stack) tipo lifo
class PilaBitacora {
private:
    NodoBitacora* tope;

public:
    PilaBitacora();
    ~PilaBitacora();

    void push(const string& fecha_hora, const string& tipo_accion, const string& detalle_operacion);
    void mostrarHistorial() const;
    bool estaVacia() const;
};

#endif