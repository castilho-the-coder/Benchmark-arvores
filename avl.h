#ifndef AVL_H
#define AVL_H

// Estrutura do nó da árvore AVL
typedef struct NO_AVL {
    int info;
    int altura;
    struct NO_AVL *esq;
    struct NO_AVL *dir;
} NO_AVL;

// Ponteiro para a árvore (raiz)
typedef NO_AVL* ArvoreAVL;

// Protótipos das funções
ArvoreAVL criarArvoreAVL();
void destruirArvoreAVL(ArvoreAVL *raiz);
int incluirInfoAVL(ArvoreAVL *raiz, int valor);
int buscarInfoAVL(ArvoreAVL raiz, int valor);
int removerInfoAVL(ArvoreAVL *raiz, int valor);

#endif // AVL_H
