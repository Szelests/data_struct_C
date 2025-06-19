#include "tree_template.h"

// === Function to create a new Node ===
Node* createNode(int data)
{
    Node* newNode = (Node*)malloc(sizeof(Node));

    if(newNode == NULL) 
    {
        printf("Wasn't possible create a new Node due lacking of memory.");
        exit(1);
    }

    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1; // Optional: Initialize height to 1 for AVL trees

    return newNode;
}

//=== Functions to walk trhought tree ===
void inOrderTraversal(Node* root)
{
    if(root != NULL) 
    {
        inOrderTraversal(root->left);
        printf("%d ", root->data);
        inOrderTraversal(root->right);
    }
}

void preOrderTraversal(Node* root)
{
    if(root != NULL) 
    {
        printf("%d ", root->data);
        preOrderTraversal(root->left);
        preOrderTraversal(root->right);
    }
}

void postOrderTraversal(Node* root)
{
    if(root != NULL) 
    {
        postOrderTraversal(root->left);
        postOrderTraversal(root->right);
        printf("%d ", root->data);
    }
}

// === Mainly Functions ===

Node* insert(Node* root, int data)
{
    // Se a árvore (ou subárvore) estiver vazia, cria um novo nó e retorna com a raiz.
    if(root == NULL) return createNode(data);

    // Caso contrário percorre a árvore recursivamente
    if(data < root->data) 
    {
        root->left = insert(root->left, data);
    } else if (data > root->data) 
    {
        root->right = insert(root->right, data);
    }

    // Se data == root->data, não faz nada para evitar duplicatas

    return root;
}

Node* search(Node* root, int data)
{
    // Caso a raiz
    if(root == NULL || root->data == data) return root;

    // Se o valor for maior que o da raiz, busca na subárvore direita
    if(data > root->data)
    {
        return search(root->right, data);
    }

    // Se o valor for menor que o da raiz, busca na subárvore esquerda
    if(data < root->data) 
    {
        return search(root->left, data);
    }
}

Node* findMinValue(Node* node)
{
    Node* current = node;

    while(current &&  current->left != NULL) 
    {
        current = current->left;
    }

    return current;
}

Node* findMaxValue(Node* node)
{
    Node* current = node;

    while(current && current->right != NULL)
    {
        current = current->right;
    }

    return current;
}

Node* deleteNode(Node* root, int data) {
    // Caso base: se a árvore estiver vazia
    if (root == NULL) {
        return root;
    }

    // Encontra o nó a ser removido
    if (data < root->data) {
        root->left = deleteNode(root->left, data);
    } else if (data > root->data) {
        root->right = deleteNode(root->right, data);
    }
    // Se o dado for igual ao da raiz, este é o nó a ser deletado
    else {
        // Caso 1: Nó com apenas um filho ou nenhum filho
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }

        // Caso 2: Nó com dois filhos
        // Pega o sucessor em ordem (menor nó da subárvore direita)
        Node* temp = findMinValueNode(root->right);

        // Copia o valor do sucessor para este nó
        root->data = temp->data;

        // Deleta o sucessor da subárvore direita
        root->right = deleteNode(root->right, temp->data);
    }
    return root;
}

/*
   ============================================================
   === INÍCIO DO COMPLEMENTO PARA ÁRVORE BINÁRIA AVL ===
   ============================================================
*/

int height(Node* node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

int getBalanceFactor(Node* node) {
    if (node == NULL) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

// --- AVL Tree Rotations --

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Realiza a rotação
    x->right = y;
    y->left = T2;

    // Atualiza as alturas
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Retorna o novo root
    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Realiza a rotação
    y->left = x;
    x->right = T2;

    // Atualiza as alturas
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Retorna o novo root
    return y;
}

// --- ESSENTIAL AVL FUNCTIONS ---

Node* insertAVL(Node* node, int data) {
    // Passo 1: Realiza a inserção normal
    if (node == NULL) {
        return createNode(data);
    }

    if (data < node->data) {
        node->left = insertAVL(node->left, data);
    } else if (data > node->data) {
        node->right = insertAVL(node->right, data);
    } else {
        // Duplicatas não são permitidas
        return node;
    }

    // Passo 2: Atualiza a altura deste ancestral
    node->height = 1 + max(height(node->left), height(node->right));

    // Passo 3: Obtém o fator de balanceamento deste ancestral para verificar se ele está desequilibrado
    int balance = getBalanceFactor(node);

    // Se o nó ficar desequilibrado, então existem 4 casos

    // Caso Esquerda-Esquerda
    if (balance > 1 && data < node->left->data) {
        return rightRotate(node);
    }

    // Caso Direita-Direita
    if (balance < -1 && data > node->right->data) {
        return leftRotate(node);
    }

    // Caso Esquerda-Direita
    if (balance > 1 && data > node->left->data) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Caso Direita-Esquerda
    if (balance < -1 && data < node->right->data) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Retorna o ponteiro do nó (não alterado)
    return node;
}

// Função de remoção para a árvore AVL
Node* deleteNodeAVL(Node* root, int data)
{
    // 1. Executa a remoção padrão de uma ABB
    if (root == NULL)
        return root;

    if (data < root->data)
        root->left = deleteNodeAVL(root->left, data);
    else if(data > root->data)
        root->right = deleteNodeAVL(root->right, data);
    else
    {
        // Nó com um filho ou nenhum filho
        if( (root->left == NULL) || (root->right == NULL) )
        {
            Node *temp = root->left ? root->left : root->right;

            // Nenhum filho
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // Um filho
             *root = *temp; // Copia o conteúdo do filho não-vazio
            free(temp);
        }
        else
        {
            // Nó com dois filhos: pega o sucessor em ordem (menor da subárvore direita)
            Node* temp = findMinValue(root->right);
            root->data = temp->data;
            root->right = deleteNodeAVL(root->right, temp->data);
        }
    }

    // Se a árvore tinha apenas um nó, retorna
    if (root == NULL)
      return root;

    // 2. Atualiza a altura do nó atual
    root->height = 1 + max(height(root->left), height(root->right));

    // 3. Calcula o fator de balanceamento
    int balance = getBalance(root);

    // Se o nó ficou desbalanceado, existem 4 casos:

    // Caso Esquerda-Esquerda (LL)
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Caso Esquerda-Direita (LR)
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }

    // Caso Direita-Direita (RR)
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Caso Direita-Esquerda (RL)
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void freeTree(Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}