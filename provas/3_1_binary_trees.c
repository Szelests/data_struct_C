/*
6) Arvore binária ordenada pelo campo "nome"
a) Desenvolva uma função para listar os nomes em ordem alfabética decrescente;
b) Desenvolva uma função para listar na tela todos os alunos Aprovados (é considerado aprovado o aluno com média maior ou igual a 6);
c) Desenvolva uma função que retorna a quantidade de alunos.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Nota
{
    char nome[40];
    float media;
    struct Nota* esq;
    struct Nota* dir;
} Nota;


//============= ESSENTIAL FUNCTIONS ======================
// Função para criar um novo nó
Nota* criarNota(const char* nome, float media) {
    Nota* novo = (Nota*)malloc(sizeof(Nota));
    strcpy(novo->nome, nome);
    novo->media = media;
    novo->esq = novo->dir = NULL;
    return novo;
}

// Inserção ordenada pelo nome
Nota* inserir(Nota* raiz, const char* nome, float media) {
    if (raiz == NULL) return criarNota(nome, media);
    if (strcmp(nome, raiz->nome) < 0)
        raiz->esq = inserir(raiz->esq, nome, media);
    else
        raiz->dir = inserir(raiz->dir, nome, media);
    return raiz;
}
//===========================================================

// a)
void alphabetical(Nota* node) 
{
    if(node != NULL) 
    {
        alphabetical(node->dir); 
        printf("%s\n", node->nome); 
        alphabetical(node->esq);
    }
}

// b) 
void approved(Nota* node)
{
    if(node != NULL)
    {
        approved(node->dir);
        if(node->media >= 6) printf("%s: %.2f\n", node->nome, node->media );
        approved(node->esq);
    }
}

// c)
// There are two ways to make this function.
// -> The first one is using static, but eif the function is called more than once it will sum over and over without reset the counter value.
// -> The correct way to make is returning the value in each iteration

// 1. First way
/*
int how_many(Nota* node)
{
    static int count = 0;
    if(node != NULL) 
    {
        how_many(node->dir);
        count++;
        how_many(node->esq);
    }

    return count;
}
*/

// 2. Second way (and correct way)
int how_many(Nota* node) 
{
    if(node == NULL) return 0;

    return 1 + how_many(node->dir) + how_many(node->esq);
}


int main() {
    Nota* raiz = NULL;

    // Inserindo alguns alunos
    raiz = inserir(raiz, "Carlos", 7.5);
    raiz = inserir(raiz, "Ana", 5.0);
    raiz = inserir(raiz, "Bruno", 8.0);
    raiz = inserir(raiz, "Diana", 6.0);
    raiz = inserir(raiz, "Eduardo", 4.5);

    printf("Nomes em ordem alfabética decrescente:\n");
    alphabetical(raiz);

    printf("\nAlunos aprovados:\n");
    approved(raiz);

    printf("\nQuantidade de alunos: %d\n", how_many(raiz));

    // Liberação de memória omitida para simplicidade
    return 0;
}