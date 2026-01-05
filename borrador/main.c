#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

typedef struct {
    int zonas;//numero de zonas n
    int** datos; // población, susceptibles e infectados por zona
    float** traslados; //matriz de transporte entre zonas
    float parametros[2]; // beta y mu entre 0.0 y 1.0 al igual que matriz de transporte
    int modelo[3]; // t entre 0 y t días, zona inicial y final entre 0 y n-1
} Virus; // esta estructura guardará los datos iniciales

typedef struct {
    int* S, *I, *R, *D;
    // susceptibles, infectados, recuperados y fallecidos SIRD
    int** Delta_I; // infectados por día en cada zona para obtener los de hace 3 días
} SIRD;


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
void inicio_simulacion(Virus* virus, SIRD* sird) {
    int n = virus->zonas;
    int t = virus->modelo[0]+1;
    
    // hacemos memoria para S, I, R, D
    sird->S = (int*)malloc(n * sizeof(int));
    sird->I = (int*)malloc(n * sizeof(int));
    sird->R = (int*)malloc(n * sizeof(int));
    sird->D = (int*)malloc(n * sizeof(int));
    
    // Reservar memoria para historia de infectados [zonas][dias+1]
    sird->Delta_I = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        sird->Delta_I[i] = (int*)malloc((t + 1) * sizeof(int));
    }
    
    // Inicializar con datos del archivo (día 0)
    for (int i = 0; i < n; i++) {
        sird->S[i] = virus->datos[i][1];  // Susceptibles iniciales
        sird->I[i] = virus->datos[i][2];  // Infectados iniciales
        sird->R[i] = 0;  // Inicialmente 0 recuperados
        sird->D[i] = 0;  // Inicialmente 0 fallecidos
        
        // Guardar infectados iniciales en historia (día 0)
        sird->Delta_I[i][0] = sird->I[i];
    }
}
void liberar_sird(SIRD* sird, int zonas) {
    if (!sird) return;
    
    free(sird->S);
    free(sird->I);
    free(sird->R);
    free(sird->D);
    
    if (sird->Delta_I) {
        for (int i = 0; i < zonas; i++) {
            free(sird->Delta_I[i]);
        }
        free(sird->Delta_I);
    }
}

int sucesiones(Virus* virus, SIRD* sird, int t){
    int n; // numero de zonas y días t para simulacion
    n=virus->zonas; 
    //t=virus->modelo[0];
    // int** S[n][t], I[n][t], R[n][t],D[n][t]; // S, I, R y D por zona
    float* Delta_I =(float*)malloc(n*sizeof(float)); // infectados dia a dia de cada zona 
    
    //valores constantes, porcentajes
    float beta, mu;
    float** g; 
    beta=virus->parametros[0]; 
    mu=virus->parametros[1];
    g=virus->traslados;
    
    // datos de destino al lugar que se desea viajar
    int zona1, zona2;
    zona1=virus->modelo[1];
    zona2=virus->modelo[2];
    
    // como delta I es n*t haré un for para calcular los valores SIRD zona por zona
    for (int i=0; i<n; i++){
        
        float sumatoria=0.0;
        //for para la sumatoria de matriz de transporte por el termino anterior de I
        for(int j=0; j<n; j++){
            int Ij=sird->I[j];
            sumatoria = sumatoria + g[i][j]*Ij;
        }
        int Sj=sird->S[i];
        int Ni=virus->datos[i][0];
        Delta_I[i]=(beta*(float)Sj/(float)Ni)*sumatoria;
    }

    // ahora un for para SIRD, importante que hay que dejar en numero entero
    for(int i=0; i<n; i++){
        int Delta_Ii=(int)roundf(Delta_I[i]); 
        
        //susceptibles
        sird->S[i]=sird->S[i] - Delta_Ii; 
        if(sird->S[i]<0) sird->S[i]=0; // para que no haya negativos
        
        //infectados
        int I3 = (t>=3)?sird->Delta_I[i][t-3] : 0;
        sird->I[i]=sird->I[i] + Delta_Ii-I3;
        if(sird->I[i]<0) sird->I[i]=0; // para que no haya negativos
        
        //recuperados
        int Re=(int)roundf((1.0-mu)*I3);
        sird->R[i] += Re;

        //fallecidos
        int F=(int)roundf(mu*I3);
        sird->D[i]+= F;

        sird->Delta_I[i][t]=sird->I[i]; // I del día t en la zona i

    }
    free(Delta_I);
    return 0;
}

