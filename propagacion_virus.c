#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include "virus.h"
#include "sird.h"
#include "dijkstra.h"

int main() {
    char respuesta;
    do {
        Virus virus = {0}; 
        char nombre_archivo[256]; 
        int intentos = 0;
        const int MAX_INTENTOS = 3;

        do {
            printf("Ingrese nombre del archivo: ");
            scanf("%255s", nombre_archivo);

            if (leer_archivo(nombre_archivo, &virus) == 0) {
                printf("Archivo leído exitosamente!\n");
                break;
            } else {
                intentos++;
                printf("Intento %d de %d fallido.\n", intentos, MAX_INTENTOS);
                
            }
        } while (intentos < MAX_INTENTOS);

        if (intentos >= MAX_INTENTOS) {
            printf("Demasiados intentos fallidos. \n");
            liberar_virus(&virus);
            printf("¿Quiere intentar con otro archivo .txt? (s/n): ");
            scanf(" %c", &respuesta);
            getchar(); 
            if (respuesta != 's' && respuesta != 'S') break;
            continue;
        }

        // Inicio de la simulación
        SIRD sird;
        inicio_simulacion(&virus, &sird);
        
        printf("\n### SIMULACIÓN ###\n");
        mostrar_resultados(&virus, &sird, 0); // mostrar estado día 0
        
        // Simular día por día
        for (int t = 1; t <= virus.modelo[0]; t++) {
            sucesiones(&virus, &sird, t);
            mostrar_resultados(&virus, &sird, t);
        }
        
        // Buscar camino mínimo
        printf("\n=== RECORRIDO CON MENOS INFECTADOS ===\n");
        recorrido_dijkstra(&virus, &sird);

        liberar_sird(&sird, virus.zonas);
        liberar_virus(&virus);
        
        // Preguntar si quiere otro archivo
       
        printf("\n¿Desea consultar otro archivo? (s/n): ");
        scanf(" %c", &respuesta);
        getchar();
    }while(respuesta == 'S'|| respuesta == 's');
    
    printf("Programa finalizado.\n");
    
    return 0;
}