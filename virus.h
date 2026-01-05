#ifndef VIRUS_H
#define VIRUS_H
// la estructura Virus tiene los datos iniciales para comenzar la simulación
typedef struct {
    int zonas;//numero de zonas n
    int** datos; // población, susceptibles e infectados por zona
    float** traslados; //matriz de transporte entre zonas
    float parametros[2]; // beta y mu entre 0.0 y 1.0 al igual que matriz de transporte
    int modelo[3]; // t entre 0 y t días, zona inicial y final entre 0 y n-1
} Virus; // esta estructura guardará los datos iniciales

// Funciones de lectura del archivo y liberación de memoria para la estructura
int leer_archivo(const char* txt, Virus* virus);
void liberar_virus(Virus* virus);

#endif
