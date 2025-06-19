/*
Inserir os seguintes elementos 15, 27, 49, 10, 8, 67, 59, 9, 13, 20 e 14 em uma árvore binária de busca (AVL) e balancear sempre que necessário. Em seguida, excluir os elementos 9, 20, 27 e 8 mantendo a árvore balanceada. Desenhe apenas as árvores quando ocorrer o desbalanceamento e após realizar o balanceamento.
*/

#include <stdio.h>
#include <stdlib.h>

// --- Estrutura do Nó da Árvore ---
typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

// --- Estados para a "Máquina de Estados" da AVL ---
typedef enum {
    STATE_BALANCED = 0,
    STATE_LEFT_LEFT,
    STATE_RIGHT_RIGHT,
    STATE_LEFT_RIGHT,
    STATE_RIGHT_LEFT
} TreeState;

// --- Protótipos das Funções ---
Node* insert_AVL(Node* node, int data);
Node* delete_AVL(Node* node, int data);

// --- Funções Auxiliares ---

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if(newNode == NULL) {
        printf("Nao foi possivel criar um no por falta de memoria.");
        exit(1);
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

int height(Node* node) {
    return (node == NULL) ? 0 : node->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

// Fator de balanceamento
int getBalance(Node* node) {
    return (node == NULL) ? 0 : height(node->left) - height(node->right);
}

Node* findMinValueNode(Node* node) {
    Node* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}


// --- Funções de Rotação ---

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    // Rotação
    x->right = y;
    y->left = T2;
    // Atualiza alturas
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x; 
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    // Rotação
    y->left = x;
    x->right = T2;
    // Atualiza alturas
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y; 
}


// --- Lógica da Máquina de Estados (FSM) ---

// Verifica o estado para uma operação de INSERÇÃO
TreeState check_insert_state(Node* node, int data) {
    int balance = getBalance(node);

    if (balance > 1 && data < node->left->data) return STATE_LEFT_LEFT;
    if (balance < -1 && data > node->right->data) return STATE_RIGHT_RIGHT;
    if (balance > 1 && data > node->left->data) return STATE_LEFT_RIGHT;
    if (balance < -1 && data < node->right->data) return STATE_RIGHT_LEFT;
    
    return STATE_BALANCED;
}

// Verifica o estado para uma operação de DELEÇÃO
TreeState check_delete_state(Node* node) {
    int balance = getBalance(node);

    // Para LL e LR, a decisão é baseada no balanceamento do filho esquerdo
    if (balance > 1) {
        return (getBalance(node->left) >= 0) ? STATE_LEFT_LEFT : STATE_LEFT_RIGHT;
    }
    // Para RR e RL, a decisão é baseada no balanceamento do filho direito
    if (balance < -1) {
        return (getBalance(node->right) <= 0) ? STATE_RIGHT_RIGHT : STATE_RIGHT_LEFT;
    }

    return STATE_BALANCED;
}


// --- Funções Principais com a Lógica FSM Integrada ---

Node* insert_AVL(Node* node, int data) {
    // 1. Inserção normal 
    if (node == NULL) return createNode(data);
    if (data < node->data) {
        node->left = insert_AVL(node->left, data);
    } else if (data > node->data) {
        node->right = insert_AVL(node->right, data);
    } else {
        return node;
    }

    // --- Início da lógica FSM no retorno da recursão ---
    
    // Atualiza a altura do nó atual
    node->height = 1 + max(height(node->left), height(node->right));

    // Obtém o estado atual do nó
    TreeState state = check_insert_state(node, data);
    
    // Se não está balanceado, aplica a transição (rotação)
    if (state != STATE_BALANCED) {
        printf("\n!!! Desbalanceamento detectado ao inserir %d. No raiz da sub-arvore desbalanceada: %d -> BALANCEANDO", data, node->data);
    }

    switch (state) {
        case STATE_LEFT_LEFT:
            node = rightRotate(node);
            break;
        case STATE_RIGHT_RIGHT:
            node = leftRotate(node);
            break;
        case STATE_LEFT_RIGHT:
            node->left = leftRotate(node->left);
            node = rightRotate(node);
            break;
        case STATE_RIGHT_LEFT:
            node->right = rightRotate(node->right);
            node = leftRotate(node);
            break;
        case STATE_BALANCED:
        default:
             // Não faz nada se já estiver balanceado
            break;
    }

    return node;
}


Node* delete_AVL(Node* node, int data) {
    // 1. Deleção normal 
    if (node == NULL) return node;

    if (data < node->data) {
        node->left = delete_AVL(node->left, data);
    } else if (data > node->data) {
        node->right = delete_AVL(node->right, data);
    } else {
        if ((node->left == NULL) || (node->right == NULL)) {
            Node* temp = node->left ? node->left : node->right;
            if (temp == NULL) {
                temp = node;
                node = NULL;
            } else {
                *node = *temp;
            }
            free(temp);
        } else {
            Node* temp = findMinValueNode(node->right);
            node->data = temp->data;
            node->right = delete_AVL(node->right, temp->data);
        }
    }

    if (node == NULL) return node;
    
    // --- Início da lógica FSM no retorno da recursão ---

    node->height = 1 + max(height(node->left), height(node->right));
    TreeState state = check_delete_state(node);
    
    if (state != STATE_BALANCED) {
        printf("\n!!! Desbalanceamento detectado apos deletar %d. No raiz da sub-arvore desbalanceada: %d -> BALANCEANDO\n", data, node->data);
    }

    switch (state) {
        case STATE_LEFT_LEFT:
            node = rightRotate(node);
            break;
        case STATE_RIGHT_RIGHT:
            node = leftRotate(node);
            break;
        case STATE_LEFT_RIGHT:
            node->left = leftRotate(node->left);
            node = rightRotate(node);
            break;
        case STATE_RIGHT_LEFT:
            node->right = rightRotate(node->right);
            node = leftRotate(node);
            break;
        case STATE_BALANCED:
        default:
            break;
    }

    return node;
}


void freeTree(Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}


int main() {
    Node* root = NULL;
    int elements_to_insert[] = {15, 27, 49, 10, 8, 67, 59, 9, 13, 20, 14};
    int num_elements_insert = sizeof(elements_to_insert) / sizeof(elements_to_insert[0]);

    printf("======================================\n     INICIANDO INSERCAO\n======================================\n");
    for (int i = 0; i < num_elements_insert; i++) {
        printf("\n--> Inserindo o elemento: %d\n", elements_to_insert[i]);
        root = insert_AVL(root, elements_to_insert[i]);
    }

    int elements_to_delete[] = {9, 20, 27, 8};
    int num_elements_delete = sizeof(elements_to_delete) / sizeof(elements_to_delete[0]);
    
    printf("\n\n======================================\n     INICIANDO DELECAO\n======================================\n");
    for (int i = 0; i < num_elements_delete; i++) {
        printf("\n--> Deletando o elemento: %d\n", elements_to_delete[i]);
        root = delete_AVL(root, elements_to_delete[i]);
    }
    
    freeTree(root);
    return 0;
}