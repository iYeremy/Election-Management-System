# AGENTS.md

## Objetivo General

Este proyecto simula un sistema electoral colombiano completo, abarcando elecciones presidenciales y municipales. Utiliza estructuras de datos personalizadas para representar regiones, ciudades, candidatos y votaciones, con funcionalidades de simulación, consulta de resultados y estadísticas. Se busca mantener la modularidad y consistencia en el diseño, alineado con los diagramas y requisitos definidos en la guía.

## Estructuras Clave y su Flujo

El proyecto se organiza según el siguiente flujo de estructuras:

1. **Multilista Regiones-Ciudades** (`MultilistaRegiones`, `Ciudad`, `Region`)

    - Cada `Region` apunta a una lista simple de `Ciudad`.
    - Cada `Ciudad` pertenece a una `Region` y contiene su propio censo y multilista de candidatos.

2. **AVL de Ciudades** (`AVLCiudades`)

    - Organiza todas las ciudades por nombre alfabéticamente.
    - Se usa para rápido acceso, búsqueda y recorrido inorden.

3. **AVL de Candidatos** (`AVLCandidatos`) — _pendiente_

    - Organiza todos los candidatos presidenciales por ID numérico.
    - Se usará para consultas y verificaciones globales.

4. **Lista Doble de Candidatos por Ciudad** (`NodoCandidato`, `Candidato`)

    - Cada `Ciudad` tiene su propia lista doble de `NodoCandidato` (multilista).
    - El `Candidato` tiene punteros hacia nodos municipales y presidenciales.

5. **Duo Presidencial** (`Duo`, `Partido`)

    - Agrupa una fórmula presidencial (presidente + vicepresidente) con su partido.

6. **Tarjetones de Votación** (`TarjetonAlcaldia`, `TarjetonPresidencial`)

    - Generan votos aleatorios (por índice) entre opciones posibles: candidatos, blanco, nulo, abstención.
    - No están conectados directamente a multilistas; la interpretación de índices se maneja externamente.

7. **Simulador Electoral** (`SistemaElectoral`) — _en construcción_

    - Controla la carga de datos, simulación de votaciones y generación de reportes.

8. **Estadísticas y Votos**

    - Cada `Ciudad` registra votos individuales por candidato, blanco, nulo y abstención.
    - Calcula porcentajes y ganador local usando métodos internos (`porcentajeX`, `ganadorAlcaldia`, etc.).

## Detalles por Clase Clave

### `Ciudad`

-   Multilista con `NodoCandidato` (lista doble enlazada por ciudad).
-   Métodos para registrar votos y calcular estadísticas.
-   Enlace hacia su `Region` y hacia la siguiente ciudad (multilista).
-   Ya está integrada con limpieza de enlaces al insertar candidatos.

### `AVLCiudades`

-   Inserta, balancea y destruye nodos usando pila propia.
-   Recorrido inorden para impresión alfabética.

### `TarjetonAlcaldia` / `TarjetonPresidencial`

-   Usan `rand()%opciones` para generar un voto.
-   Actualmente devuelven un índice (0..6 / 0..7).
-   **Limitaciones**: no validan si realmente hay candidatos disponibles.

## Notas para Codex

-   **No asumir STL**: Evitar `vector`, `map`, etc. Se usan estructuras propias.
-   **No usar memoria dinámica sin limpieza**: Si creas nodos, asegúrate de que se integran o destruyen.
-   **No ignorar flujo de estructuras**: Todas las inserciones y relaciones deben respetar el diseño (ciudades están en multilista y AVL, candidatos en multilista y AVL).
-   **Evitar lógica duplicada**: Si algo se maneja en `Ciudad`, no repetirlo en otra clase.

## Pendiente

-   Finalizar `SistemaElectoral`.
-   Conectar tarjetones con estructuras reales (interpretar índices).
-   Implementar cargadores de datos.
-   Validaciones de simulación.

---

Este archivo sirve como guía de contexto para mantener a los asistentes de código enfocados en la arquitectura y evitar errores de integración o confusiones con estructuras no utilizadas.
