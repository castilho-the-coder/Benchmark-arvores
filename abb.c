#include <stdio.h>
#include <stdlib.h>
#include "abb.h"

// Função para criar uma árvore ABB vazia
ArvoreABB criarArvoreABB() {
    return NULL;
}

// Função auxiliar para destruir os nós da árvore
void destruirNosABB(NO_ABB *no) {
    if (no == NULL) return;
    destruirNosABB(no->esq);
    destruirNosABB(no->dir);
    free(no);
}

// Função para destruir a árvore ABB
void destruirArvoreABB(ArvoreABB *raiz) {
    if (raiz == NULL) return;
    destruirNosABB(*raiz);
    *raiz = NULL;
}

// Função para incluir um novo valor na árvore ABB
int incluirInfoABB(ArvoreABB *raiz, int valor) {
    if (raiz == NULL) return 0;
    if (*raiz == NULL) {
        NO_ABB *novoNo = (NO_ABB*) malloc(sizeof(NO_ABB));
        if (novoNo == NULL) return 0;
        novoNo->info = valor;
        novoNo->esq = NULL;
        novoNo->dir = NULL;
        *raiz = novoNo;
        return 1;
    }

    if (valor < (*raiz)->info) {
        return incluirInfoABB(&((*raiz)->esq), valor);
    } else if (valor > (*raiz)->info) {
        return incluirInfoABB(&((*raiz)->dir), valor);
    } else {
        // Valor já existe
        return 0;
    }
}

// Função para buscar um valor na árvore ABB
int buscarInfoABB(ArvoreABB raiz, int valor) {
    if (raiz == NULL) return 0; // Não encontrado
    if (valor == raiz->info) return 1; // Encontrado
    if (valor < raiz->info) {
        return buscarInfoABB(raiz->esq, valor);
    } else {
        return buscarInfoABB(raiz->dir, valor);
    }
}

// Função auxiliar para encontrar o menor nó
NO_ABB* encontrarMenor(NO_ABB* no) {
    NO_ABB* atual = no;
    while (atual && atual->esq != NULL) {
        atual = atual->esq;
    }
    return atual;
}

// Função para remover um valor da árvore ABB
int removerInfoABB(ArvoreABB *raiz, int valor) {
    if (raiz == NULL || *raiz == NULL) return 0;

    if (valor < (*raiz)->info) {
        return removerInfoABB(&((*raiz)->esq), valor);
    } else if (valor > (*raiz)->info) {
        return removerInfoABB(&((*raiz)->dir), valor);
    } else {
        // Nó a ser removido encontrado
        NO_ABB *temp;
        if ((*raiz)->esq == NULL) {
            temp = *raiz;
            *raiz = (*raiz)->dir;
            free(temp);
        } else if ((*raiz)->dir == NULL) {
            temp = *raiz;
            *raiz = (*raiz)->esq;
            free(temp);
        } else {
            // Nó com dois filhos
            NO_ABB* sucessor = encontrarMenor((*raiz)->dir);
            (*raiz)->info = sucessor->info;
            removerInfoABB(&((*raiz)->dir), sucessor->info);
        }
        return 1;
    }
}
