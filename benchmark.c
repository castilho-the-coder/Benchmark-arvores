#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "abb.h"
#include "avl.h"
#include "rb.h"

//======================================
// Função de embaralhamento (Fisher-Yates)
void shuffle(int *v, int n) {
    if (n > 1) {
        for (int i = n - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int temp = v[i];
            v[i] = v[j];
            v[j] = temp;
        }
    }
}

//======================================
// Função para gerar dados, salvar em arquivo e preencher o vetor
void gerar_dados(int *vetor, int tamanho, const char* nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo %s\n", nome_arquivo);
        exit(1);
    }

    for (int i = 0; i < tamanho; i++) {
        vetor[i] = i + 1;
    }

    shuffle(vetor, tamanho);

    fprintf(arquivo, "Chaves geradas para o conjunto de %d elementos:\n", tamanho);
    for (int i = 0; i < tamanho; i++) {
        fprintf(arquivo, "%d\n", vetor[i]);
    }

    fclose(arquivo);
}

//======================================
// Função para medir o tempo de uma operação
double medir_tempo(void (*operacao)(void*, int), void* arvore, int* dados, int tamanho) {
    clock_t inicio, fim;
    inicio = clock();
    for (int i = 0; i < tamanho; i++) {
        operacao(arvore, dados[i]);
    }
    fim = clock();
    return ((double)(fim - inicio)) / CLOCKS_PER_SEC;
}

// Wrappers para as funções de inserção, busca e remoção
void inserir_abb_wrapper(void* arvore, int valor) {
    incluirInfoABB((ArvoreABB*)arvore, valor);
}
void buscar_abb_wrapper(void* arvore, int valor) {
    buscarInfoABB(*(ArvoreABB*)arvore, valor);
}
void remover_abb_wrapper(void* arvore, int valor) {
    removerInfoABB((ArvoreABB*)arvore, valor);
}

void inserir_avl_wrapper(void* arvore, int valor) {
    incluirInfoAVL((ArvoreAVL*)arvore, valor);
}
void buscar_avl_wrapper(void* arvore, int valor) {
    buscarInfoAVL(*(ArvoreAVL*)arvore, valor);
}
void remover_avl_wrapper(void* arvore, int valor) {
    removerInfoAVL((ArvoreAVL*)arvore, valor);
}

void inserir_rb_wrapper(void* arvore, int valor) {
    incluirInfoRB((ArvoreRB*)arvore, valor);
}
void buscar_rb_wrapper(void* arvore, int valor) {
    buscarInfoRB(*(ArvoreRB*)arvore, valor);
}
void remover_rb_wrapper(void* arvore, int valor) {
    removerInfoRB((ArvoreRB*)arvore, valor);
}


//===================================
int main() {
    srand(time(NULL));

    int tamanhos[] = {1000, 10000, 100000, 1000000};
    const char* nomes_arquivos[] = {"chaves_1k.txt", "chaves_10k.txt", "chaves_100k.txt", "chaves_1m.txt"};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);

    printf("Iniciando Benchmark...\n\n");

    for (int i = 0; i < num_tamanhos; i++) {
        int tamanho = tamanhos[i];
        int *dados = (int*) malloc(tamanho * sizeof(int));
        if (dados == NULL) {
            printf("Erro de alocacao de memoria para os dados.\n");
            return 1;
        }

        printf("--------------------------------------------------\n");
        printf("Conjunto de Dados: %d elementos\n", tamanho);
        printf("--------------------------------------------------\n");

        gerar_dados(dados, tamanho, nomes_arquivos[i]);
        printf("Arquivo %s gerado com as chaves embaralhadas.\n\n", nomes_arquivos[i]);

        // --- Benchmark ABB ---
        ArvoreABB abb = criarArvoreABB();
        double tempo_insercao_abb = medir_tempo(inserir_abb_wrapper, &abb, dados, tamanho);
        double tempo_busca_abb = medir_tempo(buscar_abb_wrapper, &abb, dados, tamanho);
        double tempo_remocao_abb = medir_tempo(remover_abb_wrapper, &abb, dados, tamanho);
        destruirArvoreABB(&abb);

        printf("ABB:\n");
        printf("  Tempo de Insercao: %f segundos\n", tempo_insercao_abb);
        printf("  Tempo de Busca:    %f segundos\n", tempo_busca_abb);
        printf("  Tempo de Remocao:  %f segundos\n\n", tempo_remocao_abb);

        // --- Benchmark AVL ---
        ArvoreAVL avl = criarArvoreAVL();
        double tempo_insercao_avl = medir_tempo(inserir_avl_wrapper, &avl, dados, tamanho);
        double tempo_busca_avl = medir_tempo(buscar_avl_wrapper, &avl, dados, tamanho);
        double tempo_remocao_avl = medir_tempo(remover_avl_wrapper, &avl, dados, tamanho);
        destruirArvoreAVL(&avl);

        printf("AVL:\n");
        printf("  Tempo de Insercao: %f segundos\n", tempo_insercao_avl);
        printf("  Tempo de Busca:    %f segundos\n", tempo_busca_avl);
        printf("  Tempo de Remocao:  %f segundos\n\n", tempo_remocao_avl);

        // --- Benchmark Rubro-Negra ---
        ArvoreRB rb = criarArvoreRB();
        double tempo_insercao_rb = medir_tempo(inserir_rb_wrapper, &rb, dados, tamanho);
        double tempo_busca_rb = medir_tempo(buscar_rb_wrapper, &rb, dados, tamanho);
        double tempo_remocao_rb = medir_tempo(remover_rb_wrapper, &rb, dados, tamanho);
        destruirArvoreRB(&rb);

        printf("Rubro-Negra:\n");
        printf("  Tempo de Insercao: %f segundos\n", tempo_insercao_rb);
        printf("  Tempo de Busca:    %f segundos\n", tempo_busca_rb);
        printf("  Tempo de Remocao:  %f segundos\n\n", tempo_remocao_rb);

        free(dados);
    }

    printf("Benchmark finalizado.\n");

    return 0;
}
