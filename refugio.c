#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nombre[31];
    int edad;
    char habilidad[31];
    int salud;
} Superviviente;

typedef struct {
    Superviviente *lista;
    int n;
    int capacidad;
} Refugio;

void inicializar_refugio(Refugio *r) {
    if (r == NULL) {
        return;
    }
    r->lista = NULL;
    r->n = 0;
    r->capacidad = 0;
}

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

static void quitar_salto_linea(char *cadena) {
    size_t len = strlen(cadena);
    if (len > 0 && cadena[len - 1] == '\n') {
        cadena[len - 1] = '\0';
    }
}

static int solo_espacios_o_vacio(const char *cadena) {
    if (cadena[0] == '\0') {
        return 1;
    }
    for (size_t i = 0; cadena[i] != '\0'; i++) {
        if (cadena[i] != ' ' && cadena[i] != '\t' && cadena[i] != '\r') {
            return 0;
        }
    }
    return 1;
}

int agregar_superviviente(Refugio *r) {
    if (r == NULL) {
        return 0;
    }

    Superviviente nuevo;
    memset(&nuevo, 0, sizeof(Superviviente));

    printf("Nombre del superviviente: ");
    if (fgets(nuevo.nombre, sizeof(nuevo.nombre), stdin) == NULL) {
        printf("Error al leer el nombre.\n");
        return 0;
    }
    quitar_salto_linea(nuevo.nombre);
    if (strchr(nuevo.nombre, '\0') == nuevo.nombre + sizeof(nuevo.nombre) - 1 &&
        strlen(nuevo.nombre) == sizeof(nuevo.nombre) - 1) {
        /* Si se llenaron los 30 caracteres exactos, limpiamos el resto de la línea si lo hubiera */
        limpiar_buffer();
    }

    if (solo_espacios_o_vacio(nuevo.nombre)) {
        printf("Nombre invalido: no puede estar vacio.\n");
        return 0;
    }

    printf("Edad: ");
    if (scanf("%d", &nuevo.edad) != 1 || nuevo.edad <= 0) {
        printf("Edad invalida: debe ser mayor que 0.\n");
        limpiar_buffer();
        return 0;
    }
    limpiar_buffer();

    printf("Habilidad especial: ");
    if (fgets(nuevo.habilidad, sizeof(nuevo.habilidad), stdin) == NULL) {
        printf("Error al leer la habilidad especial.\n");
        return 0;
    }
    quitar_salto_linea(nuevo.habilidad);
    if (strlen(nuevo.habilidad) == sizeof(nuevo.habilidad) - 1) {
        limpiar_buffer();
    }

    printf("Salud (0 a 100): ");
    if (scanf("%d", &nuevo.salud) != 1 || nuevo.salud < 0 || nuevo.salud > 100) {
        printf("Salud invalida: debe estar entre 0 y 100.\n");
        limpiar_buffer();
        return 0;
    }
    limpiar_buffer();

    if (r->capacidad == 0 || r->lista == NULL) {
        int nueva_capacidad = 4;
        Superviviente *nueva_lista = malloc((size_t)nueva_capacidad * sizeof(Superviviente));
        if (nueva_lista == NULL) {
            printf("Error: no se pudo reservar memoria inicial con malloc.\n");
            return 0;
        }
        r->lista = nueva_lista;
        r->capacidad = nueva_capacidad;
        r->n = 0;
    } else if (r->n >= r->capacidad) {
        int nueva_capacidad = r->capacidad * 2;
        Superviviente *temp = realloc(r->lista, (size_t)nueva_capacidad * sizeof(Superviviente));
        if (temp == NULL) {
            printf("Error: no se pudo ampliar memoria con realloc. Se conservan los registros previos.\n");
            return 0;
        }
        r->lista = temp;
        r->capacidad = nueva_capacidad;
    }

    r->lista[r->n] = nuevo;
    r->n++;
    return 1;
}

void listar_supervivientes(const Refugio *r) {
    if (r == NULL || r->n <= 0 || r->lista == NULL) {
        printf("No hay supervivientes registrados en el refugio.\n");
        return;
    }

    printf("\n--- Lista de supervivientes (%d) ---\n", r->n);
    for (int i = 0; i < r->n; i++) {
        printf("%d. Nombre: %s | Edad: %d | Habilidad: %s | Salud: %d\n",
               i + 1,
               r->lista[i].nombre,
               r->lista[i].edad,
               r->lista[i].habilidad,
               r->lista[i].salud);
    }
    printf("--------------------------------------\n");
}
   int main(void) {
    Refugio refugio;
    inicializar_refugio(&refugio);
    
    listar_supervivientes(&refugio);

    agregar_superviviente(&refugio);
    listar_supervivientes(&refugio);

    free(refugio.lista);
    refugio.lista = NULL;
    return 0;
}
