#pragma once

#include <stdlib.h>
#include <stdio.h>

// Define the structure for a binary tree node
typedef struct 
{
    int data;
    struct Node* left;
    struct Node* right;
    int height; // Optional: can be used for AVL trees or to store height of the node
} Node; 

// === Function to create a new Node ===
/**
 * @brief Creates a new Node with the given data.
 * @param data The data to be stored in the Node.
 * @return A pointer to the newly created Node.
 */
Node* createNode(int data);

/**
 * @brief Performs an in-order traversal of the binary tree: Left->Root->Right.
 * @param root A pointer to the root of the binary tree.
 */
void inOrderTraversal(Node* root);

/**
 * @brief Performs a pre-order traversal of the binary tree: Root->Left->Right.
 * @param root A pointer to the root of the binary tree.
 */
void preOrderTraversal(Node* root);

/**
 * @brief Performs a post-order traversal of the binary tree: Left->Right->Root.
 * @param root A pointer to the root of the binary tree.
 * @note This function is typically used to free memory allocated for the tree nodes.
 */
void postOrderTraversal(Node* root);

// === Manly tree functions ===

/**
 * @brief Inserts a new Node with the given data into the binary tree.
 * @param root A pointer to the root of the binary tree.
 * @param data The data to be inserted into the tree.
 * @return A pointer to the root of the modified binary tree.
 */
Node* insert(Node* root, int data);

/**
 * @brief Searches for a Node with the specified data in the binary tree.
 * @param root A pointer to the root of the binary tree.
 * @param data The data to be searched for.
 * @return A pointer to the found Node, or NULL if not found.
 */
Node *search(Node* root, int data);

/**
 * @brief Finds the minimum value Node in the binary tree.
 * @param root A pointer to the root of the binary tree.
 * @return A pointer to the Node with the minimum value, or NULL if the tree is empty.
 */
Node* findMinValue(Node* node);

/**
 * @brief Finds the maximum value Node in the binary tree.
 * @param node A pointer to the root of the binary tree.
 * @return A pointer to the Node with the maximum value, or NULL if the tree is empty.
 */
Node* findMaxValue(Node* node);

/**
 * @brief Deletes a Node with the specified data from the binary tree.
 * @param root A pointer to the root of the binary tree.
 * @param data The data of the Node to be deleted.
 * @return A pointer to the root of the modified binary tree.
 */
Node* delete(Node* root, int data);

/*
   ============================================================
   === INÍCIO DO COMPLEMENTO PARA ÁRVORE BINÁRIA AVL ===
   ============================================================
*/

/**
 * @brief Calculates the height of a Node in the binary tree.
 * @param node A pointer to the Node whose height is to be calculated.
 * @return The height of the Node.
 */
int height(Node* node);

/**
 * @brief Calculates the balance factor of a Node in the binary tree.
 * @param node A pointer to the Node whose balance factor is to be calculated.
 * @return The balance factor of the Node.
 */
int getBalanceFactor(Node* node);

/**
 * @brief Returns the maximum of two integers.
 * @param a The first integer.
 * @param b The second integer.
 * @return The maximum of a and b.
 */
int max(int a, int b);

// Rotação Simples à Esquerda
//     x               y
//    / \             / \
//   T1  y      ->   x   T3
//      / \         / \
//     T2  T3      T1  T2
// === AVL Tree Rotations ===
/**
 * @brief Performs a right rotation on the given Node.
 * @param y A pointer to the Node to be rotated.
 * @return A pointer to the new root of the subtree after rotation.
 */
Node* rightRotate(Node* y);

// Rotação Simples à Direita
//       y           x
//      / \         / \
//     x   T3  ->  T1  y
//    / \             / \
//   T1  T2          T2  T3
/**
 * @brief Performs a left rotation on the given Node.
 * @param x A pointer to the Node to be rotated.
 * @return A pointer to the new root of the subtree after rotation.
 */
Node* leftRotate(Node* x);

// --- Essential AVL Tree Functions ---

/**
 * @brief Inserts a new Node with the given data into the AVL tree, maintaining its balance.
 * @param node A pointer to the root of the AVL tree.
 * @param data The data to be inserted into the tree.
 * @return A pointer to the root of the modified AVL tree.
 */
Node* insertAVL(Node* node, int data);

/**
 * @brief Deletes a Node with the specified data from the AVL tree, maintaining its balance.
 * @param root A pointer to the root of the AVL tree.
 * @param data The data of the Node to be deleted.
 * @return A pointer to the root of the modified AVL tree.
 */
Node* deleteAVL(Node* root, int data);