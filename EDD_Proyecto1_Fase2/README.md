# Coffee Track - Fase 2 (Estructuras de Datos)

**Universidad de San Carlos de Guatemala**
**Facultad de Ingeniería - Escuela de Ciencias y Sistemas**
**Curso:** Estructuras de Datos
**Estudiante:** Angel Mazariegos
**Carné:** 202300583

---

## Descripcion del Proyecto
La Fase 2 de Coffee Track evoluciona el sistema implementando estructuras de datos no lineales. El objetivo principal es garantizar la trazabilidad completa del grano y asegurar la integridad criptográfica de la información mediante un Árbol de Merkle. 

El proyecto está desarrollado en C++ sin el uso de librerías de la STL para las estructuras principales, manejando indexación por fechas mediante un Árbol B, y organizando los lotes diarios mediante Árboles AVL.

---

## Decisiones de Diseño Justificadas

1. **Estructuras Anidadas (Árbol B + AVL):** Se utilizó un Árbol B como índice principal por fechas. Cada clave contiene un puntero hacia la raíz de un Árbol AVL independiente. Esta decisión aísla los datos por día, evitando que un solo árbol masivo se desbalancee o ralentice las búsquedas.
2. **Generación de Certificados Físicos:** Se optó por usar `<fstream>` para generar archivos `.txt` en la carpeta `certificados/`. Esto permite que el Árbol de Merkle lea el contenido físico para generar su hash, aislando el hash del nombre del hash del contenido.
3. **Trazabilidad por Lista Enlazada:** Para el historial de estados, se incluyó una lista simplemente enlazada dentro del nodo AVL. Esto permite un registro ligero con timestamps precisos usando `<ctime>`.
4. **Omisión de Grafo Ponderado:** Conforme a las directrices del enunciado, no se integró la estructura del Grafo Ponderado ni el algoritmo de Dijkstra para el cálculo de rutas óptimas, manteniendo la opción 3.1 enfocada únicamente en la inserción iterativa de fincas y conexiones en consola.

---

## Funcion de Hashing (Justificacion)

Para el cálculo de los hashes se implementó una variante casera del algoritmo FNV-1a (Fowler-Noll-Vo). 

**Justificación:**
* **Cero dependencias:** No requiere librerías externas, cumpliendo las restricciones.
* **Determinista:** La misma cadena de texto siempre producirá exactamente la misma salida hexadecimal.
* **Distribución Uniforme:** Utiliza operaciones a nivel de bits (XOR y multiplicaciones por números primos) que reducen drásticamente las colisiones.
* **Formato Estricto:** La salida es un string hexadecimal de longitud fija, perfecto para nombrar archivos y estructurar las hojas del Árbol de Merkle.

---

## Manual Tecnico: Analisis de Complejidad

### 1. Árbol B (Índice por Fecha - Orden 6)
* **Búsqueda: O(log_m n)**. Siendo `m` el orden del árbol (6) y `n` la cantidad de fechas. La altura crece muy lentamente, garantizando búsquedas rápidas.
* **Inserción: O(log_m n)**. Requiere buscar la hoja correcta y, en el peor caso, aplicar la división de nodos (split) hacia la raíz.
* **Recorrido In-Order: O(n)**. Visita secuencialmente cada clave y cada hijo recursivamente.

### 2. Árbol AVL (Registro de Lotes)
* **Búsqueda: O(log n)**. El factor de balanceo garantiza que la altura del árbol nunca sea mayor al logaritmo base 2 de la cantidad de nodos.
* **Inserción: O(log n)**. La ubicación toma tiempo logarítmico. Las rotaciones de balanceo (LL, RR, LR, RL) se ejecutan en tiempo constante O(1).

### 3. Árbol de Merkle
* **Construcción: O(n)**. Donde `n` es el número de certificados. El algoritmo procesa la lista de forma recursiva, visitando cada hoja y creando cada nodo intermedio exactamente una vez.
* **Cálculo de Hash: O(L)**. Donde `L` es la longitud del string. Evalúa cada carácter individualmente.

---

## Compilacion y Ejecucion

**Comando de Compilación:**
```bash
g++ main.cpp ArbolAVL.cpp ArbolB.cpp GestorJSON.cpp ArbolMerkle.cpp -o coffee_track_f2.exe