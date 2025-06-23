/*
7) Escrever uma função em C que receba como parâmetro a raiz de uma árvore binária de inteiros não nula e retorne o menor valor da subárvore da direita ou -1 caso a subárvore da direita esteja vazia.
*/

typedef struct tree 
{
    int num;
    struct tree *esq;
    struct tree *dir;
} Tree;

#include <stdio.h>
#include <stdlib.h>

// Função para inserir na árvore binária de busca
Tree* inserir(Tree* raiz, int num) {
    if (raiz == NULL) {
        Tree* novo = (Tree*)malloc(sizeof(Tree));
        novo->num = num;
        novo->esq = novo->dir = NULL;
        return novo;
    }
    if (num < raiz->num)
        raiz->esq = inserir(raiz->esq, num);
    else
        raiz->dir = inserir(raiz->dir, num);
    return raiz;
}

int get_smaller(Tree* root) 
{
    if(root == NULL || root->dir == NULL) return -1;
    Tree *current = root->dir;

    while (current->esq != NULL) current = current->esq;

    return current->num;
}

int main() {
    Tree* raiz = NULL;
    raiz = inserir(raiz, 20);
    raiz = inserir(raiz, 10);
    raiz = inserir(raiz, 30);
    raiz = inserir(raiz, 25);
    raiz = inserir(raiz, 40);
    raiz = inserir(raiz, 22);

    printf("Menor valor da subárvore direita de 20: %d\n", get_smaller(raiz)); // Deve imprimir 22

    // Testando caso sem subárvore direita
    Tree* so_esquerda = inserir(NULL, 50);
    so_esquerda = inserir(so_esquerda, 40);
    printf("Menor valor da subárvore direita de 50: %d\n", get_smaller(so_esquerda)); // Deve imprimir -1

    return 0;
}