# Simulacion de la propagación de un virus

**Estudiante**: Valentina Serón Canales 

**Asignatura**: Matemáticas Discretas

**Departamento de Informática y Cs. de la Computación** 

**Facultad de Ingeniería** 

**Universidad de Concepción**

## Descripción
Esta tarea desarrolla el modelo de propagación de un virus en distintas zonas con numero de población, la cual contiene personas Susceptibles, Infectadas, Recuperadas y Fallecidas. 

Variables por zona:
- Sᵢ(t): Susceptibles en el día t 
- Iᵢ(t): Infectados en el día t
- Rᵢ(t): Recuperados en el día t
- Dᵢ(t): Fallecidos en el día t
Donde i:{0,1,..,n-1} y n es el número de zonas.

Los parametros usados en ecuaciones son:
- β: Tasa de contagio (0 ≤ β ≤ 1)
- μ: Tasa de mortalidad (0 ≤ μ ≤ 1)
- gᵢⱼ: Porcentaje que se mueve de zona i a zona j

Las ecuaciones que modelan al problema:
- ΔIᵢ(t) = (β × Sᵢ(t-1) / Nᵢ) × Σⱼ [gᵢⱼ × Iⱼ(t-1)]
- Sᵢ(t) = Sᵢ(t-1) - ΔIᵢ(t)
- Iᵢ(t) = Iᵢ(t-1) + ΔIᵢ(t) - Iᵢ(t-3)
- Rᵢ(t) = Rᵢ(t-1) + (1-μ) × Iᵢ(t-3)
- Dᵢ(t) = Dᵢ(t-1) + μ × Iᵢ(t-3)

Este trabajo entrega los elementos señalados por día y utiliza el algoritmo de Dijkstra para encontrar un recorrido de una zona i a otra j con la menor cantidad de infectados que se podría topar una persona. 

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

## Consideraciones del programa
- El programa solo recibe archivos .txt con la configuración señalada.
- Existe una consulta para probar más archivos de texto con datos de simulación.
- Si se falla más de 3 veces en el archivo, el programa termina.
- Solo considera archivos dentro de la carpeta de trabajo.
