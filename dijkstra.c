#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include "virus.h"
#include "sird.h"
// Función para Dijkstra 
void recorrido_dijkstra(Virus* virus, SIRD* sird) {
    // por mientras un mensaje de la función :p
    int dia = virus->modelo[0];
    int zona1= virus->modelo[1];
    int zona2= virus->modelo[2];
    
    // estas estructuras van a guardar las distancias, nodos anteriores y visitados
    int n=virus->zonas;
    int* distancias = (int*)malloc(n * sizeof(int));
    int* anteriores = (int*)malloc(n * sizeof(int));
    bool* visitado = (bool*)malloc(n * sizeof(bool));

    //si el destino es la misma zona de inicio
    if (zona1==zona2){
        printf("Rmin:[%d]\n", zona1);
        return;
    }

    for (int i=0; i<n; i++){
        distancias[i]=INT_MAX;
        anteriores[i]=-1;
        visitado[i]=false;
    }
    // tomamos como "costo de inicio" del recorrido a la zona1
    distancias[zona1]=sird->I[zona1];
    // algoritmo de dijkstra: encuentra el recorrido con menor "distancia" (infectados)
    for (int aux=0;aux<n;aux++){
        int nodo=-1;
        int min_d=INT_MAX;
        for(int i=0;i<n;i++) {
            if(!visitado[i] && distancias[i] < min_d){
                min_d=distancias[i];
                nodo=i;
            }
        }
        // cuando llegue al nodo destino o no hay acceso a una zona a otra
        if(nodo==-1 || nodo==zona2) break;
        visitado[nodo]=true;

        // actualizar distancias de nodos adyacentes
        // zonas adyacentes = nodos adyacentes
        for(int nodo_ady=0;nodo_ady<n;nodo_ady++){
            if(virus->traslados[nodo][nodo_ady] > 0.0 && !visitado[nodo_ady]){
                int costo=sird->I[nodo_ady];
                int distancia= distancias[nodo] + costo;
                // el costo de ir a nodo adyacente es el numero de I de esa zona
                if(distancia<distancias[nodo_ady]){
                    distancias[nodo_ady]=distancia;
                    anteriores[nodo_ady]=nodo;
                }
            }
        }
    }
    if(distancias[zona2]==INT_MAX){
        printf("No hay un recorrido desde la zona %d a la %d\n", zona1, zona2);
    } else {
        
        int camino[n];
        int indice=0;
        for(int v=zona2;v!=-1;v=anteriores[v]){
            camino[indice++]=v;
        }
        printf("Rmin: [");
        for(int i=indice-1;i>=0;i--){
            printf("%d", camino[i]);
            if(i>0)printf(", ");
        }
        printf("]\n");
    }
    free(distancias);
    free(anteriores);
    free(visitado);
}
