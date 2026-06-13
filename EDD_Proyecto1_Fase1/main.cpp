#include "ListaDoble.h"
#include "ColaDespulpado.h"
#include "PilaBitacora.h"
#include "ListaFincas.h"
#include "GestorArchivos.h"
#include <iostream>

using std::cout;
using std::endl;

int main() {
    ListaDoble inventario;
    ColaDespulpado cola;
    PilaBitacora bitacora;
    ListaFincas fincas;

    cout << "--- GENERANDO DATOS DE PRUEBA ---" << endl;

    // 1. Inventario
    cargarLotesCSV("datos.csv", inventario);
    
    // 2. Cola
    cola.enqueue("LOT-003", "Finca La Hermosa", 200, "2026-06-02");
    cola.enqueue("LOT-022", "Finca Las Nubes", 60, "2026-06-09");
    
    // 3. Bitacora
    bitacora.push("2026-06-13 10:00", "Registro de Lote", "Se registro Lote 003 de Finca La Hermosa");
    bitacora.push("2026-06-13 14:30", "Envio a Cola", "Lote 022 enviado a despulpado");

    // 4. Fincas y Entregas
    fincas.registrarFinca("Finca El Injerto");
    fincas.registrarEntregaFinca("Finca El Injerto", "2026-06-10", 40);
    fincas.registrarEntregaFinca("Finca El Injerto", "2026-06-12", 45);
    
    fincas.registrarFinca("Finca Santa Elena");
    fincas.registrarEntregaFinca("Finca Santa Elena", "2026-06-01", 120);

    cout << "\n--- GENERANDO REPORTES GRAPHVIZ ---" << endl;
    inventario.generarReporteGrafo();
    cola.generarReporteGrafo();
    bitacora.generarReporteGrafo();
    fincas.generarReporteGrafo();

    return 0;
}