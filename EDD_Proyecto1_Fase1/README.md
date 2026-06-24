# Manual Técnico - EDD Coffee Track (Fase 1)

**Estudiante:** Angel Sebastian Mazariegos Pérez
**Carné:** 202300583

## 1. Descripción General del Sistema
El sistema EDD Coffee Track es una aplicación de consola desarrollada en C++ para la gestión de inventarios y procesos en el beneficio húmedo del café. El sistema automatiza la recepción de sacos, la línea de espera de despulpado, el inventario de bodega y mantiene una bitácora inmutable de transacciones. 

## 2. Estructuras de Datos Implementadas
Para cumplir con los requerimientos, se implementaron las siguientes estructuras de datos lineales basadas en el uso estricto de memoria dinámica (sin uso de librerías STL).

### 2.1 Lista Doblemente Enlazada (Inventario de Lotes de Café)
* **Decisión de Diseño:** Se eligió una lista doblemente enlazada para mantener el inventario de bodega, ya que permite la inserción ordenada de lotes (por código de lote) y facilita los recorridos bidireccionales, necesarios para la visualización del inventario y la generación de reportes.
* **Análisis de Complejidad:**
    * `insertarOrdenado()`: **O(n)** en el peor de los casos, ya que debe recorrer la lista para encontrar la posición correcta de inserción.
    * `buscarYObtener()`: **O(n)**, requiere recorrer la lista secuencialmente para encontrar el lote solicitado.

### 2.2 Cola (Cola de Procesamiento/Despulpado)
* **Decisión de Diseño:** Se implementó una cola (FIFO - First In, First Out) con punteros al `frente` y al `final` para gestionar los lotes que esperan ser despulpados. Esto garantiza que el primer lote en llegar sea el primero en ser procesado.
* **Análisis de Complejidad:**
    * `enqueue()` (Insertar al final): **O(1)**, gracias al uso del puntero `final`.
    * `dequeue()` (Extraer del frente): **O(1)**, operación directa utilizando el puntero `frente`.

### 2.3 Pila (Bitácora del Jefe de Beneficio)
* **Decisión de Diseño:** Se utilizó una pila (LIFO - Last In, First Out) para la bitácora de transacciones del administrador. Esto permite que las operaciones más recientes se visualicen primero, facilitando la auditoría cronológica inversa.
* **Análisis de Complejidad:**
    * `push()` (Insertar al tope): **O(1)**, operación directa modificando el puntero `tope`.
    * `mostrarHistorial()`: **O(n)**, ya que debe recorrer la pila desde el tope hasta el fondo para imprimir todos los registros.

### 2.4 Lista de Pilas (Fincas y sus Entregas Diarias)
* **Decisión de Diseño:** Se combinó una lista simplemente enlazada para gestionar las fincas productoras, donde cada nodo de la lista contiene un puntero hacia una estructura de pila interna que registra el historial de entregas de esa finca.
* **Análisis de Complejidad:**
    * `registrarFinca()`: **O(1)**, ya que se inserta al inicio de la lista simple de fincas.
    * `registrarEntregaFinca()`: **O(n)**, requiere buscar secuencialmente la finca en la lista (O(n)) y luego realizar la operación `push` en su pila interna (O(1)).

## 3. Generación de Reportes Visuales (Graphviz)
El sistema incluye un módulo para la generación automática de archivos `.dot` que son compilados mediante comandos del sistema a imágenes PNG. Se crearon representaciones visuales para las cuatro estructuras de datos, aplicando configuraciones de nodos (`shape=record`), enlaces direccionales y bidireccionales, agrupaciones (`rank=same`) y códigos de colores según los requisitos del proyecto.