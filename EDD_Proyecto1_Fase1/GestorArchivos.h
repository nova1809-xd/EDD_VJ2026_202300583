#ifndef GESTORARCHIVOS_H
#define GESTORARCHIVOS_H

#include <string>
#include "ListaDoble.h"

// funcion para leer el csv y llenar el inventario de bodega
void cargarLotesCSV(const std::string& ruta_archivo, ListaDoble& inventario);

#endif