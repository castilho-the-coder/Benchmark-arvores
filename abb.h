#ifndef ABB_H
#define ABB_H

// Estrutura do nó da árvore ABB
typedef struct NO_ABB {
    int info;
    struct NO_ABB *esq;
    struct NO_ABB *dir;
} NO_ABB;

// Ponteiro para a árvore (raiz)
typedef NO_ABB* ArvoreABB;

// Protótipos das funções
ArvoreABB criarArvoreABB();
void destruirArvoreABB(ArvoreABB *raiz);
int incluirInfoABB(ArvoreABB *raiz, int valor);
int buscarInfoABB(ArvoreABB raiz, int valor);
int removerInfoABB(ArvoreABB *raiz, int valor);

#endif // ABB_H
