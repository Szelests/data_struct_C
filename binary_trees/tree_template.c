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