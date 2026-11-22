#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RECURSO_AGUA 1
#define RECURSO_COMIDA 2
#define RECURSO_MEDICAMENTOS 3
#define RECURSO_MUNICIONES 4

typedef struct {
    char nombre[31];
    int edad;
    char habilidad[31];
    int salud;
} Superviviente;

typedef struct {
    int agua;
    int comida;
    int medicamentos;
    int municiones;
} Recursos;

typedef struct {
    Superviviente *lista;
    int n;
    int capacidad;
    Recursos recursos;
} Refugio;

void inicializar_refugio(Refugio *r) {
    if (r == NULL) {
        return;
    }
    r->lista = NULL;
    r->n = 0;
    r->capacidad = 0;
    r->recursos.agua = 20;
    r->recursos.comida = 20;
    r->recursos.medicamentos = 10;
    r->recursos.municiones = 15;
}

void mostrar_recursos(const Refugio *r) {
    if (r == NULL) {
        printf("Refugio no valido.\n");
        return;
    }

    printf("\n--- Recursos del refugio ---\n");
    printf("Agua:         %d\n", r->recursos.agua);
    printf("Comida:       %d\n", r->recursos.comida);
    printf("Medicamentos: %d\n", r->recursos.medicamentos);
    printf("Municiones:   %d\n", r->recursos.municiones);
    printf("................................\n");
}

int modificar_recurso(Refugio *r, int tipo, int cantidad) {
    if (r == NULL || cantidad == 0) {
        printf("Operacion invalida: refugio nulo o cantidad igual a 0.\n");
        return 0;
    }

    int *recurso_ptr = NULL;
    switch (tipo) {
        case RECURSO_AGUA:
            recurso_ptr = &r->recursos.agua;
            break;
        case RECURSO_COMIDA:
            recurso_ptr = &r->recursos.comida;
            break;
        case RECURSO_MEDICAMENTOS:
            recurso_ptr = &r->recursos.medicamentos;
            break;
        case RECURSO_MUNICIONES:
            recurso_ptr = &r->recursos.municiones;
            break;
        default:
            printf("Tipo de recurso invalido (%d).\n", tipo);
            return 0;
    }

    if (cantidad < 0) {
        int salida = -cantidad;
        if (salida > *recurso_ptr) {
            printf("Salida rechazada: disponible %d, solicitado %d.\n", *recurso_ptr, salida);
            return 0;
        }
        *recurso_ptr -= salida;
        return 1;
    }

    *recurso_ptr += cantidad;
    return 1;
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
    mostrar_recursos(&refugio);

    int res_entrada = modificar_recurso(&refugio, RECURSO_AGUA, 5);
    printf("Entrada agua (+5) -> %d\n", res_entrada);

    int res_salida_exacta = modificar_recurso(&refugio, RECURSO_MEDICAMENTOS, -10);
    printf("Salida exacta medicamentos (-10) -> %d\n", res_salida_exacta);

    int res_salida_exceso = modificar_recurso(&refugio, RECURSO_MEDICAMENTOS, -1);
    printf("Salida de mas medicamentos (-1) -> %d\n", res_salida_exceso);

    int res_tipo_invalido = modificar_recurso(&refugio, 99, 3);
    printf("Tipo invalido -> %d\n", res_tipo_invalido);

    int res_cantidad_cero = modificar_recurso(&refugio, RECURSO_AGUA, 0);
    printf("Cantidad 0 -> %d\n", res_cantidad_cero);

    mostrar_recursos(&refugio);

    free(refugio.lista);
    refugio.lista = NULL;
    return 0;
}