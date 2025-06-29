#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// =================== ESTRUTURAS ===================

/**
 * @brief Estrutura para armazenar a lista de passageiros de um voo (árvore binária).
 */
typedef struct tree {
    char name[40];
    struct tree *left, *right;
} PassengersList;

/**
 * @brief Estrutura para armazenar as informações de um voo.
 */
typedef struct node {
    struct node *prox;
    char Id[8];
    char Destino[4];
    char Empresa[20];
    char Registro[9];
    char Modelo[20];
    uint32_t Assentos;
    PassengersList* lp;
} Voo;

/**
 * @brief Estrutura para gerenciar a fila de voos.
 */
typedef struct queue {
    Voo *front;
    Voo *rear;
} QueuePlane;

/**
 * @brief Enum para os estados da máquina de estados do aeroporto.
 */
typedef enum estados {
    START = 0,
    CADASTRAR_VOO = 1,
    AUTORIZAR_DECOLAGEM = 2,
    LISTAR_1_VOO = 3,
    LISTAR_ALL_VOOS = 4,
    CONTAR_VOOS = 5,
    CADASTRAR_PASSAGEIRO = 6,
    REMOVER_PASSAGEIRO = 7,
    LISTAR_PASSAGEIROS = 8,
    SAIR = 9
} AIRPORT_STATES_en;

// =================== FUNCTIONS DECLARATIONS ===================

// --- voo functions ---
void cadastrarVoo(QueuePlane* queue);
void autorizarDecolagem(QueuePlane* queue);
void listarPrimeiroVoo(QueuePlane* queue);
void listarTodosVoos(QueuePlane* queue);
void contarVoos(QueuePlane* queue);
void liberarFila(QueuePlane** queue);

// --- passenger functions ---
PassengersList* cadastrarPassageiro(PassengersList* root, char* name, uint32_t* total, uint32_t max);
PassengersList* removerPassageiro(PassengersList* root, char* name, uint32_t* total);
void listarPassageiros(PassengersList* root);
void liberarPassageiros(PassengersList* root);
void gerenciarCadastroPassageiro(QueuePlane* queue);
void gerenciarRemocaoPassageiro(QueuePlane* queue);
void gerenciarListagemPassageiros(QueuePlane* queue);

// --- auxiliary functions ---
void manage_fsm_airport(QueuePlane* queue, AIRPORT_STATES_en state);
AIRPORT_STATES_en get_airport_state(void);
Voo* encontrarVoo(QueuePlane* queue, const char* id);
PassengersList* encontrarMinimo(PassengersList* node);
void limparBuffer(void);

void salvarVooEmJson(Voo* voo);
void escreverPassageirosJson(FILE* file, PassengersList* root, int* isFirst);

// =================== FUNÇÃO PRINCIPAL ===================

/**
 * @brief Função principal que inicia o sistema de gerenciamento de voos.
 * Cria a fila de voos e gerencia o loop de estados da máquina de estados do aeroporto.
 */
int main() {
    QueuePlane* fila_de_avioes = (QueuePlane*)malloc(sizeof(QueuePlane));
    if (fila_de_avioes == NULL) {
        printf("Erro crítico de alocação de memória.\n");
        return 1;
    }
    fila_de_avioes->front = NULL;
    fila_de_avioes->rear = NULL;

    AIRPORT_STATES_en estado_atual;
    do {
        estado_atual = get_airport_state();
        manage_fsm_airport(fila_de_avioes, estado_atual);
    } while (estado_atual != SAIR);

    liberarFila(&fila_de_avioes);
    printf("Sistema encerrado. Memória liberada.\n");
    return 0;
}

// =================== MÁQUINA DE ESTADOS ===================

/**
 * @brief Gerencia a máquina de estados do aeroporto, executando ações baseadas no estado atual.
 * @param queue Ponteiro para a fila de voos.
 * @param state Estado atual da máquina de estados.
 */
