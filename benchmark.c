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

    const int NUM_EXECUCOES = 5;
    int tamanhos[] = {1000, 10000, 100000, 1000000};
    const char* nomes_arquivos[] = {"chaves_1k.txt", "chaves_10k.txt", "chaves_100k.txt", "chaves_1m.txt"};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);

    time_t inicio_total, fim_total;
    char buffer_inicio[26], buffer_fim[26];
    struct tm* tm_info;

    time(&inicio_total);
    tm_info = localtime(&inicio_total);
    strftime(buffer_inicio, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Iniciando Benchmark em: %s\n\n", buffer_inicio);

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

        double total_insercao_abb = 0, total_busca_abb = 0, total_remocao_abb = 0;
        double total_insercao_avl = 0, total_busca_avl = 0, total_remocao_avl = 0;
        double total_insercao_rb = 0, total_busca_rb = 0, total_remocao_rb = 0;

        for (int j = 0; j < NUM_EXECUCOES; j++) {
            shuffle(dados, tamanho); // Re-embaralhar para cada execução

            // --- Benchmark ABB ---
            ArvoreABB abb = criarArvoreABB();
            total_insercao_abb += medir_tempo(inserir_abb_wrapper, &abb, dados, tamanho);
            total_busca_abb += medir_tempo(buscar_abb_wrapper, &abb, dados, tamanho);
            total_remocao_abb += medir_tempo(remover_abb_wrapper, &abb, dados, tamanho);
            destruirArvoreABB(&abb);

            // --- Benchmark AVL ---
            ArvoreAVL avl = criarArvoreAVL();
            total_insercao_avl += medir_tempo(inserir_avl_wrapper, &avl, dados, tamanho);
            total_busca_avl += medir_tempo(buscar_avl_wrapper, &avl, dados, tamanho);
            total_remocao_avl += medir_tempo(remover_avl_wrapper, &avl, dados, tamanho);
            destruirArvoreAVL(&avl);

            // --- Benchmark Rubro-Negra ---
            ArvoreRB rb = criarArvoreRB();
            total_insercao_rb += medir_tempo(inserir_rb_wrapper, &rb, dados, tamanho);
            total_busca_rb += medir_tempo(buscar_rb_wrapper, &rb, dados, tamanho);
            total_remocao_rb += medir_tempo(remover_rb_wrapper, &rb, dados, tamanho);
            destruirArvoreRB(&rb);
        }

        printf("Resultados (Media de %d execucoes):\n\n", NUM_EXECUCOES);

        printf("ABB:\n");
        printf("  Tempo Medio de Insercao: %f segundos\n", total_insercao_abb / NUM_EXECUCOES);
        printf("  Tempo Medio de Busca:    %f segundos\n", total_busca_abb / NUM_EXECUCOES);
        printf("  Tempo Medio de Remocao:  %f segundos\n\n", total_remocao_abb / NUM_EXECUCOES);

        printf("AVL:\n");
        printf("  Tempo Medio de Insercao: %f segundos\n", total_insercao_avl / NUM_EXECUCOES);
        printf("  Tempo Medio de Busca:    %f segundos\n", total_busca_avl / NUM_EXECUCOES);
        printf("  Tempo Medio de Remocao:  %f segundos\n\n", total_remocao_avl / NUM_EXECUCOES);

        printf("Rubro-Negra:\n");
        printf("  Tempo Medio de Insercao: %f segundos\n", total_insercao_rb / NUM_EXECUCOES);
        printf("  Tempo Medio de Busca:    %f segundos\n", total_busca_rb / NUM_EXECUCOES);
        printf("  Tempo Medio de Remocao:  %f segundos\n\n", total_remocao_rb / NUM_EXECUCOES);

        free(dados);
    }

    time(&fim_total);
    tm_info = localtime(&fim_total);
    strftime(buffer_fim, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Benchmark finalizado em: %s\n", buffer_fim);

    return 0;
}
