#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

// Função para criar uma árvore AVL vazia
ArvoreAVL criarArvoreAVL() {
    return NULL;
}

// Função auxiliar para destruir os nós da árvore
void destruirNosAVL(NO_AVL *no) {
    if (no == NULL) return;
    destruirNosAVL(no->esq);
    destruirNosAVL(no->dir);
    free(no);
}

// Função para destruir a árvore AVL
void destruirArvoreAVL(ArvoreAVL *raiz) {
    if (raiz == NULL) return;
    destruirNosAVL(*raiz);
    *raiz = NULL;
}

// Funções auxiliares para o balanceamento da AVL
int altura(NO_AVL *no) {
    if (no == NULL) return -1;
    return no->altura;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int fatorBalanceamento(NO_AVL *no) {
    if (no == NULL) return 0;
    return altura(no->esq) - altura(no->dir);
}

// Rotações
NO_AVL* rotacaoDireita(NO_AVL *y) {
    NO_AVL *x = y->esq;
    NO_AVL *T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;

    return x;
}

NO_AVL* rotacaoEsquerda(NO_AVL *x) {
    NO_AVL *y = x->dir;
    NO_AVL *T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;

    return y;
}

// Função auxiliar para incluir e balancear
NO_AVL* incluirNoAVL(NO_AVL* no, int valor) {
    if (no == NULL) {
        NO_AVL* novoNo = (NO_AVL*) malloc(sizeof(NO_AVL));
        if (novoNo == NULL) return NULL;
        novoNo->info = valor;
        novoNo->esq = NULL;
        novoNo->dir = NULL;
        novoNo->altura = 0;
        return novoNo;
    }

    if (valor < no->info) {
        no->esq = incluirNoAVL(no->esq, valor);
    } else if (valor > no->info) {
        no->dir = incluirNoAVL(no->dir, valor);
    } else {
        return no; // Valor já existe
    }

    no->altura = 1 + max(altura(no->esq), altura(no->dir));
    int fb = fatorBalanceamento(no);

    // Casos de desbalanceamento
    if (fb > 1 && valor < no->esq->info) {
        return rotacaoDireita(no);
    }
    if (fb < -1 && valor > no->dir->info) {
        return rotacaoEsquerda(no);
    }
    if (fb > 1 && valor > no->esq->info) {
        no->esq = rotacaoEsquerda(no->esq);
        return rotacaoDireita(no);
    }
    if (fb < -1 && valor < no->dir->info) {
        no->dir = rotacaoDireita(no->dir);
        return rotacaoEsquerda(no);
    }

    return no;
}

// Função principal de inclusão
int incluirInfoAVL(ArvoreAVL *raiz, int valor) {
    if (raiz == NULL) return 0;
    *raiz = incluirNoAVL(*raiz, valor);
    return 1;
}

// Função para buscar um valor na árvore AVL
int buscarInfoAVL(ArvoreAVL raiz, int valor) {
    if (raiz == NULL) return 0;
    if (valor == raiz->info) return 1;
    if (valor < raiz->info) {
        return buscarInfoAVL(raiz->esq, valor);
    } else {
        return buscarInfoAVL(raiz->dir, valor);
    }
}

// Função auxiliar para encontrar o menor nó
NO_AVL* encontrarMenorAVL(NO_AVL* no) {
    NO_AVL* atual = no;
    while (atual && atual->esq != NULL) {
        atual = atual->esq;
    }
    return atual;
}


// Função auxiliar para remover e balancear
NO_AVL* removerNoAVL(NO_AVL* raiz, int valor) {
    if (raiz == NULL) return raiz;

    if (valor < raiz->info) {
        raiz->esq = removerNoAVL(raiz->esq, valor);
    } else if (valor > raiz->info) {
        raiz->dir = removerNoAVL(raiz->dir, valor);
    } else {
        // Nó a ser removido encontrado
        if (raiz->esq == NULL || raiz->dir == NULL) {
            NO_AVL *temp = raiz->esq ? raiz->esq : raiz->dir;
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else {
                *raiz = *temp;
            }
            free(temp);
        } else {
            NO_AVL* temp = encontrarMenorAVL(raiz->dir);
            raiz->info = temp->info;
            raiz->dir = removerNoAVL(raiz->dir, temp->info);
        }
    }

    if (raiz == NULL) return raiz;

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));
    int fb = fatorBalanceamento(raiz);

    // Casos de desbalanceamento
    if (fb > 1 && fatorBalanceamento(raiz->esq) >= 0) {
        return rotacaoDireita(raiz);
    }
    if (fb > 1 && fatorBalanceamento(raiz->esq) < 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }
    if (fb < -1 && fatorBalanceamento(raiz->dir) <= 0) {
        return rotacaoEsquerda(raiz);
    }
    if (fb < -1 && fatorBalanceamento(raiz->dir) > 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

// Função principal de remoção
int removerInfoAVL(ArvoreAVL *raiz, int valor) {
    if (raiz == NULL) return 0;
    *raiz = removerNoAVL(*raiz, valor);
    return 1;
}