void manage_fsm_airport(QueuePlane* queue, AIRPORT_STATES_en state) {
    switch(state) {
        case CADASTRAR_VOO: 
            cadastrarVoo(queue); 
            break;
        case AUTORIZAR_DECOLAGEM: 
            autorizarDecolagem(queue); 
            break;
        case LISTAR_1_VOO: 
            listarPrimeiroVoo(queue); 
            break;
        case LISTAR_ALL_VOOS: 
            listarTodosVoos(queue);
             break;
        case CONTAR_VOOS: 
            contarVoos(queue);
             break;
        case CADASTRAR_PASSAGEIRO: 
            gerenciarCadastroPassageiro(queue); 
            break;
        case REMOVER_PASSAGEIRO: 
            gerenciarRemocaoPassageiro(queue); 
            break;
        case LISTAR_PASSAGEIROS: 
            gerenciarListagemPassageiros(queue); 
            break;
        case SAIR: 
            printf("Iniciando processo de encerramento...\n"); 
            break;
        default: 
            printf("Opção inválida, tente novamente.\n"); 
            break;
    }
    printf("\nPressione Enter para continuar...");
    limparBuffer();
    getchar();
}

/**
 * @brief Exibe o menu de gerenciamento de voos e coleta a ação desejada pelo usuário.
 * @return Retorna o estado selecionado pelo usuário.
 */
AIRPORT_STATES_en get_airport_state(void) {
    int choice;
    system("clear || cls");
    printf("========= MENU DE GERENCIAMENTO DE VOOS =========\n");
    printf("1: Cadastrar novo voo\n");
    printf("2: Autorizar decolagem do próximo voo\n");
    printf("3: Listar informações do próximo voo\n");
    printf("4: Listar todos os voos na fila\n");
    printf("5: Contar voos na fila\n");
    printf("-------------------------------------------------\n");
    printf("6: Cadastrar passageiro em um voo\n");
    printf("7: Remover passageiro de um voo\n");
    printf("8: Listar passageiros de um voo\n");
    printf("-------------------------------------------------\n");
    printf("9: Sair\n");
    printf("=================================================\n");
    printf("SELECIONE A AÇÃO: ");

    if (scanf("%d", &choice) != 1) {
        limparBuffer();
        return -1;
    }
    limparBuffer();
    return (AIRPORT_STATES_en)choice;
}

// =================== FUNÇÕES DE VOO ===================

/**
 * @brief Coleta os dados de um novo voo, o adiciona à fila e cria seu arquivo JSON.
 * @param queue Ponteiro para a fila de voos a ser modificada.
 */
void cadastrarVoo(QueuePlane* queue) {
    Voo* newVoo = (Voo*)malloc(sizeof(Voo));
    if(newVoo == NULL) {
        printf("Não foi possível alocar memória para um novo voo.\n");
        return;
    }

    printf("\n--- CADASTRO DE NOVO VOO ---\n");
    printf("Registro da Aeronave (ex: PR-GUO, será o nome do arquivo): ");
    scanf("%8s", newVoo->Registro);
    printf("ID (ex: G3-1234): ");
    scanf("%7s", newVoo->Id);
    printf("Destino (3 letras, ex: CWB): ");
    scanf("%3s", newVoo->Destino);
    printf("Empresa: ");
    scanf("%19s", newVoo->Empresa);
    printf("Modelo da Aeronave: ");
    scanf("%19s", newVoo->Modelo);

    printf("Quantidade de assentos: ");
    while (scanf("%u", &newVoo->Assentos) != 1 || newVoo->Assentos < 1) {
        printf("Entrada inválida. Digite um número de assentos maior que 0: ");
        limparBuffer();
    }
    limparBuffer();

    newVoo->lp = NULL;
    newVoo->prox = NULL;

    if(queue->front == NULL) {
        queue->front = newVoo;
        queue->rear = newVoo;
    } else {
        queue->rear->prox = newVoo;
        queue->rear = newVoo;
    }
    printf("\nVoo %s cadastrado com sucesso!\n", newVoo->Id);

    salvarVooEmJson(newVoo);
}

/**
 * @brief Simula decolagem, removendo o voo da fila, sua memória e seu arquivo JSON.
 * @param queue Ponteiro para a fila de voos.
 */
