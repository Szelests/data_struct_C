#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// =========== STRUCTS / VARIABLES / DECLARATIONS ==========

// Tree Struct to insert passangers
typedef struct tree 
{
    char name[40];
    struct tree *left, *right;
} PassangersList;


// Plane node
typedef struct node 
{
    struct node *prox;
    char Id[8];
    char Destino[4];
    char Empresa[20];
    char Registro[9];
    char Modelo[20];
    uint32_t Assentos;
    PassangersList* lp;
} Voo;

// Plane Queue
typedef struct queue 
{
    struct queue *front;
    struct queue *rear;
} QueuePlane;

// --- Enum de estados ---
typedef enum estados 
{
    START = 0,

    // VOOS
    CADASTRAR_VOO,
    AUTORIZAR_DECOLAGEM,
    LSITAR_1_VOO,
    LISTAR_ALL_VOOS,
    CONTAR_VOOS,
    LIBERAR_VOOS,

    // PASSAGEIROS
    CADASTRAR_PASSAGEIRO,
    REMOVER_PASSAGEIRO,
    LISTAR_PASSAGEIROS,
    LIBERAR_PASSAGEIROS,

    DEFAULT,
}AIRPORT_STATES_en;

// --- Métodos da QueuePlane ---
void cadastrarVoo(QueuePlane* queue);
void autorizarDecolagem(QueuePlane* queue);
void listarPrimeiroVoo(QueuePlane* queue);
void listarTodosVoos(QueuePlane* queue);
void contarVoos(QueuePlane* queue);
void liberarFila(QueuePlane* queue);


// --- Métodos do PassangerList ---
void cadastrarPassageiro(PassangersList* list);
void removerPassageiro(PassangersList* list);
void listarPassageiros(PassangersList* list);
void liberarPassageiros(PassangersList* list);


// --- Funções Essenciais ---
void manage_fsm_airport(QueuePlane* queue);
int get_airport_state(void);

void manage_fsm_airport(QueuePlane* queue)
{
    static AIRPORT_STATES_en airport_state = START;

    airport_state = get_airport_state();

    switch(airport_state)
    {
        // VOO
        case CADASTRAR_VOO:
            break;
        
        case AUTORIZAR_DECOLAGEM:
            break;
        
        case LSITAR_1_VOO:
            break;
        
        case LISTAR_ALL_VOOS:
            break;
        
        case CONTAR_VOOS:
            break;

        case LIBERAR_VOOS:
            break;
        
        // PASSAGEIROS

        case CADASTRAR_PASSAGEIRO:
            break;

        case REMOVER_PASSAGEIRO:
            break;
        
        case LISTAR_PASSAGEIROS:
            break;
        
        case LIBERAR_PASSAGEIROS:
            break;
        
        default:
            printf("BYE BYE");
            break;

    }


}

// ----- VOO functions
void cadastrarVoo(QueuePlane* queue)
{
    Voo* newVoo =(Voo*)malloc(sizeof(Voo));
    if(newVoo == NULL) 
    {
        printf("Não foi possível devido à falta de memória. \n");
        exit(1);
    }

    PassangersList* lista_de_passageiros;
    newVoo->lp = lista_de_passageiros;
    int assentos;
    char confirm;

    printf("INSIRA OS SEGUINTES DADOS DO VOO:\n");
    printf("ID:\n");
    scanf("%s", &newVoo->Id);
    printf("Destino:\n");
    scanf("%s", &newVoo->Destino);
    printf("Empresa: \n");
    scanf("%s", newVoo->Empresa);
    printf("Registro: \n");
    scanf("%s", newVoo->Registro);
    printf("Modelo: \n");
    scanf("%s", newVoo->Modelo);
    printf("Quantidade de assentos: (Mínimo 1 assento)\n");
    scanf("%s", assentos);
    while(assentos < 1) 
    {
        printf("Selecione uma quantidade maior que 0 de assentos.");
        scanf("%d", assentos);
    }
    newVoo->Assentos = assentos;

    printf("Gostaria de cadastrar passageiros? [N/Y]");
    if(confirm == "Y" || confirm == "y")
    {

    } else 
    {
        printf("Voltando ao menu...");
    }

    if(queue->front == NULL) 
    {
        queue->front = newVoo;
        queue->rear = newVoo;
    } else 
    {
        
    }

    manage_fsm_airport(queue);
}


// ----- PASSANGER functions

// ====== ESSENTIAL FUNCTIONS =====
int get_airport_state()
{
    int airport_state;

    printf("SELECIONE A AÇÃO:\n");
    printf("1: Cadastrar novo voo\n");
    printf("2: Cadastrar passageiro\n");
    printf("3: Remover passageiro\n");
    printf("4: Mostrar próximo voo\n");
    printf("5: Autorizar decolagem\n");
    printf("6: Listar aviões na fila\n");
    printf("7: Mostrar todos os voos\n");
    printf("8: Mostrar todos passageiros do voo\n");
    printf("Digite qualquer tecla para SAIR\n");
    scanf("%d", &airport_state);

    return (AIRPORT_STATES_en)airport_state;
}

// ===== MAIN =====
int main()
{
    QueuePlane* fila_de_avioes = (QueuePlane*)malloc(sizeof(QueuePlane));

    manage_fsm_airport(fila_de_avioes);
}

