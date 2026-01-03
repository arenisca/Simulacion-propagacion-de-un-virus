#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int zonas;//numero de zonas n
    int** datos; // población, susceptibles e infectados por zona
    float** traslados; //matriz de transporte entre zonas
    float parametros[2]; // beta y mu entre 0.0 y 1.0 al igual que matriz de transporte
    int modelo[3]; // t entre 0 y t días, zona inicial y final entre 0 y n-1
} Virus; // esta estructura guardará los datos iniciales

int leer_archivo(const char* txt, Virus* virus){
    if (!txt || !virus) {
        printf("Error en parámetros de lectura de archivo.\n");
        return -1;
    }

    FILE* archivo=fopen(txt,"r");
    if(!archivo){printf("Error al abrir el archivo %s\n", txt); return -1;}
    
    // linea 0: para lectura del numero de zonas (n)
    if (fscanf(archivo, "%d", &virus->zonas) != 1 || virus->zonas <= 0) {
        printf("Error: número de zonas inválido.\n");
        fclose(archivo);
        return -1;
    }

    // malloc para memoria de "datos"
    virus->datos = (int**)malloc(virus->zonas * sizeof(int*));
    for (int i = 0; i < virus->zonas; i++) {
        virus->datos[i] = (int*)malloc(3 * sizeof(int));
    }

    //linea 1 a n: lectura datos (población, susceptibles, infectados) x cada zona
    for (int i = 0; i < virus->zonas; i++) {
        if (fscanf(archivo, "%d %d %d", &virus->datos[i][0],  // población
                                        &virus->datos[i][1],  // susceptibles
                                        &virus->datos[i][2])  // infectados
                                                       != 3) {
            printf("Error en datos de zona %d\n", i);
            fclose(archivo);
            return -1;
        }
        
        // Validación de numero de la poblacion con susceptibles más infectados
        if (virus->datos[i][0] <= 0 || 
            virus->datos[i][1] < 0 || 
            virus->datos[i][2] < 0 ||
            virus->datos[i][1] + virus->datos[i][2] > virus->datos[i][0]) {
            printf("Datos inválidos en zona %d\n", i);
            fclose(archivo);
            return -1;
        }
    }

    // preparo matriz de traslado
    virus->traslados = (float**)malloc(virus->zonas * sizeof(float*));
    for (int i = 0; i < virus->zonas; i++) {
        virus->traslados[i] = (float*)malloc(virus->zonas * sizeof(float));
    }

    //linea n+1 a 2n: matriz de traslados entre zonas
    for (int i = 0; i < virus->zonas; i++) {
        for (int j = 0; j < virus->zonas; j++) {
            if (fscanf(archivo, "%f", &virus->traslados[i][j]) != 1) {
                printf("Error en matriz de traslados [%d][%d]\n", i, j);
                fclose(archivo);
                return -1;
            }
        }
    }

    //linea 2n+1: parámetros beta y mu
    if (fscanf(archivo, "%f %f", &virus->parametros[0], &virus->parametros[1]) != 2) {
        printf("Error en parámetros beta y mu\n");
        fclose(archivo);
        return -1;
    }

    // linea 2n+2: día t, zona_inicial y zona_final
    if (fscanf(archivo, "%d %d %d", 
               &virus->modelo[0],  // día
               &virus->modelo[1],  // zona inicial
               &virus->modelo[2])  // zona final
               != 3) {
        printf("Error en datos del modelo\n");
        fclose(archivo);
        return -1;
    }

    // verificación de que zona inicial y final esté entre 0 y n
    if (virus->modelo[1] < 0 || virus->modelo[1] >= virus->zonas ||
        virus->modelo[2] < 0 || virus->modelo[2] >= virus->zonas) {
        printf("Error: zonas inicial/final fuera de rango\n");
        fclose(archivo);
        return -1;
    }

    fclose(archivo);
    return 0; // Éxito

}
void liberar_virus(Virus* virus) {
    if (!virus) return;
    
    // Liberar datos
    if (virus->datos) {
        for (int i = 0; i < virus->zonas; i++) {
            free(virus->datos[i]);
        }
        free(virus->datos);
    }
    
    // Liberar traslados
    if (virus->traslados) {
        for (int i = 0; i < virus->zonas; i++) {
            free(virus->traslados[i]);
        }
        free(virus->traslados);
    }
}

int sucesiones(Virus* virus){
    n=virus->zonas; 
    t=virus->modelo[0];
    int S[n], I[n], R[n],D[n]; // S, I, R y D por zona
    int Delta_I[n][t]; // infectados dia a dia
    float g[n][n]; // matriz de trnsporte
    float beta, mu;
    beta=virus->parametros[0]; 
    mu=virus->parametros[1];
}

int main() {
    Virus virus = {0}; // Inicializamos estructura para datos del archivo
    char nombre_archivo[256];
    int intentos = 0;
    const int MAX_INTENTOS = 3;

    do {
        printf("Ingrese nombre del archivo: ");
        scanf("%255s", nombre_archivo);

        if (leer_archivo(nombre_archivo, &virus) == 0) {
            printf("Archivo leído exitosamente!\n");
            printf("Zonas: %d\n", virus.zonas);
            printf("Beta: %.2f, Mu: %.2f\n", 
                   virus.parametros[0], virus.parametros[1]);
            break;
        } else {
            intentos++;
            printf("Intento %d de %d fallido.\n", intentos, MAX_INTENTOS);
            
            // Limpiar estructura para reintentar
            liberar_virus(&virus);
            virus.zonas = 0;
            virus.datos = NULL;
            virus.traslados = NULL;
        }
    } while (intentos < MAX_INTENTOS);

    if (intentos >= MAX_INTENTOS) {
        printf("Demasiados intentos fallidos. Saliendo...\n");
        return 1;
    }

    // Aquí continuaría la simulación...
    
    // Liberar memoria al final
    liberar_virus(&virus);
    
    return 0;
}