void autorizarDecolagem(QueuePlane* queue) {
    if (queue->front == NULL) {
        printf("Nenhum voo na fila para decolagem.\n");
        return;
    }

    Voo* decolando = queue->front;
    printf("\nAutorizando decolagem do Voo %s, destino %s.\n", decolando->Id, decolando->Destino);

    char filename[20];
    sprintf(filename, "%s.json", decolando->Registro);
    if (remove(filename) == 0) {
        printf("Arquivo '%s' de registro do voo foi removido.\n", filename);
    } else {
        perror("Erro ao remover o arquivo do voo");
    }

    queue->front = queue->front->prox;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    liberarPassageiros(decolando->lp);
    free(decolando);

    printf("Voo decolou. Fila e arquivos atualizados.\n");
}

/**
 * @brief Exibe o próximo voo na fila e suas informações.
 * @param queue Ponteiro para a fila de voos.
 */
void listarPrimeiroVoo(QueuePlane* queue) {
    if (queue->front == NULL) {
        printf("Nenhum voo na fila.\n");
        return;
    }
    Voo* p = queue->front;
    printf("\n--- PRÓXIMO VOO A DECOLAR ---\n");
    printf("ID: %s\nRegistro: %s\nDestino: %s\nEmpresa: %s\nAeronave: %s\nAssentos: %u\n",
        p->Id, p->Registro, p->Destino, p->Empresa, p->Modelo, p->Assentos);
}

/**
 * @brief Lista todos os voos na fila, exibindo suas informações.
 * @param queue Ponteiro para a fila de voos.
 */
void listarTodosVoos(QueuePlane* queue) {
    if (queue->front == NULL) {
        printf("Fila de voos está vazia.\n");
        return;
    }
    Voo* atual = queue->front;
    int pos = 1;
    printf("\n--- FILA DE VOOS PARA DECOLAGEM ---\n");
    while (atual != NULL) {
        printf("%d. Voo %s | Registro: %s | Destino: %s\n", pos++, atual->Id, atual->Registro, atual->Destino);
        atual = atual->prox;
    }
}

/**
 * @brief Conta o número de voos na fila e exibe a quantidade.
 * @param queue Ponteiro para a fila de voos.
 */
void contarVoos(QueuePlane* queue) {
    int count = 0;
    Voo* atual = queue->front;
    while(atual != NULL) {
        count++;
        atual = atual->prox;
    }
    if (count == 0)
        printf("Não há voos na fila.\n");
    else if (count == 1)
        printf("Há 1 voo na fila.\n");
    else
        printf("Há %d voos na fila.\n", count);
}

/**
 * @brief Libera toda a memória alocada para a fila de voos e seus elementos.
 * @param queue Ponteiro duplo para a fila de voos, que será setado como NULL após a liberação.
 */
void liberarFila(QueuePlane** queue) {
    if (queue == NULL || *queue == NULL) return;
    Voo* atual = (*queue)->front;
    while(atual != NULL) {
        Voo* temp = atual;
        atual = atual->prox;
        liberarPassageiros(temp->lp);
        free(temp);
    }
    free(*queue);
    *queue = NULL;
}

// =================== FUNÇÕES DE PASSAGEIROS ===================

/**
 * @brief Cadastra um novo passageiro na árvore binária de passageiros do voo.
 * Protege contra buffer overflow e excesso de passageiros.
 * @param root Raiz da árvore de passageiros.
 * @param name Nome do passageiro a ser cadastrado.
 * @param total Ponteiro para o total de passageiros já cadastrados.
 * @param max Número máximo de assentos disponíveis.
 * @return Retorna a nova raiz da árvore (pode ser a mesma ou um novo nó).
 */
PassengersList* cadastrarPassageiro(PassengersList* root, char* name, uint32_t* total, uint32_t max) {
    if (*total >= max) {
        printf("Limite de passageiros atingido para este voo.\n");
        return root;
    }
    if (root == NULL) {
        PassengersList* newP = (PassengersList*)malloc(sizeof(PassengersList));
        if (newP == NULL) {
            printf("Falha na alocação.\n");
            return root;
        }
        strncpy(newP->name, name, sizeof(newP->name) - 1);
        newP->name[sizeof(newP->name) - 1] = '\0';
        newP->left = newP->right = NULL;
        (*total)++;
        return newP;
    }
    if (strcasecmp(name, root->name) < 0)
        root->left = cadastrarPassageiro(root->left, name, total, max);
    else if (strcasecmp(name, root->name) > 0)
        root->right = cadastrarPassageiro(root->right, name, total, max);
    return root;
}

