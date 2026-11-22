#include <stdio.h>
typedef struct {
    char nombre[30];
    int edad;
    char habilidad[30];
    int salud;
} superviviente;

typedef struct{
    superviviente *lista;
    int n;
    int capacidad;
} Refugio;

void inicializar_refugio(Refugio *r){
    if (r == NULL){
        return;
    }
    r->lista = NULL;
    r->n = 0;
    r->capacidad = 0;
}

int main(void){
    Refugio refugio;
    inicializar_refugio(&refugio);
    return 0;
}