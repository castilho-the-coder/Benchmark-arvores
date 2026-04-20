#ifndef RB_H
#define RB_H

// Cores dos nós da árvore Rubro-Negra
typedef enum { VERMELHO, PRETO } Cor;

// Estrutura do nó da árvore Rubro-Negra
typedef struct NO_RB {
    int info;
    Cor cor;
    struct NO_RB *esq;
    struct NO_RB *dir;
    struct NO_RB *pai;
} NO_RB;

// Ponteiro para a árvore (raiz)
typedef NO_RB* ArvoreRB;

// Protótipos das funções
ArvoreRB criarArvoreRB();
void destruirArvoreRB(ArvoreRB *raiz);
int incluirInfoRB(ArvoreRB *raiz, int valor);
int buscarInfoRB(ArvoreRB raiz, int valor);
int removerInfoRB(ArvoreRB *raiz, int valor);

#endif // RB_H