void mostrar_resultados(Virus* virus, SIRD* sird, int dia) {
    printf("Día %d:\n", dia);
    for (int i = 0; i < virus->zonas; i++) {
        printf("  Zona %d: S=%d, I=%d, R=%d, D=%d\n", 
                  i, sird->S[i], sird->I[i], sird->R[i], sird->D[i]);
    }
}

void resultados(Virus* virus, SIRD* sird) {
    int t = virus->modelo[0];
    
    // Susceptibles por día 


    // Infectados por día 
    for (int i = 0; i < virus->zonas; i++) {
        printf("I%d = [", i);
        for (int d = 0; d <= t; d++) {
            printf("%d", sird->Delta_I[i][d]);
            if (d < t) printf(", ");
        }
        printf("]\n");
    }
    
    // Recuperados por día
    for (int i = 0; i < virus->zonas; i++) {
        printf("R%d = [", i);
        for (int d = 0; d <= t; d++) {
            printf("%d", sird->R[i]);
            if (d < t) printf(", ");
        }
        printf("]\n");
    }
    
    // Fallecidos por día
    for (int i = 0; i < virus->zonas; i++) {
        printf("D%d = [", i);
        for (int d = 0; d <= t; d++) {
            printf("%d", sird->D[i]);
            if (d < t) printf(", ");
        }
        printf("]\n");
    }
}

// Función para Dijkstra *** por completar ***
void recorrido_dijkstra(Virus* virus) {
    // por mientras un mensaje de la función :p
    int dia = virus->modelo[0];
    int zona1= virus->modelo[1];
    int zona2= virus->modelo[2];
    //printf("Recorrido con menor número de contagiados del día %d:\n", dia);
    //printf("Desde zona %d hasta zona %d\n", zona1, zona2);
    // ...
    int n=virus->zonas;
    
}


int main() {
    Virus virus = {0}; 
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

    // Inicio de la simulación
    SIRD sird;
    inicio_simulacion(&virus, &sird);
    
    printf("\n=== SIMULACIÓN ===\n");
    mostrar_resultados(&virus, &sird, 0);
    
    // Simular día por día
    for (int t = 1; t <= virus.modelo[0]; t++) {
        sucesiones(&virus, &sird, t);
        mostrar_resultados(&virus, &sird, t);
    }
    
    // Buscar camino mínimo
    //printf("\n=== RECORRIDO CON MENOS INFECTADOS ===\n");
    //recorrido_dijkstra(&virus);
    
    // Preguntar si quiere otro archivo
    char respuesta;
    printf("\n¿Desea consultar otro archivo? (s/n): ");
    scanf(" %c", &respuesta);
    // Esta parte se la pedí a la IA porque no sabía cómo repetir el main en caso de que quisiera otro archivo
    // dejarlo explicito para no confundir
    if (respuesta == 's' || respuesta == 'S') {
        // Limpiar y repetir
        liberar_sird(&sird, virus.zonas);
        liberar_virus(&virus);
        main(); //trucazo 
    } else {
        // Liberar memoria y salir
        liberar_sird(&sird, virus.zonas);
        liberar_virus(&virus);
        printf("Programa finalizado.\n");
    }
    // Liberar memoria al final;
    liberar_virus(&virus);
    
    return 0;
}

