/*
Faça um programa para executar as operações abaixo em uma árvore binária de busca de números inteiros (utilize uma função para cada item):

1. Inserir número
2. Mostrar todos os números (ordenados)
3. Mostrar o maior número
4. Mostrar o menor número
5. Mostrar a quantidade de nós da árvore
6. Mostrar a soma dos nós
7. Mostrar os filhos de um determinado nó (caso existam)
8. Mostrar a profundidade de um determinado nó, listando o caminho percorrido
9. Sair (remover a árvore antes de fechar o programa).
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// --- Tree Node Structure ---
typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;


// ===== Function Declarations =====

// Menu functions
Node* insert(Node* root, int data);             // 1. Insert
void showAll(Node* root);                       // 2. Show all
void showBiggest(Node* root);                   // 3. Show biggest
void showSmallest(Node* root);                  // 4. Show smallest
int countNodes(Node* root);                     // 5. Count nodes
int sumNodes(Node* root);                       // 6. Sum nodes
void showChildren(Node* root, int data);        // 7. Show children of a node
void showPath(Node* root, int data);            // 8. Show path and depth
void freeTree(Node* root);                      // 9. Free the tree from memory




// Helper functions
Node* createNode(int data);
Node* findNode(Node* root, int data); // Helper to find a node
Node* setupInitialTree(Node* root);     // Check if the user wanna create a new tree or not

// ===== Main Function =====

int main() {
    Node* root = NULL;
    int choice = 0;
    int num;

    root = setupInitialTree(root);

    while (choice != 9) {
        printf("\n\n--- BINARY TREE MENU ---\n");
        printf("1. Insert number\n");
        printf("2. Show all numbers (sorted)\n");
        printf("3. Show the biggest number\n");
        printf("4. Show the smallest number\n");
        printf("5. Show the count of nodes\n");
        printf("6. Show the sum of all nodes\n");
        printf("7. Show a node's children\n");
        printf("8. Show a node's path and depth\n");
        printf("9. Exit\n");
        printf("------------------------\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        printf("\n");

        switch (choice) {
            case 1:
                printf("Enter the number to insert: ");
                scanf("%d", &num);
                root = insert(root, num);
                printf("Number %d inserted successfully!\n", num);
                break;
            case 2:
                printf("Numbers in the tree (in order): ");
                if (root == NULL) {
                    printf("The tree is empty.");
                } else {
                    showAll(root);
                }
                printf("\n");
                break;
            case 3:
                showBiggest(root);
                break;
            case 4:
                showSmallest(root);
                break;
            case 5:
                printf("The tree has %d node(s).\n", countNodes(root));
                break;
            case 6:
                 printf("The sum of all nodes is: %d.\n", sumNodes(root));
                break;
            case 7:
                printf("Enter the node to see its children: ");
                scanf("%d", &num);
                showChildren(root, num);
                break;
            case 8:
                printf("Enter the node to see its path and depth: ");
                scanf("%d", &num);
                showPath(root, num);
                break;
            case 9:
                printf("Exiting and freeing tree memory...\n");
                freeTree(root);
                root = NULL; // Important: set root to null after freeing
                printf("Program finished.\n");
                break;
            default:
                printf("Invalid option. Please try again.\n");
        }
    }

    return 0;
}


// ===== Function Implementations =====

// Creates a new node
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Could not allocate memory.");
        exit(1);
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// 1. Insert an element
Node* insert(Node* root, int data) {
    if (root == NULL) return createNode(data);

    if (data < root->data) {
        root->left = insert(root->left, data);
    } else if (data > root->data) {
        root->right = insert(root->right, data);
    }
    // If data already exists, do nothing
    return root;
}

// 2. Show all numbers (in-order traversal)
void showAll(Node* root) {
    if (root != NULL) {
        showAll(root->left);
        printf("%d ", root->data);
        showAll(root->right);
    }
}

// 3. Show the biggest number
void showBiggest(Node* root) {
    if (root == NULL) {
        printf("The tree is empty.\n");
        return;
    }
    Node* current = root;
    while (current->right != NULL) {
        current = current->right;
    }
    printf("Biggest value: %d\n", current->data);
}

// 4. Show the smallest number
void showSmallest(Node* root) {
    if (root == NULL) {
        printf("The tree is empty.\n");
        return;
    }
    Node* current = root;
    while (current->left != NULL) {
        current = current->left;
    }
    printf("Smallest value: %d\n", current->data);
}

// 5. Count how many elements exist
int countNodes(Node* root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + countNodes(root->left) + countNodes(root->right);
}

// 6. Sum all elements
int sumNodes(Node* root) {
    if (root == NULL) {
        return 0;
    }
    return root->data + sumNodes(root->left) + sumNodes(root->right);
}

// Helper function to find a node by its value
Node* findNode(Node* root, int data) {
    if (root == NULL || root->data == data) {
        return root;
    }
    if (data < root->data) {
        return findNode(root->left, data);
    }
    return findNode(root->right, data);
}

// 7. Show the children of a specific node
void showChildren(Node* root, int data) {
    Node* parent = findNode(root, data);

    if (parent == NULL) {
        printf("Node %d not found in the tree.\n", data);
        return;
    }

    printf("Parent: %d\n", parent->data);
    
    if (parent->left != NULL) {
        printf("  Left Child: %d\n", parent->left->data);
    } else {
        printf("  Has no left child.\n");
    }

    if (parent->right != NULL) {
        printf("  Right Child: %d\n", parent->right->data);
    } else {
        printf("  Has no right child.\n");
    }
}


// 8. Show the depth and path of a node
void showPath(Node* root, int data) {
    if (root == NULL) {
        printf("The tree is empty.\n");
        return;
    }

    Node* current = root;
    int depth = 0;
    printf("Path to %d: ", data);

    while (current != NULL && current->data != data) {
        printf("%d -> ", current->data);
        if (data < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
        depth++;
    }

    if (current != NULL) {
        printf("%d\n", current->data);
        printf("Depth of node %d: %d\n", data, depth);
    } else {
        printf("... Node %d not found.\n", data);
    }
}

// 9. Free the entire tree from memory
void freeTree(Node* root) {
    if (root == NULL) {
        return;
    }
    // Use post-order traversal to free children before the parent
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// Helper function to ask the user about initializing the tree
Node* setupInitialTree(Node* root) {
    char confirm;
    printf("Start with a pre-populated tree for testing? (Y/N): ");
    
    // The space before %c is important to consume any leftover whitespace/newline
    scanf(" %c", &confirm); 

    // In C, characters are compared with single quotes
    if (confirm == 'Y' || confirm == 'y') {
        printf("Creating a test tree...\n");
        // Initial data to populate the tree
        int initial_nums[] = {50, 30, 70, 20, 40, 60, 80};
        int n = sizeof(initial_nums) / sizeof(initial_nums[0]);
        for(int i = 0; i < n; i++) {
            root = insert(root, initial_nums[i]);
        }
    } else {
        printf("Starting with an empty tree.\n");
    }
    return root;
}