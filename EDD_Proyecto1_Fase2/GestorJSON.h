#ifndef GESTORJSON_H
#define GESTORJSON_H

#include <string>
#include <fstream>
#include <iostream>
#include "ArbolB.h"

using std::string;

// nodo simple para guardar las fincas validas temporalmente
struct NodoFincaTemp {
    string codigo;
    string nombre;
    NodoFincaTemp* siguiente;

    NodoFincaTemp(string cod, string nom) {
        codigo = cod;
        nombre = nom;
        siguiente = nullptr;
    }
};

class GestorJSON {
private:
    NodoFincaTemp* cabeza_fincas;

    // funcioncitas auxiliares para extraer los datos sucios del string
    string extraerEntreComillas(string linea, string clave);
    int extraerNumero(string linea, string clave);

public:
    GestorJSON();
    ~GestorJSON();

    // metodos de validacion de fincas
    void registrarFincaValida(string codigo, string nombre);
    bool existeFinca(string codigo);

    // la funcion maestra que lee el archivo
    void cargarDesdeArchivo(string ruta, ArbolB& arbol_fechas);
};

#endif