/**
 * @brief Remove um passageiro da árvore binária de passageiros do voo.
 * @param root Raiz da árvore de passageiros.
 * @param name Nome do passageiro a ser removido.
 * @param total Ponteiro para o total de passageiros já cadastrados.
 * @return Retorna a nova raiz da árvore (pode ser a mesma ou um novo nó).
 */
PassengersList* removerPassageiro(PassengersList* root, char* name, uint32_t* total) {
    if (root == NULL) return root;
    if (strcasecmp(name, root->name) < 0)
        root->left = removerPassageiro(root->left, name, total);
    else if (strcasecmp(name, root->name) > 0)
        root->right = removerPassageiro(root->right, name, total);
    else {
        if (root->left == NULL) {
            PassengersList* temp = root->right;
            free(root);
            (*total)--;
            return temp;
        } else if (root->right == NULL) {
            PassengersList* temp = root->left;
            free(root);
            (*total)--;
            return temp;
        }
        PassengersList* temp = encontrarMinimo(root->right);
        strncpy(root->name, temp->name, sizeof(root->name) - 1);
        root->name[sizeof(root->name) - 1] = '\0';
        root->right = removerPassageiro(root->right, temp->name, total);
    }
    return root;
}

/**
 * @brief Lista todos os passageiros em ordem alfabética.
 * @param root Raiz da árvore de passageiros.
 */
void listarPassageiros(PassengersList* root) {
    if (root != NULL) {
        listarPassageiros(root->left);
        printf("- %s\n", root->name);
        listarPassageiros(root->right);
    }
}

/**
 * @brief Libera toda a memória alocada para a árvore de passageiros.
 * @param root Raiz da árvore de passageiros.
 */
void liberarPassageiros(PassengersList* root) {
    if (root != NULL) {
        liberarPassageiros(root->left);
        liberarPassageiros(root->right);
        free(root);
    }
}

/**
 * @brief Gerencia o cadastro de um passageiro em um voo.
 * Protege contra excesso de passageiros e buffer overflow.
 * @param queue Ponteiro para a fila de voos.
 */
void gerenciarCadastroPassageiro(QueuePlane* queue) {
    char idVoo[8];
    printf("Digite o ID do voo para adicionar o passageiro: ");
    scanf("%7s", idVoo);

    Voo* vooAlvo = encontrarVoo(queue, idVoo);
    if (vooAlvo == NULL) {
        printf("Voo com ID %s não encontrado.\n", idVoo);
        return;
    }

    uint32_t total = 0;
    // Conta passageiros já cadastrados
    void contar(PassengersList* r) { if (r) { total++; contar(r->left); contar(r->right); } }
    contar(vooAlvo->lp);

    if (total >= vooAlvo->Assentos) {
        printf("Todos os assentos deste voo já estão ocupados.\n");
        return;
    }

    char nomePassageiro[40];
    printf("Digite o nome completo do passageiro: ");
    limparBuffer();
    fgets(nomePassageiro, sizeof(nomePassageiro), stdin);
    nomePassageiro[strcspn(nomePassageiro, "\n")] = 0;

    vooAlvo->lp = cadastrarPassageiro(vooAlvo->lp, nomePassageiro, &total, vooAlvo->Assentos);
    printf("Passageiro '%s' cadastrado no voo %s.\n", nomePassageiro, idVoo);

    salvarVooEmJson(vooAlvo);
}

/**
 * @brief Gerencia a remoção de um passageiro de um voo.
 * @param queue Ponteiro para a fila de voos.
 */
void gerenciarRemocaoPassageiro(QueuePlane* queue) {
    char idVoo[8];
    printf("Digite o ID do voo para remover o passageiro: ");
    scanf("%7s", idVoo);

    Voo* vooAlvo = encontrarVoo(queue, idVoo);
    if (vooAlvo == NULL) {
        printf("Voo com ID %s não encontrado.\n", idVoo);
        return;
    }

    uint32_t total = 0;
    void contar(PassengersList* r) { if (r) { total++; contar(r->left); contar(r->right); } }
    contar(vooAlvo->lp);

    char nomePassageiro[40];
    printf("Digite o nome completo do passageiro a ser removido: ");
    limparBuffer();
    fgets(nomePassageiro, sizeof(nomePassageiro), stdin);
    nomePassageiro[strcspn(nomePassageiro, "\n")] = 0;

    vooAlvo->lp = removerPassageiro(vooAlvo->lp, nomePassageiro, &total);
    printf("Tentativa de remoção concluída.\n");

    salvarVooEmJson(vooAlvo);
}

