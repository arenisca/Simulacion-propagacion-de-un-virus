**Departamento de Informática y Cs. de la Computación** 
**Facultad de Ingeniería** 
**Universidad de Concepción** 

### Tarea Computacional Matemáticas Discretas 
## Simulacion de la propagación de un virus 

**Docente**: Prof. Guillermo Cabrera Vives 

**Estudiante**: Valentina Serón Canales @arenisca 

## Descripción
Esta tarea desarrolla el modelo de propagación de un virus en distintas zonas con numero de población N, la cual contiene personas Susceptibles, Infectadas, Recuperadas y Fallecidas. Este trabajo entrega los elementos señalados por día y utiliza el algoritmo de Dijkstra para encontrar un recorrido de una zona i a otra j con la menor cantidad de infectados que se podría topar una persona. 

## Estructura 
```bash
Simulacion-propagacion-de-un-virus/
│ 
├── borrador # Carpeta con archivo .c borrador de la tarea 
├── README.md # archivo markdown 
├── dijkstra.c # Algoritmo de Dijkstra  
├── dijkstra.h # Cabeceras para dijkstra.c 
├── propagacion_virus.c # Programa principal (interfaz de usuario) 
├── sird.c # Implementación del modelo epidemiológico 
├── sird.h # Cabeceras para sird.c 
├── virus.c # Manejo de datos y parámetros del modelo 
├── virus.h # Cabeceras para virus.c 
└── virus_ejemplo2.txt # Archivo de ejemplo 2 para pruebas 
 ```
## Compilación y ejecución
- Compilador GCC
- Sistema Linux/Unix
- Biblioteca matemática estándar

```bash
gcc propagacion_virus.c virus.c sird.c dijkstra.c -o virus -lm
.\virus 
``` 
## Formato del archivo .txt de entrada
```bash
virus_ejemplo.txt:
3                       ← Número de zonas
100000 100000 0         ← Zona 0: Población, Susceptibles, Infectados
200000 200000 0         ← Zona 1: Población, Susceptibles, Infectados
300000 298000 2000      ← Zona 2: Población, Susceptibles, Infectados
0.5 0.0 0.5             ← Matriz transporte: fila 0 (g₀₀, g₀₁, g₀₂)
0.0 0.7 0.3             ← Matriz transporte: fila 1 (g₁₀, g₁₁, g₁₂)
0.0 0.3 0.7             ← Matriz transporte: fila 2 (g₂₀, g₂₁, g₂₂)
0.4 0.1                 ← Parámetros: β (tasa contagio), μ (tasa mortalidad)
4 0 1                   ← Día final de simulación, zona inicio, zona destino
```
## Consideraciones del programa
- Existe una consulta para probar más archivos de texto con datos de simulación.
- si se falla más de 3 veces en el archivo, el programa termina.

