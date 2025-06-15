#pragma once

#include <stdlib.h>
#include <stdio.h>

// Define the structure for a binary tree node
typedef struct 
{
    int data;
    struct Node* left;
    struct Node* right;
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