/**
 * @brief Gerencia a listagem de passageiros de um voo.
 * @param queue Ponteiro para a fila de voos.
 */
void gerenciarListagemPassageiros(QueuePlane* queue) {
    char idVoo[8];
    printf("Digite o ID do voo para listar os passageiros: ");
    scanf("%7s", idVoo);

    Voo* vooAlvo = encontrarVoo(queue, idVoo);
    if (vooAlvo == NULL) {
        printf("Voo com ID %s não encontrado.\n", idVoo);
        return;
    }

    printf("\n--- LISTA DE PASSAGEIROS DO VOO %s ---\n", idVoo);

    if (vooAlvo->lp == NULL) {
        printf("Não há passageiros cadastrados neste voo.\n");
    } else {
        listarPassageiros(vooAlvo->lp);
    }
}

// =================== FUNÇÕES AUXILIARES ===================

/**
 * @brief Busca um voo pelo ID na fila.
 * @param queue Ponteiro para a fila de voos.
 * @param id ID do voo a ser buscado.
 * @return Ponteiro para o voo encontrado ou NULL.
 */
Voo* encontrarVoo(QueuePlane* queue, const char* id) {
    Voo* atual = queue->front;
    while (atual != NULL) {
        if (strcmp(id, atual->Id) == 0)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

/**
 * @brief Encontra o nó com o menor valor (nome) na árvore de passageiros.
 * @param node Raiz da árvore.
 * @return Ponteiro para o nó mínimo.
 */
PassengersList* encontrarMinimo(PassengersList* node) {
    PassengersList* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

/**
 * @brief Limpa o buffer de entrada para evitar problemas com scanf/fgets.
 */
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// =================== FUNÇÕES DE PERSISTÊNCIA ===================

/**
 * @brief Salva os dados de um voo, incluindo sua lista de passageiros, em um arquivo JSON.
 * O nome do arquivo é o registro do voo (ex: PR-GUO.json).
 * @param voo Ponteiro para o voo que será salvo.
 */
void salvarVooEmJson(Voo* voo) {
    if (voo == NULL) return;

    char filename[20];
    snprintf(filename, sizeof(filename), "%s.json", voo->Registro);

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("ERRO: Não foi possível criar o arquivo %s\n", filename);
        return;
    }

    fprintf(file, "{\n");
    fprintf(file, "  \"registro\": \"%s\",\n", voo->Registro);
    fprintf(file, "  \"id_voo\": \"%s\",\n", voo->Id);
    fprintf(file, "  \"destino\": \"%s\",\n", voo->Destino);
    fprintf(file, "  \"empresa\": \"%s\",\n", voo->Empresa);
    fprintf(file, "  \"modelo_aeronave\": \"%s\",\n", voo->Modelo);
    fprintf(file, "  \"assentos\": %u,\n", voo->Assentos);
    fprintf(file, "  \"passageiros\": [\n");

    int isFirst = 1;
    escreverPassageirosJson(file, voo->lp, &isFirst);

    fprintf(file, "\n  ]\n");
    fprintf(file, "}\n");

    fclose(file);
    printf("Dados do voo %s salvos/atualizados em '%s'.\n", voo->Id, filename);
}

/**
 * @brief (Recursivo) Escreve os nomes dos passageiros em um array JSON dentro do arquivo.
 * @param file Ponteiro para o arquivo aberto.
 * @param root Raiz da árvore de passageiros.
 * @param isFirst Ponteiro para um flag que indica se este é o primeiro passageiro a ser escrito.
 */
void escreverPassageirosJson(FILE* file, PassengersList* root, int* isFirst) {
    if (root == NULL) return;

    escreverPassageirosJson(file, root->left, isFirst);

    if (*isFirst) {
        fprintf(file, "    \"%s\"", root->name);
        *isFirst = 0;
    } else {
        fprintf(file, ",\n    \"%s\"", root->name);
    }

    escreverPassageirosJson(file, root->right, isFirst);
}