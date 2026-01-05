#ifndef SIRD_H
#define SIRD_H
#include "virus.h"
#include "sird.h"

typedef struct {
    int* S, *I, *R, *D;
    // susceptibles, infectados, recuperados y fallecidos SIRD
    int** Delta_I; // infectados por día en cada zona para obtener los de hace 3 días
} SIRD;

//funciones para SIRD, calcular las ecuaciones y mostrar resultados
void inicio_simulacion(Virus* virus, SIRD* sird);
void liberar_sird(SIRD* sird, int zonas);
int sucesiones(Virus* virus, SIRD* sird, int t);
void mostrar_resultados(Virus* virus, SIRD* sird, int dia);

#endif // SIRD_H