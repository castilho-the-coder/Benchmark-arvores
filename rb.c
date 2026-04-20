#include <stdio.h>
#include <stdlib.h>
#include "rb.h"

// Nó sentinela
NO_RB sentinela = {0, PRETO, NULL, NULL, NULL};
#define NULO &sentinela

// Função para criar uma árvore Rubro-Negra vazia
ArvoreRB criarArvoreRB() {
    return NULO;
}

// Função auxiliar para destruir os nós da árvore
void destruirNosRB(NO_RB *no) {
    if (no == NULO) return;
    destruirNosRB(no->esq);
    destruirNosRB(no->dir);
    free(no);
}

// Função para destruir a árvore Rubro-Negra
void destruirArvoreRB(ArvoreRB *raiz) {
    if (raiz == NULL || *raiz == NULO) return;
    destruirNosRB(*raiz);
    *raiz = NULO;
}

// Rotações
void rotacaoEsquerdaRB(ArvoreRB *raiz, NO_RB *x) {
    NO_RB *y = x->dir;
    x->dir = y->esq;
    if (y->esq != NULO) {
        y->esq->pai = x;
    }
    y->pai = x->pai;
    if (x->pai == NULO) {
        *raiz = y;
    } else if (x == x->pai->esq) {
        x->pai->esq = y;
    } else {
        x->pai->dir = y;
    }
    y->esq = x;
    x->pai = y;
}

void rotacaoDireitaRB(ArvoreRB *raiz, NO_RB *y) {
    NO_RB *x = y->esq;
    y->esq = x->dir;
    if (x->dir != NULO) {
        x->dir->pai = y;
    }
    x->pai = y->pai;
    if (y->pai == NULO) {
        *raiz = x;
    } else if (y == y->pai->esq) {
        y->pai->esq = x;
    } else {
        y->pai->dir = x;
    }
    x->dir = y;
    y->pai = x;
}

// Função para corrigir a árvore após a inserção
void corrigirInsercaoRB(ArvoreRB *raiz, NO_RB *z) {
    while (z->pai->cor == VERMELHO) {
        if (z->pai == z->pai->pai->esq) {
            NO_RB *y = z->pai->pai->dir;
            if (y->cor == VERMELHO) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->dir) {
                    z = z->pai;
                    rotacaoEsquerdaRB(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoDireitaRB(raiz, z->pai->pai);
            }
        } else {
            NO_RB *y = z->pai->pai->esq;
            if (y->cor == VERMELHO) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esq) {
                    z = z->pai;
                    rotacaoDireitaRB(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoEsquerdaRB(raiz, z->pai->pai);
            }
        }
    }
    (*raiz)->cor = PRETO;
}

// Função para incluir um novo valor na árvore Rubro-Negra
int incluirInfoRB(ArvoreRB *raiz, int valor) {
    NO_RB *z = (NO_RB*) malloc(sizeof(NO_RB));
    if (z == NULL) return 0;
    z->info = valor;
    z->esq = NULO;
    z->dir = NULO;
    z->cor = VERMELHO;

    NO_RB *y = NULO;
    NO_RB *x = *raiz;

    while (x != NULO) {
        y = x;
        if (z->info < x->info) {
            x = x->esq;
        } else if (z->info > x->info) {
            x = x->dir;
        } else {
            free(z);
            return 0; // Valor já existe
        }
    }

    z->pai = y;
    if (y == NULO) {
        *raiz = z;
    } else if (z->info < y->info) {
        y->esq = z;
    } else {
        y->dir = z;
    }

    corrigirInsercaoRB(raiz, z);
    return 1;
}

// Função para buscar um valor na árvore Rubro-Negra
int buscarInfoRB(ArvoreRB raiz, int valor) {
    while (raiz != NULO && valor != raiz->info) {
        if (valor < raiz->info) {
            raiz = raiz->esq;
        } else {
            raiz = raiz->dir;
        }
    }
    return (raiz != NULO);
}

// Funções auxiliares para remoção
void transplantarRB(ArvoreRB *raiz, NO_RB *u, NO_RB *v) {
    if (u->pai == NULO) {
        *raiz = v;
    } else if (u == u->pai->esq) {
        u->pai->esq = v;
    } else {
        u->pai->dir = v;
    }
    v->pai = u->pai;
}

NO_RB* minimoRB(NO_RB* no) {
    while (no->esq != NULO) {
        no = no->esq;
    }
    return no;
}

void corrigirRemocaoRB(ArvoreRB *raiz, NO_RB *x) {
    while (x != *raiz && x->cor == PRETO) {
        if (x == x->pai->esq) {
            NO_RB *w = x->pai->dir;
            if (w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoEsquerdaRB(raiz, x->pai);
                w = x->pai->dir;
            }
            if (w->esq->cor == PRETO && w->dir->cor == PRETO) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (w->dir->cor == PRETO) {
                    w->esq->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoDireitaRB(raiz, w);
                    w = x->pai->dir;
                }
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->dir->cor = PRETO;
                rotacaoEsquerdaRB(raiz, x->pai);
                x = *raiz;
            }
        } else {
            NO_RB *w = x->pai->esq;
            if (w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoDireitaRB(raiz, x->pai);
                w = x->pai->esq;
            }
            if (w->dir->cor == PRETO && w->esq->cor == PRETO) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (w->esq->cor == PRETO) {
                    w->dir->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoEsquerdaRB(raiz, w);
                    w = x->pai->esq;
                }
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->esq->cor = PRETO;
                rotacaoDireitaRB(raiz, x->pai);
                x = *raiz;
            }
        }
    }
    x->cor = PRETO;
}

// Função para remover um valor da árvore Rubro-Negra
int removerInfoRB(ArvoreRB *raiz, int valor) {
    NO_RB *z = *raiz;
    while (z != NULO && z->info != valor) {
        if (valor < z->info) z = z->esq;
        else z = z->dir;
    }

    if (z == NULO) return 0; // Não encontrado

    NO_RB *y = z;
    NO_RB *x;
    Cor corOriginalY = y->cor;

    if (z->esq == NULO) {
        x = z->dir;
        transplantarRB(raiz, z, z->dir);
    } else if (z->dir == NULO) {
        x = z->esq;
        transplantarRB(raiz, z, z->esq);
    } else {
        y = minimoRB(z->dir);
        corOriginalY = y->cor;
        x = y->dir;
        if (y->pai == z) {
            x->pai = y;
        } else {
            transplantarRB(raiz, y, y->dir);
            y->dir = z->dir;
            y->dir->pai = y;
        }
        transplantarRB(raiz, z, y);
        y->esq = z->esq;
        y->esq->pai = y;
        y->cor = z->cor;
    }
    free(z);
    if (corOriginalY == PRETO) {
        corrigirRemocaoRB(raiz, x);
    }
    return 1;
}
