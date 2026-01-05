#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sird.h"
#include "virus.h"

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
